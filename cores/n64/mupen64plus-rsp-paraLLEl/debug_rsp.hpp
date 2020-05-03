#ifndef RSP_HPP__
#define RSP_HPP__

#include <memory>
#include <stdint.h>
#include <string.h>
#include <string>
#include <unordered_map>

#include "debug_jit.hpp"
#include "llvm_jit.hpp"
#include "rsp_op.hpp"
#include "state.hpp"

namespace RSP
{
#ifdef DEBUG_JIT
using Block = JIT::DebugBlock;
#else
using Block = JIT::Block;
#endif
using Func = JIT::Func;


class alignas(64) CPU
{
public:
	CPU();
	~CPU();

	CPU(CPU &&) = delete;
	void operator=(CPU &&) = delete;

	void set_dmem(uint32_t *dmem)
	{
		state.dmem = dmem;
	}

	void set_imem(uint32_t *imem)
	{
		state.imem = imem;
	}

	void set_rdram(uint32_t *rdram)
	{
		state.rdram = rdram;
	}

	void invalidate_imem();

	CPUState &get_state()
	{
		return state;
	}

	ReturnMode run();

	void enter(uint32_t pc);
	void call(uint32_t target, uint32_t ret);
	int ret(uint32_t pc);
	void exit(ReturnMode mode);

	void print_registers();

private:
	CPUState state;
	Func blocks[IMEM_WORDS] = {};
	std::unordered_map<std::string, uint64_t> symbol_table;
#ifndef DEBUG_JIT
	JIT::LLVMEngine jit_engine;
#endif
	std::unordered_map<uint64_t, std::unique_ptr<Block>> cached_blocks[IMEM_WORDS];

	void invalidate_code();
	uint64_t hash_imem(unsigned pc, unsigned count) const;
	Func jit_region(uint64_t hash, unsigned pc, unsigned count);

	std::string full_code;
	std::string body;

	void init_symbol_table();

	alignas(64) uint32_t cached_imem[IMEM_WORDS] = {};

	// Platform specific.
#ifdef __GNUC__
	intptr_t env[64];
	// We're reading this after setjmp returns so need to make sure the read happens when we expect it to.
	volatile ReturnMode return_mode;
#else
#error "Need __builtin_setjmp/longjmp support alternative for other compilers ..."
#endif

#define CALL_STACK_SIZE 32
	uint32_t call_stack[CALL_STACK_SIZE] = {};
	unsigned call_stack_ptr = 0;

	unsigned analyze_static_end(unsigned pc, unsigned end);
};
} // namespace RSP

#endif
