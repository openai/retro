#ifndef DEBUG_JIT_HPP__
#define DEBUG_JIT_HPP__

#include <memory>
#include <stdint.h>
#include <string>
#include <unordered_map>

namespace JIT
{
using Func = void (*)(void *, void *);
class DebugBlock
{
public:
	DebugBlock(const std::unordered_map<std::string, uint64_t> &symbol_table);
	~DebugBlock();

	bool compile(uint64_t hash, const std::string &source);
	Func get_func() const
	{
		return block;
	}

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
	Func block = nullptr;
};
} // namespace JIT

#endif
