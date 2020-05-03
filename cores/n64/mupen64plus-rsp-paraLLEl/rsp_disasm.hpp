#ifndef RSP_DISASM_HPP_
#define RSP_DISASM_HPP_

#include <string>

namespace RSP
{
std::string disassemble(uint32_t pc, uint32_t instr);
const char *register_name(unsigned reg_index);
}

#endif