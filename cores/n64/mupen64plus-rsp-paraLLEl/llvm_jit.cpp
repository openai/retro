#include "llvm_jit.hpp"

#include <clang/CodeGen/CodeGenAction.h>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
#include <clang/Driver/Tool.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/ObjectCache.h>
#include <llvm/ExecutionEngine/RuntimeDyld.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>

#include <stdio.h>
using namespace clang;
using namespace std;

namespace JIT
{
struct Block::Impl
{
	Impl(LLVMEngine &engine_)
	    : engine(engine_)
	{
	}

	LLVMEngine &engine;
	Func block = nullptr;
	size_t block_size = 0;
	bool compile(const std::string &source);
};

Block::Block(LLVMEngine &engine)
{
	impl = std::unique_ptr<Impl>(new Impl(engine));
}

Block::~Block()
{
}

struct LLVMHolder
{
	LLVMHolder()
	{
		llvm::InitializeNativeTarget();
		llvm::InitializeNativeTargetAsmPrinter();
		llvm::InitializeNativeTargetAsmParser();
	}

	~LLVMHolder()
	{
		llvm::llvm_shutdown();
	}
};

struct LLVMEngine::Impl
{
	Impl(const std::unordered_map<std::string, uint64_t> &symbol_table_)
	    : symbol_table(symbol_table_)
	{
		static LLVMHolder llvm_holder;

		execution_session = llvm::make_unique<llvm::orc::ExecutionSession>();
		execution_session->setErrorReporter([](llvm::Error error) {
			if (error)
				llvm::errs() << "Error: " << error << "\n";
		});

		llvm::orc::LegacyRTDyldObjectLinkingLayer::Resources resources;
		resources.MemMgr = llvm::make_unique<llvm::SectionMemoryManager>();
		resources.Resolver = llvm::orc::createLegacyLookupResolver(
		    *execution_session, [this](const std::string &name) -> llvm::JITSymbol { return findSymbol(name); },
		    [](llvm::Error) {});

		object_layer = llvm::make_unique<llvm::orc::LegacyRTDyldObjectLinkingLayer>(
		    *execution_session, [=](llvm::orc::VModuleKey) { return resources; });

		auto host = llvm::orc::JITTargetMachineBuilder::detectHost();
		target_machine = llvm::cantFail(host->createTargetMachine());
		target_machine->setOptLevel(llvm::CodeGenOpt::Level::Default);
		data_layout = llvm::make_unique<llvm::DataLayout>(std::move(*host->getDefaultDataLayoutForTarget()));
		compile_layer = llvm::make_unique<
		    llvm::orc::LegacyIRCompileLayer<llvm::orc::LegacyRTDyldObjectLinkingLayer, llvm::orc::SimpleCompiler>>(
		    *object_layer, llvm::orc::SimpleCompiler(*target_machine));
	}

	std::unique_ptr<EmitLLVMOnlyAction> compile_c(const std::string &source)
	{
		llvm::SmallVector<const char *, 4> args;
		args.push_back("__block.c");
		args.push_back("-std=c99");
		args.push_back("-O2");

		std::string string_buffer;
		llvm::raw_string_ostream ss(string_buffer);

		IntrusiveRefCntPtr<DiagnosticOptions> diag_opts = new DiagnosticOptions();
		TextDiagnosticPrinter *diag_client = new TextDiagnosticPrinter(ss, &*diag_opts);
		IntrusiveRefCntPtr<DiagnosticIDs> diag_id(new DiagnosticIDs());
		DiagnosticsEngine diags(diag_id, &*diag_opts, diag_client);

		auto CI = llvm::make_unique<CompilerInvocation>();
		auto *invocation = CI.get();
		CompilerInvocation::CreateFromArgs(*CI, args.data(), args.data() + args.size(), diags);

		auto clang = llvm::make_unique<CompilerInstance>();
		clang->setInvocation(std::move(CI));
		clang->createDiagnostics();

		auto act = llvm::make_unique<EmitLLVMOnlyAction>();

		StringRef code_data(source);
		auto buffer = llvm::MemoryBuffer::getMemBufferCopy(code_data);
		invocation->getPreprocessorOpts().clearRemappedFiles();
		invocation->getPreprocessorOpts().addRemappedFile("__block.c", buffer.release());

		if (!clang->ExecuteAction(*act))
		{
			llvm::errs() << "ExecuteAction failed.\n";
			return {};
		}
		return act;
	}

	Func compile(const std::string &source)
	{
		auto act = compile_c(source);
		if (!act)
			return nullptr;

		auto K = execution_session->allocateVModule();
		auto error = compile_layer->addModule(K, act->takeModule());

		if (error)
			return nullptr;

		auto entry_point = compile_layer->findSymbolIn(K, "block_entry", true);
		auto block = reinterpret_cast<Func>(llvm::cantFail(entry_point.getAddress()));
		return block;
	}

	llvm::JITSymbol findSymbol(const std::string &name)
	{
		auto itr = symbol_table.find(name);
		if (itr != symbol_table.end())
			return llvm::JITSymbol(itr->second, llvm::JITSymbolFlags::None);
		else
			return llvm::JITSymbol(nullptr);
	}

	const std::unordered_map<std::string, uint64_t> &symbol_table;

	llvm::LLVMContext context;
	std::unique_ptr<llvm::orc::ExecutionSession> execution_session;
	std::unique_ptr<llvm::orc::LegacyRTDyldObjectLinkingLayer> object_layer;
	std::unique_ptr<
	    llvm::orc::LegacyIRCompileLayer<llvm::orc::LegacyRTDyldObjectLinkingLayer, llvm::orc::SimpleCompiler>>
	    compile_layer;
	std::unique_ptr<llvm::TargetMachine> target_machine;
	std::unique_ptr<llvm::orc::MangleAndInterner> mangler;
	std::unique_ptr<llvm::DataLayout> data_layout;
};

LLVMEngine::LLVMEngine(const std::unordered_map<std::string, uint64_t> &symbol_table)
{
	impl.reset(new Impl(symbol_table));
}

LLVMEngine::~LLVMEngine()
{
}

bool Block::compile(uint64_t, const std::string &source)
{
	bool ret = impl->compile(source);
	if (ret)
	{
		block = impl->block;
		block_size = impl->block_size;
	}
	return ret;
}

bool Block::Impl::compile(const std::string &source)
{
	block = engine.impl->compile(source);
	return block != nullptr;
}
} // namespace JIT
