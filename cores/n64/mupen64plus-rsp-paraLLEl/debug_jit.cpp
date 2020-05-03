#include "debug_jit.hpp"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

namespace JIT
{

struct DebugBlock::Impl
{
	Impl() = default;
	Impl(Impl &&) = delete;
	void operator=(Impl &&) = delete;
	~Impl();

	void *dylib = nullptr;
	Func block = nullptr;
	string name, soname;

	bool compile(uint64_t hash, const std::string &source);
};

DebugBlock::DebugBlock(const unordered_map<string, uint64_t> &)
{
}

DebugBlock::~DebugBlock()
{
}

DebugBlock::Impl::~Impl()
{
	if (dylib)
		dlclose(dylib);

	remove(soname.c_str());
	remove(name.c_str());
}

bool DebugBlock::compile(uint64_t hash, const std::string &source)
{
	impl = unique_ptr<Impl>(new Impl);
	bool ret = impl->compile(hash, source);
	if (ret)
		block = impl->block;
	return ret;
}

bool DebugBlock::Impl::compile(uint64_t hash, const std::string &source)
{
	name = "/tmp/";
	name += to_string(hash);
	soname = name;
	name += ".c";
	soname += ".so";

	FILE *file = fopen(name.c_str(), "w");
	if (!file)
		return false;

	fputs(source.c_str(), file);
	fclose(file);

	char command[256];
	if (sizeof(size_t) == 8)
	{
		sprintf(command, "gcc -o %s %s -shared -fpic -O0 -g -std=c99 -Wl,--unresolved-symbols=ignore-all",
		        soname.c_str(),
		        name.c_str());
	}
	else if (sizeof(size_t) == 4)
	{
		sprintf(command, "gcc -m32 -o %s %s -shared -fpic -O0 -g -std=c99 -Wl,--unresolved-symbols=ignore-all",
		        soname.c_str(),
		        name.c_str());
	}
	int ret = system(command);
	if (ret != 0)
		return false;

	dylib = dlopen(soname.c_str(), RTLD_LOCAL | RTLD_LAZY);
	if (!dylib)
		return false;

	block = reinterpret_cast<Func>(dlsym(dylib, "block_entry"));
	if (!dylib)
		return false;
	return true;
}

} // namespace JIT
