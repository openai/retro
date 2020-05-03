#ifndef JIT_HPP
#define JIT_HPP

#include <memory>
#include <string>
#include <unordered_map>

namespace JIT
{
using Func = void (*)(void *, void *);
class LLVMEngine
{
public:
	LLVMEngine(const std::unordered_map<std::string, uint64_t> &symbol_table);
	~LLVMEngine();

private:
	friend class Block;
	struct Impl;
	std::unique_ptr<Impl> impl;
};

class Block
{
public:
	Block(LLVMEngine &engine);
	~Block();
	bool compile(uint64_t hash, const std::string &source);
	Func get_func() const
	{
		return block;
	}

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
	Func block = nullptr;
	size_t block_size = 0;
};
} // namespace JIT

#endif
