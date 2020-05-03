#include "rsp_op.hpp"
#include "rsp_disasm.hpp"
#include <random>

using namespace RSP;

static uint64_t hash_registers(const CPUState *rsp)
{
	uint64_t h = 0xcbf29ce484222325ull;
	auto *data = reinterpret_cast<const uint32_t *>(&rsp->cp2);
	unsigned words = sizeof(rsp->cp2) >> 2;
	for (size_t i = 0; i < words; i++)
		h = (h * 0x100000001b3ull) ^ data[i];

	return h;
}

static void fill_cp2_register_state(std::mt19937 &rnd, CPUState *rsp)
{
	auto *data = reinterpret_cast<uint32_t *>(&rsp->cp2);
	unsigned words = sizeof(rsp->cp2) >> 2;
	for (unsigned i = 0; i < words; i++)
		data[i] = rnd();
}

static void run_vu_opcode(std::mt19937 &rnd, CPUState *rsp, unsigned instr)
{
	fill_cp2_register_state(rnd, rsp);

	uint32_t op = instr & 63;
	uint32_t vd = (instr >> 6) & 31;
	uint32_t vs = (instr >> 11) & 31;
	uint32_t vt = (instr >> 16) & 31;
	uint32_t e = (instr >> 21) & 15;

	using VUOp = void (*)(RSP::CPUState *, unsigned vd, unsigned vs, unsigned vt, unsigned e);
	static const VUOp ops[64] = {
		RSP_VMULF, RSP_VMULU, nullptr, nullptr, RSP_VMUDL, RSP_VMUDM, RSP_VMUDN, RSP_VMUDH, RSP_VMACF, RSP_VMACU, nullptr,
		nullptr, RSP_VMADL, RSP_VMADM, RSP_VMADN, RSP_VMADH, RSP_VADD, RSP_VSUB, nullptr, RSP_VABS, RSP_VADDC, RSP_VSUBC,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, RSP_VSAR, nullptr, nullptr, RSP_VLT,
		RSP_VEQ, RSP_VNE, RSP_VGE, RSP_VCL, RSP_VCH, RSP_VCR, RSP_VMRG, RSP_VAND, RSP_VNAND, RSP_VOR, RSP_VNOR,
		RSP_VXOR, RSP_VNXOR, nullptr, nullptr, RSP_VRCP, RSP_VRCPL, RSP_VRCPH, RSP_VMOV, RSP_VRSQ, RSP_VRSQL, RSP_VRSQH,
		RSP_VNOP,
	};

	auto *vuop = ops[op];
	if (!vuop)
		vuop = RSP_RESERVED;

	vuop(rsp, vd, vs, vt, e);

	uint64_t register_hash = hash_registers(rsp);
	auto disasm = disassemble(0, instr | (0x25u << 25u));
	printf("%s (%llx)\n", disasm.c_str(), static_cast<unsigned long long>(register_hash));
}

int main()
{
	// Exhaustive test every single VU opcode with random input.
	std::mt19937 rnd(0xf00b4); // Fixed seed.
	CPUState state = {};

	// Sweep through every opcode at least once.
	constexpr unsigned num_opcodes = 1u << 25u;
	for (unsigned instr = 0; instr < num_opcodes; instr++)
		run_vu_opcode(rnd, &state, instr);

	// Try random opcodes some more, should hopefully hit some interesting corner cases eventually.
	constexpr unsigned num_random_opcodes = 50000000;
	for (unsigned i = 0; i < num_random_opcodes; i++)
	{
		unsigned instr = rnd() & (num_opcodes - 1);
		run_vu_opcode(rnd, &state, instr);
	}
}
