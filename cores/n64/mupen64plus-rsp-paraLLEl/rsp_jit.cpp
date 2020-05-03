#include "rsp_jit.hpp"
#include "rsp_disasm.hpp"
#include <utility>
#include <assert.h>

using namespace std;

//#define TRACE
//#define TRACE_ENTER
//#define TRACE_DISASM

// We're only guaranteed 3 V registers (x86).
#define JIT_REGISTER_STATE JIT_V0
#define JIT_REGISTER_DMEM JIT_V1
#define JIT_REGISTER_INDIRECT_PC JIT_V2

#define JIT_REGISTER_MODE JIT_R1
#define JIT_REGISTER_NEXT_PC JIT_R0

#define JIT_FRAME_SIZE 256

#if __WORDSIZE == 32
#undef jit_ldxr_ui
#define jit_ldxr_ui jit_ldxr_i
#undef jit_ldxi_ui
#define jit_ldxi_ui jit_ldxi_i
#endif

namespace RSP
{
namespace JIT
{
CPU::CPU()
{
	cleanup_jit_states.reserve(16 * 1024);
	init_jit("RSP");
	init_jit_thunks();
}

CPU::~CPU()
{
	for (auto *_jit : cleanup_jit_states)
		jit_destroy_state();
	finish_jit();
}

void CPU::invalidate_imem()
{
	for (unsigned i = 0; i < CODE_BLOCKS; i++)
		if (memcmp(cached_imem + i * CODE_BLOCK_WORDS, state.imem + i * CODE_BLOCK_WORDS, CODE_BLOCK_SIZE))
			state.dirty_blocks |= (0x3 << i) >> 1;
}

void CPU::invalidate_code()
{
	if (!state.dirty_blocks)
		return;

	for (unsigned i = 0; i < CODE_BLOCKS; i++)
	{
		if (state.dirty_blocks & (1 << i))
		{
			memset(blocks + i * CODE_BLOCK_WORDS, 0, CODE_BLOCK_WORDS * sizeof(blocks[0]));
			memcpy(cached_imem + i * CODE_BLOCK_WORDS, state.imem + i * CODE_BLOCK_WORDS, CODE_BLOCK_SIZE);
		}
	}

	state.dirty_blocks = 0;
}

// Need super-fast hash here.
uint64_t CPU::hash_imem(unsigned pc, unsigned count) const
{
	size_t size = count;

	// FNV-1.
	const auto *data = state.imem + pc;
	uint64_t h = 0xcbf29ce484222325ull;
	h = (h * 0x100000001b3ull) ^ pc;
	h = (h * 0x100000001b3ull) ^ count;
	for (size_t i = 0; i < size; i++)
		h = (h * 0x100000001b3ull) ^ data[i];
	return h;
}

#ifdef TRACE
static uint64_t hash_registers(const CPUState *rsp)
{
	const auto *data = rsp->sr;
	uint64_t h = 0xcbf29ce484222325ull;
	for (size_t i = 1; i < 32; i++)
		h = (h * 0x100000001b3ull) ^ data[i];

	data = reinterpret_cast<const uint32_t *>(&rsp->cp2);
	unsigned words = sizeof(rsp->cp2) >> 2;
	for (size_t i = 0; i < words; i++)
		h = (h * 0x100000001b3ull) ^ data[i];

	return h;
}

static uint64_t hash_dmem(const CPUState *rsp)
{
	const auto *data = rsp->dmem;
	uint64_t h = 0xcbf29ce484222325ull;
	for (size_t i = 0; i < 1024; i++)
		h = (h * 0x100000001b3ull) ^ data[i];
	return h;
}
#endif

unsigned CPU::analyze_static_end(unsigned pc, unsigned end)
{
	// Scans through IMEM and finds the logical "end" of the instruction stream.
	// A logical end of the instruction stream is where execution must terminate.
	// If we have forward branches into this block, i.e. gotos, they extend the execution stream.
	// However, we cannot execute beyond end.
	unsigned max_static_pc = pc;
	unsigned count = end - pc;

	for (unsigned i = 0; i < count; i++)
	{
		uint32_t instr = state.imem[pc + i];
		uint32_t type = instr >> 26;
		uint32_t target;

		bool forward_goto;
		if (pc + i + 1 >= max_static_pc)
		{
			forward_goto = false;
			max_static_pc = pc + i + 1;
		}
		else
			forward_goto = true;

		// VU
		if ((instr >> 25) == 0x25)
			continue;

		switch (type)
		{
		case 000:
			switch (instr & 63)
			{
			case 010:
			case 011:
				// JR and JALR always terminate execution of the block.
				// We execute the next instruction via delay slot and exit.
				// Unless we can branch past the JR
				// (max_static_pc will be higher than expected),
				// this will be the static end.
				if (!forward_goto)
				{
					max_static_pc = max(pc + i + 2, max_static_pc);
					goto end;
				}
				break;

			case 015:
				// BREAK always terminates.
				if (!forward_goto)
					goto end;
				break;

			default:
				break;
			}
			break;

		case 001: // REGIMM
			switch ((instr >> 16) & 31)
			{
			case 000: // BLTZ
			case 001: // BGEZ
			case 021: // BGEZAL
			case 020: // BLTZAL
				// TODO/Optimization: Handle static branch case where $0 is used.
				target = (pc + i + 1 + instr) & 0x3ff;
				if (target >= pc && target < end) // goto
					max_static_pc = max(max_static_pc, target + 1);
				break;

			default:
				break;
			}
			break;

		case 002: // J
		case 003: // JAL
			// Where we choose to end the block here is critical for performance, since otherwise
			// we end up hashing a lot of garbage as it turns out ...

			// J is resolved by goto. Same with JAL if call target happens to be inside the block.
			target = instr & 0x3ff;
			if (target >= pc && target < end) // goto
			{
				// J is a static jump, so if we aren't branching
				// past this instruction and we're branching backwards,
				// we can end the block here.
				if (!forward_goto)
				{
					max_static_pc = max(pc + i + 2, max_static_pc);
					goto end;
				}
				else
					max_static_pc = max(max_static_pc, target + 1);
			}
			else if (!forward_goto)
			{
				// If we have static branch outside our block,
				// we terminate the block.
				max_static_pc = max(pc + i + 2, max_static_pc);
				goto end;
			}
			break;

		case 004: // BEQ
		case 005: // BNE
		case 006: // BLEZ
		case 007: // BGTZ
			// TODO/Optimization: Handle static branch case where $0 is used.
			target = (pc + i + 1 + instr) & 0x3ff;
			if (target >= pc && target < end) // goto
				max_static_pc = max(max_static_pc, target + 1);
			break;

		default:
			break;
		}
	}

end:
	unsigned ret = min(max_static_pc, end);
	return ret;
}

extern "C"
{
#define BYTE_ENDIAN_FIXUP(x, off) ((((x) + (off)) ^ 3) & 0xfffu)
	static Func rsp_enter(void *cpu, unsigned pc)
	{
		return static_cast<CPU *>(cpu)->get_jit_block(pc);
	}

	static jit_word_t rsp_unaligned_lh(const uint8_t *dram, jit_word_t addr)
	{
		auto off0 = BYTE_ENDIAN_FIXUP(addr, 0);
		auto off1 = BYTE_ENDIAN_FIXUP(addr, 1);
		return jit_word_t(int16_t((dram[off0] << 8) |
		                          (dram[off1] << 0)));
	}

	static jit_word_t rsp_unaligned_lw(const uint8_t *dram, jit_word_t addr)
	{
		auto off0 = BYTE_ENDIAN_FIXUP(addr, 0);
		auto off1 = BYTE_ENDIAN_FIXUP(addr, 1);
		auto off2 = BYTE_ENDIAN_FIXUP(addr, 2);
		auto off3 = BYTE_ENDIAN_FIXUP(addr, 3);

		// To sign extend, or not to sign extend, hm ...
		return jit_word_t((int32_t(dram[off0]) << 24) |
		                  (int32_t(dram[off1]) << 16) |
		                  (int32_t(dram[off2]) << 8) |
		                  (int32_t(dram[off3]) << 0));
	}

	static jit_uword_t rsp_unaligned_lhu(const uint8_t *dram, jit_word_t addr)
	{
		auto off0 = BYTE_ENDIAN_FIXUP(addr, 0);
		auto off1 = BYTE_ENDIAN_FIXUP(addr, 1);
		return jit_word_t(uint16_t((dram[off0] << 8) |
		                          (dram[off1] << 0)));
	}

	static void rsp_unaligned_sh(uint8_t *dram, jit_word_t addr, jit_word_t data)
	{
		auto off0 = BYTE_ENDIAN_FIXUP(addr, 0);
		auto off1 = BYTE_ENDIAN_FIXUP(addr, 1);
		dram[off0] = (data >> 8) & 0xff;
		dram[off1] = (data >> 0) & 0xff;
	}

	static void rsp_unaligned_sw(uint8_t *dram, jit_word_t addr, jit_word_t data)
	{
		auto off0 = BYTE_ENDIAN_FIXUP(addr, 0);
		auto off1 = BYTE_ENDIAN_FIXUP(addr, 1);
		auto off2 = BYTE_ENDIAN_FIXUP(addr, 2);
		auto off3 = BYTE_ENDIAN_FIXUP(addr, 3);

		dram[off0] = (data >> 24) & 0xff;
		dram[off1] = (data >> 16) & 0xff;
		dram[off2] = (data >> 8) & 0xff;
		dram[off3] = (data >> 0) & 0xff;
	}

#ifdef TRACE
	static void rsp_report_pc(const CPUState *state, jit_uword_t pc, jit_uword_t instr)
	{
		auto disasm = disassemble(pc, instr);
		disasm += " (" + std::to_string(hash_registers(state)) + ") (" + std::to_string(hash_dmem(state)) + ")";
		puts(disasm.c_str());
	}
#endif

#ifdef TRACE_ENTER
	static void rsp_report_enter(jit_uword_t pc)
	{
		printf("  ... Enter 0x%03x ...  ", unsigned(pc & 0xffcu));
	}
#endif
}

void CPU::jit_save_indirect_register(jit_state_t *_jit, unsigned mips_register)
{
	unsigned jit_reg = regs.load_mips_register_noext(_jit, mips_register);
	jit_movr(JIT_REGISTER_INDIRECT_PC, jit_reg);
	regs.unlock_mips_register(mips_register);
}

void CPU::jit_save_illegal_indirect_register(jit_state_t *_jit)
{
	jit_stxi(-JIT_FRAME_SIZE + 3 * sizeof(jit_word_t), JIT_FP, JIT_REGISTER_INDIRECT_PC);
}

void CPU::jit_load_indirect_register(jit_state_t *_jit, unsigned jit_reg)
{
	jit_movr(jit_reg, JIT_REGISTER_INDIRECT_PC);
}

void CPU::jit_load_illegal_indirect_register(jit_state_t *_jit, unsigned jit_reg)
{
	jit_ldxi(jit_reg, JIT_FP, -JIT_FRAME_SIZE + 3 * sizeof(jit_word_t));
}

void CPU::jit_begin_call(jit_state_t *_jit)
{
	// Workarounds weird Lightning behavior around register usage.
	// It has been observed that EBX (V0) is clobbered on x86 Linux when
	// calling out to C code.
	jit_live(JIT_REGISTER_STATE);
	jit_live(JIT_REGISTER_DMEM);
	jit_live(JIT_REGISTER_INDIRECT_PC);

	jit_prepare();
}

void CPU::jit_end_call(jit_state_t *_jit, jit_pointer_t ptr)
{
	jit_finishi(ptr);

	// Workarounds weird Lightning behavior around register usage.
	// It has been observed that EBX (V0) is clobbered on x86 Linux when
	// calling out to C code.
	jit_live(JIT_REGISTER_STATE);
	jit_live(JIT_REGISTER_DMEM);
	jit_live(JIT_REGISTER_INDIRECT_PC);
}

void CPU::jit_save_illegal_cond_branch_taken(jit_state_t *_jit)
{
	unsigned cond_reg = regs.load_mips_register_noext(_jit, RegisterCache::COND_BRANCH_TAKEN);
	jit_stxi(-JIT_FRAME_SIZE + sizeof(jit_word_t), JIT_FP, cond_reg);
	regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
}

void CPU::jit_restore_illegal_cond_branch_taken(jit_state_t *_jit, unsigned reg)
{
	jit_ldxi(reg, JIT_FP, -JIT_FRAME_SIZE + sizeof(jit_word_t));
}

void CPU::jit_clear_illegal_cond_branch_taken(jit_state_t *_jit, unsigned tmp_reg)
{
	jit_movi(tmp_reg, 0);
	jit_stxi(-JIT_FRAME_SIZE + sizeof(jit_word_t), JIT_FP, tmp_reg);
}

void CPU::init_jit_thunks()
{
	jit_state_t *_jit = jit_new_state();

	jit_prolog();

	// Saves registers from C++ code.
	jit_frame(JIT_FRAME_SIZE);
	auto *state = jit_arg();

	// These registers remain fixed and all called thunks will poke into these registers as necessary.
	jit_getarg(JIT_REGISTER_STATE, state);
	jit_ldxi_i(JIT_REGISTER_NEXT_PC, JIT_REGISTER_STATE, offsetof(CPUState, pc));
	jit_ldxi(JIT_REGISTER_DMEM, JIT_REGISTER_STATE, offsetof(CPUState, dmem));

	// When thunks need non-local goto, they jump here.
	auto *entry_label = jit_indirect();

#ifdef TRACE_ENTER
	{
		// Save PC.
		jit_stxi_i(offsetof(CPUState, pc), JIT_REGISTER_STATE, JIT_REGISTER_NEXT_PC);
		jit_prepare();
		jit_pushargr(JIT_REGISTER_NEXT_PC);
		jit_finishi(reinterpret_cast<jit_pointer_t>(rsp_report_enter));
		jit_ldxi_i(JIT_REGISTER_NEXT_PC, JIT_REGISTER_STATE, offsetof(CPUState, pc));
	}
#endif

	jit_prepare();
	jit_pushargr(JIT_REGISTER_STATE);
	jit_pushargr(JIT_REGISTER_NEXT_PC);
	jit_finishi(reinterpret_cast<jit_pointer_t>(rsp_enter));
	jit_retval(JIT_REGISTER_NEXT_PC);

	// Jump to thunk.

	// Clear out branch delay slots.
	jit_clear_illegal_cond_branch_taken(_jit, JIT_REGISTER_MODE);
	jit_stxi_i(offsetof(CPUState, sr) + RegisterCache::COND_BRANCH_TAKEN * 4, JIT_REGISTER_STATE, JIT_REGISTER_MODE);

	jit_jmpr(JIT_REGISTER_NEXT_PC);

	// When we want to return, JIT thunks will jump here.
	auto *return_label = jit_indirect();

	// Save PC.
	jit_stxi_i(offsetof(CPUState, pc), JIT_REGISTER_STATE, JIT_REGISTER_NEXT_PC);

	// Return status. This register is considered common for all thunks.
	jit_retr(JIT_REGISTER_MODE);

	thunks.enter_frame = reinterpret_cast<int (*)(void *)>(jit_emit());
	thunks.enter_thunk = jit_address(entry_label);
	thunks.return_thunk = jit_address(return_label);

	//printf(" === DISASM ===\n");
	//jit_disassemble();
	jit_clear_state();
	//printf(" === END DISASM ===\n");
	cleanup_jit_states.push_back(_jit);
}

Func CPU::get_jit_block(uint32_t pc)
{
	pc &= IMEM_SIZE - 1;
	uint32_t word_pc = pc >> 2;
	auto &block = blocks[word_pc];

	if (!block)
	{
		unsigned end = (pc + (CODE_BLOCK_SIZE * 2)) >> CODE_BLOCK_SIZE_LOG2;
		end <<= CODE_BLOCK_SIZE_LOG2 - 2;
		end = min(end, unsigned(IMEM_SIZE >> 2));
		end = analyze_static_end(word_pc, end);

		uint64_t hash = hash_imem(word_pc, end - word_pc);
		auto &ptr = cached_blocks[word_pc][hash];
		if (ptr)
			block = ptr;
		else
			block = ptr = jit_region(hash, word_pc, end - word_pc);
	}
	return block;
}

int CPU::enter(uint32_t pc)
{
	// Top level enter.
	state.pc = pc;
	static_assert(offsetof(CPU, state) == 0, "CPU state must lie on first byte.");
	int ret = thunks.enter_frame(this);
	return ret;
}

void CPU::jit_end_of_block(jit_state_t *_jit, uint32_t pc, const CPU::InstructionInfo &last_info)
{
	// If we run off the end of a block with a pending delay slot, we need to move it to CPUState.
	// We always branch to the next PC, and the delay slot will be handled after the first instruction in next block.

	unsigned cond_branch_reg = 0;
	if (last_info.branch && last_info.conditional)
	{
		cond_branch_reg = regs.load_mips_register_noext(_jit, RegisterCache::COND_BRANCH_TAKEN);
		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
	}
	unsigned scratch_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);
	regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
	regs.flush_register_window(_jit);

	jit_node_t *forward = nullptr;
	if (last_info.branch)
	{
		if (last_info.conditional)
			forward = jit_beqi(cond_branch_reg, 0);

		if (last_info.indirect)
			jit_load_indirect_register(_jit, scratch_reg);
		else
			jit_movi(scratch_reg, last_info.branch_target);
		jit_stxi_i(offsetof(CPUState, branch_target), JIT_REGISTER_STATE, scratch_reg);
		jit_movi(scratch_reg, 1);
		jit_stxi_i(offsetof(CPUState, has_delay_slot), JIT_REGISTER_STATE, scratch_reg);
	}

	if (forward)
		jit_patch(forward);
	jit_movi(JIT_REGISTER_NEXT_PC, pc);
	jit_patch_abs(jit_jmpi(), thunks.enter_thunk);
}

void CPU::jit_handle_impossible_delay_slot(jit_state_t *_jit, const InstructionInfo &info,
                                           const InstructionInfo &last_info, uint32_t base_pc,
                                           uint32_t end_pc)
{
	unsigned cond_branch_reg = regs.load_mips_register_noext(_jit, RegisterCache::COND_BRANCH_TAKEN);
	unsigned scratch_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);
	unsigned illegal_cond_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER1);

	regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
	regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
	regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER1);
	regs.flush_register_window(_jit);
	// We can still use the registers after flushing,
	// but we cannot call on the register cache any more until we resolve the branch.

	// A case here would be:
	// beq r0, r1, somewhere
	// beq r1, r2, somewhere
	// <-- we are here ...
	// add r0, r1, r2

	// This case should normally never happen, but you never know what happens on a fixed platform ...
	// Cond branch information for the first branch is found in JIT_FP[-JIT_FRAME_SIZE].
	// Cond branch information for the second branch is found in COND_BRANCH_TAKEN.

	// If the first branch was taken, we will transfer control, but we will never use a local goto here
	// since we potentially need to set the has_delay_slot argument.
	// If the first branch is not taken, we will defer any control transfer until the next instruction, nothing happens,
	// except that FP[0] is cleared.

	jit_node_t *nobranch = nullptr;
	if (last_info.conditional)
	{
		jit_restore_illegal_cond_branch_taken(_jit, illegal_cond_reg);
		jit_clear_illegal_cond_branch_taken(_jit, scratch_reg);
		nobranch = jit_beqi(illegal_cond_reg, 0);
	}
	else
		jit_clear_illegal_cond_branch_taken(_jit, cond_branch_reg);

	// ... But do we have a delay slot to take care of?
	if (!info.conditional)
		jit_movi(cond_branch_reg, 1);
	jit_stxi_i(offsetof(CPUState, has_delay_slot), JIT_REGISTER_STATE, cond_branch_reg);

	if (info.indirect)
		jit_load_indirect_register(_jit, cond_branch_reg);
	else
		jit_movi(cond_branch_reg, info.branch_target);
	jit_stxi_i(offsetof(CPUState, branch_target), JIT_REGISTER_STATE, cond_branch_reg);

	// We are done with register use.

	// Here we *will* take the branch.
	if (last_info.indirect)
		jit_load_illegal_indirect_register(_jit, JIT_REGISTER_NEXT_PC);
	else
		jit_movi(JIT_REGISTER_NEXT_PC, last_info.branch_target);

	jit_patch_abs(jit_jmpi(), thunks.enter_thunk);

	if (nobranch)
		jit_patch(nobranch);
}

void CPU::jit_handle_delay_slot(jit_state_t *_jit, const InstructionInfo &last_info,
                                uint32_t base_pc, uint32_t end_pc)
{
	unsigned scratch_cond_reg = 0;
	if (last_info.conditional)
	{
		regs.load_mips_register_noext(_jit, RegisterCache::COND_BRANCH_TAKEN);
		unsigned cond_branch_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);

		scratch_cond_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);

		// Clear out branch state.
		jit_movr(scratch_cond_reg, cond_branch_reg);
		jit_movi(cond_branch_reg, 0);

		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
		regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
	}
	else
	{
		unsigned cond_branch_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
		jit_movi(cond_branch_reg, 0);
		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
	}
	regs.flush_register_window(_jit);

	if (last_info.conditional)
	{
		if (!last_info.indirect && last_info.branch_target >= base_pc && last_info.branch_target < end_pc)
		{
			// Patch this up later.
			unsigned local_index = (last_info.branch_target - base_pc) >> 2;
			local_branches.push_back({ jit_bnei(scratch_cond_reg, 0), local_index });
		}
		else
		{
			auto *no_branch = jit_beqi(scratch_cond_reg, 0);
			if (last_info.indirect)
				jit_load_indirect_register(_jit, JIT_REGISTER_NEXT_PC);
			else
				jit_movi(JIT_REGISTER_NEXT_PC, last_info.branch_target);
			jit_patch_abs(jit_jmpi(), thunks.enter_thunk);
			jit_patch(no_branch);
		}
	}
	else
	{
		if (!last_info.indirect && last_info.branch_target >= base_pc && last_info.branch_target < end_pc)
		{
			// Patch this up later.
			unsigned local_index = (last_info.branch_target - base_pc) >> 2;
			local_branches.push_back({ jit_jmpi(), local_index });
		}
		else
		{
			if (last_info.indirect)
				jit_load_indirect_register(_jit, JIT_REGISTER_NEXT_PC);
			else
				jit_movi(JIT_REGISTER_NEXT_PC, last_info.branch_target);
			jit_patch_abs(jit_jmpi(), thunks.enter_thunk);
		}
	}
}

void CPU::jit_exit(jit_state_t *_jit, uint32_t pc, const InstructionInfo &last_info,
                   ReturnMode mode, bool first_instruction)
{
	regs.flush_register_window(_jit);
	jit_movi(JIT_REGISTER_MODE, mode);
	jit_exit_dynamic(_jit, pc, last_info, first_instruction);
}

void CPU::jit_exit_dynamic(jit_state_t *_jit, uint32_t pc, const InstructionInfo &last_info, bool first_instruction)
{
	// We must not touch REGISTER_MODE / TMP1 here, fortunately we don't need to.
	if (first_instruction)
	{
		// Need to consider that we need to move delay slot to PC.
		jit_ldxi_i(JIT_REGISTER_NEXT_PC, JIT_REGISTER_STATE, offsetof(CPUState, has_delay_slot));

		auto *latent_delay_slot = jit_bnei(JIT_REGISTER_NEXT_PC, 0);

		// Common case.
		// Immediately exit.
		jit_movi(JIT_REGISTER_NEXT_PC, (pc + 4) & 0xffcu);
		jit_patch_abs(jit_jmpi(), thunks.return_thunk);

		// If we had a latent delay slot, we handle it here.
		jit_patch(latent_delay_slot);

		// jit_exit is never called from a branch instruction, so we do not have to handle double branch delay slots here.
		jit_movi(JIT_REGISTER_NEXT_PC, 0);
		jit_stxi_i(offsetof(CPUState, has_delay_slot), JIT_REGISTER_STATE, JIT_REGISTER_NEXT_PC);
		jit_ldxi_i(JIT_REGISTER_NEXT_PC, JIT_REGISTER_STATE, offsetof(CPUState, branch_target));
	}
	else if (!last_info.branch)
	{
		// Immediately exit.
		jit_movi(JIT_REGISTER_NEXT_PC, (pc + 4) & 0xffcu);
	}
	else if (!last_info.indirect && !last_info.conditional)
	{
		// Redirect PC to whatever value we were supposed to branch to.
		jit_movi(JIT_REGISTER_NEXT_PC, last_info.branch_target);
	}
	else if (!last_info.conditional)
	{
		// We have an indirect branch, load that register into PC.
		jit_load_indirect_register(_jit, JIT_REGISTER_NEXT_PC);
	}
	else if (last_info.indirect)
	{
		// Indirect conditional branch.
		jit_ldxi_i(JIT_REGISTER_NEXT_PC, JIT_REGISTER_STATE,
		           offsetof(CPUState, sr) + RegisterCache::COND_BRANCH_TAKEN * 4);
		auto *node = jit_beqi(JIT_REGISTER_NEXT_PC, 0);
		jit_load_indirect_register(_jit, JIT_REGISTER_NEXT_PC);
		auto *to_end = jit_jmpi();
		jit_patch(node);
		jit_movi(JIT_REGISTER_NEXT_PC, (pc + 4) & 0xffcu);
		jit_patch(to_end);
	}
	else
	{
		// Direct conditional branch.
		jit_ldxi_i(JIT_REGISTER_NEXT_PC, JIT_REGISTER_STATE,
		           offsetof(CPUState, sr) + RegisterCache::COND_BRANCH_TAKEN * 4);
		auto *node = jit_beqi(JIT_REGISTER_NEXT_PC, 0);
		jit_movi(JIT_REGISTER_NEXT_PC, last_info.branch_target);
		auto *to_end = jit_jmpi();
		jit_patch(node);
		jit_movi(JIT_REGISTER_NEXT_PC, (pc + 4) & 0xffcu);
		jit_patch(to_end);
	}

	jit_patch_abs(jit_jmpi(), thunks.return_thunk);
}

void CPU::jit_emit_store_operation(jit_state_t *_jit,
                                   uint32_t pc, uint32_t instr,
                                   void (*jit_emitter)(jit_state_t *jit, unsigned, unsigned, unsigned), const char *asmop,
                                   jit_pointer_t rsp_unaligned_op,
                                   uint32_t endian_flip,
                                   const InstructionInfo &last_info)
{
	uint32_t align_mask = 3 - endian_flip;
	unsigned rt = (instr >> 16) & 31;
	int16_t simm = int16_t(instr);
	unsigned rs = (instr >> 21) & 31;
	unsigned rt_reg = regs.load_mips_register_noext(_jit, rt);
	unsigned rs_reg = regs.load_mips_register_noext(_jit, rs);
	unsigned rs_tmp_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);
	jit_addi(rs_tmp_reg, rs_reg, simm);
	jit_andi(rs_tmp_reg, rs_tmp_reg, 0xfffu);

	// If we are unaligned, it gets very messy to JIT, so just thunk it out to C code.
	jit_node_t *unaligned = nullptr;
	if (align_mask)
	{
		regs.unlock_mips_register(rt);
		regs.unlock_mips_register(rs);
		regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
		// We're going to call, so need to save caller-save register we care about.
		regs.flush_caller_save_registers(_jit);

		unaligned = jit_bmsi(rs_tmp_reg, align_mask);
	}

	// The MIPS is big endian, but the words are swapped per word in integration, so it's kinda little-endian,
	// except we need to XOR the address for byte and half-word accesses.
	if (endian_flip != 0)
		jit_xori(rs_tmp_reg, rs_tmp_reg, endian_flip);

	jit_emitter(_jit, rs_tmp_reg, JIT_REGISTER_DMEM, rt_reg);

	jit_node_t *aligned = nullptr;
	if (align_mask)
	{
		aligned = jit_jmpi();
		jit_patch(unaligned);
		jit_begin_call(_jit);
		jit_pushargr(JIT_REGISTER_DMEM);
		jit_pushargr(rs_tmp_reg);
		jit_pushargr(rt_reg);
		jit_end_call(_jit, rsp_unaligned_op);
		jit_patch(aligned);
	}
	else
	{
		regs.unlock_mips_register(rt);
		regs.unlock_mips_register(rs);
		regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
	}
}

// The RSP may or may not have a load-delay slot, but it doesn't seem to matter in practice, so just emulate without
// a load-delay slot.

void CPU::jit_emit_load_operation(jit_state_t *_jit,
                                  uint32_t pc, uint32_t instr,
                                  void (*jit_emitter)(jit_state_t *jit, unsigned, unsigned, unsigned), const char *asmop,
                                  jit_pointer_t rsp_unaligned_op,
                                  uint32_t endian_flip,
                                  const InstructionInfo &last_info)
{
	uint32_t align_mask = endian_flip ^ 3;
	unsigned rt = (instr >> 16) & 31;
	if (rt == 0)
		return;

	int16_t simm = int16_t(instr);
	unsigned rs = (instr >> 21) & 31;
	unsigned rs_reg = regs.load_mips_register_noext(_jit, rs);
	unsigned rs_tmp_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);
	jit_addi(rs_tmp_reg, rs_reg, simm);
	jit_andi(rs_tmp_reg, rs_tmp_reg, 0xfffu);

	unsigned ret_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER1);

	// If we are unaligned, it gets very messy to JIT, so just thunk it out to C code.
	jit_node_t *unaligned = nullptr;
	if (align_mask)
	{
		// Flush the register cache here since we might call.
		// We will still use rs_reg/rt_reg, but they only live for this short burst only.
		regs.unlock_mips_register(rs);
		regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
		regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER1);

		regs.flush_caller_save_registers(_jit);
		unaligned = jit_bmsi(rs_tmp_reg, align_mask);
	}

	// The MIPS is big endian, but the words are swapped per word in integration, so it's kinda little-endian,
	// except we need to XOR the address for byte and half-word accesses.
	if (endian_flip != 0)
		jit_xori(rs_tmp_reg, rs_tmp_reg, endian_flip);

	jit_emitter(_jit, ret_reg, JIT_REGISTER_DMEM, rs_tmp_reg);

	jit_node_t *aligned = nullptr;
	if (align_mask)
	{
		aligned = jit_jmpi();
		jit_patch(unaligned);
	}

	if (align_mask)
	{
		// We're going to call, so need to save caller-save register we care about.
		jit_begin_call(_jit);
		jit_pushargr(JIT_REGISTER_DMEM);
		jit_pushargr(rs_tmp_reg);
		jit_end_call(_jit, rsp_unaligned_op);
		jit_retval(ret_reg);
		jit_patch(aligned);
	}
	else
	{
		regs.unlock_mips_register(rs);
		regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
		regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER1);
	}

	unsigned rt_reg = regs.modify_mips_register(_jit, rt);
	jit_movr(rt_reg, ret_reg);
	regs.unlock_mips_register(rt);
}

void CPU::jit_instruction(jit_state_t *_jit, uint32_t pc, uint32_t instr,
                          InstructionInfo &info, const InstructionInfo &last_info,
                          bool first_instruction, bool next_instruction_is_branch_target)
{
#ifdef TRACE
	regs.flush_register_window(_jit);
	jit_begin_call(_jit);
	jit_pushargr(JIT_REGISTER_STATE);
	jit_pushargi(pc);
	jit_pushargi(instr);
	jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(rsp_report_pc));
#endif

	// VU
	if ((instr >> 25) == 0x25)
	{
		// VU instruction. COP2, and high bit of opcode is set.
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

		regs.flush_caller_save_registers(_jit);
		jit_begin_call(_jit);
		jit_pushargr(JIT_REGISTER_STATE);
		jit_pushargi(vd);
		jit_pushargi(vs);
		jit_pushargi(vt);
		jit_pushargi(e);
		jit_end_call(_jit ,reinterpret_cast<jit_pointer_t>(vuop));
		return;
	}

	// TODO: Meaningful register allocation.
	// For now, always flush register state to memory after an instruction for simplicity.
	// Should be red-hot in L1 cache, so probably won't be that bad.
	// On x86 and x64, we unfortunately have an anemic register bank to work with in Lightning.

	uint32_t type = instr >> 26;

#define NOP_IF_RD_ZERO() if (rd == 0) { break; }
#define NOP_IF_RT_ZERO() if (rt == 0) { break; }

	switch (type)
	{
	case 000:
	{
		auto rd = (instr >> 11) & 31;
		auto rt = (instr >> 16) & 31;
		auto shift = (instr >> 6) & 31;
		auto rs = (instr >> 21) & 31;

		switch (instr & 63)
		{
		case 000: // SLL
		{
			NOP_IF_RD_ZERO();
			unsigned rt_reg = regs.load_mips_register_noext(_jit, rt);
			unsigned rd_reg = regs.modify_mips_register(_jit, rd);
			jit_lshi(rd_reg, rt_reg, shift);
			regs.unlock_mips_register(rt);
			regs.unlock_mips_register(rd);
			break;
		}

		case 002: // SRL
		{
			NOP_IF_RD_ZERO();
			unsigned rt_reg = regs.load_mips_register_zext(_jit, rt);
			unsigned rd_reg = regs.modify_mips_register(_jit, rd);
			jit_rshi_u(rd_reg, rt_reg, shift);
			regs.unlock_mips_register(rt);
			regs.unlock_mips_register(rd);
			break;
		}

		case 003: // SRA
		{
			NOP_IF_RD_ZERO();
			unsigned rt_reg = regs.load_mips_register_sext(_jit, rt);
			unsigned rd_reg = regs.modify_mips_register(_jit, rd);
			jit_rshi(rd_reg, rt_reg, shift);
			regs.unlock_mips_register(rt);
			regs.unlock_mips_register(rd);
			break;
		}

		case 004: // SLLV
		{
			NOP_IF_RD_ZERO();
			unsigned rt_reg = regs.load_mips_register_noext(_jit, rt);
			unsigned rs_reg = regs.load_mips_register_noext(_jit, rs);
			unsigned rs_tmp_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);
			jit_andi(rs_tmp_reg, rs_reg, 31);
			regs.unlock_mips_register(rs);
			unsigned rd_reg = regs.modify_mips_register(_jit, rd);
			jit_lshr(rd_reg, rt_reg, rs_tmp_reg);
			regs.unlock_mips_register(rt);
			regs.unlock_mips_register(rd);
			regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
			break;
		}

		case 006: // SRLV
		{
			NOP_IF_RD_ZERO();
			unsigned rt_reg = regs.load_mips_register_zext(_jit, rt);
			unsigned rs_reg = regs.load_mips_register_noext(_jit, rs);
			unsigned rs_tmp_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);
			jit_andi(rs_tmp_reg, rs_reg, 31);
			regs.unlock_mips_register(rs);
			unsigned rd_reg = regs.modify_mips_register(_jit, rd);
			jit_rshr_u(rd_reg, rt_reg, rs_tmp_reg);
			regs.unlock_mips_register(rt);
			regs.unlock_mips_register(rd);
			regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
			break;
		}

		case 007: // SRAV
		{
			unsigned rt_reg = regs.load_mips_register_sext(_jit, rt);
			unsigned rs_reg = regs.load_mips_register_noext(_jit, rs);
			unsigned rs_tmp_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);
			jit_andi(rs_tmp_reg, rs_reg, 31);
			regs.unlock_mips_register(rs);
			unsigned rd_reg = regs.modify_mips_register(_jit, rd);
			jit_rshr(rd_reg, rt_reg, rs_tmp_reg);
			regs.unlock_mips_register(rt);
			regs.unlock_mips_register(rd);
			regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
			break;
		}

		// If the last instruction is also a branch instruction, we will need to do some funky handling
		// so make sure we save the old branch taken register.
#define FLUSH_IMPOSSIBLE_DELAY_SLOT() do { \
	if (last_info.branch && last_info.conditional) \
		jit_save_illegal_cond_branch_taken(_jit); \
	if (last_info.branch && last_info.indirect) \
		jit_save_illegal_indirect_register(_jit); \
	} while(0)

		case 010: // JR
		{
			FLUSH_IMPOSSIBLE_DELAY_SLOT();
			info.branch = true;
			info.indirect = true;
			jit_save_indirect_register(_jit, rs);

			// If someone can branch to the delay slot, we have to turn this into a conditional branch.
			if (next_instruction_is_branch_target)
			{
				info.conditional = true;
				regs.immediate_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN, 1);
			}
			else
				regs.immediate_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN, 0);

			regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
			break;
		}

		case 011: // JALR
		{
			FLUSH_IMPOSSIBLE_DELAY_SLOT();
			jit_save_indirect_register(_jit, rs);
			if (rd != 0)
			{
				regs.immediate_mips_register(_jit, rd, (pc + 8) & 0xffcu);
				regs.unlock_mips_register(rd);
			}

			info.branch = true;
			info.indirect = true;
			// If someone can branch to the delay slot, we have to turn this into a conditional branch.
			if (next_instruction_is_branch_target)
			{
				info.conditional = true;
				regs.immediate_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN, 1);
			}
			else
				regs.immediate_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN, 0);

			regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
			break;
		}

		case 015: // BREAK
		{
			jit_exit(_jit, pc, last_info, MODE_BREAK, first_instruction);
			info.handles_delay_slot = true;
			break;
		}

#define THREE_REG_OP(op, ext) \
	NOP_IF_RD_ZERO(); \
	unsigned rs_reg = regs.load_mips_register_##ext(_jit, rs); \
	unsigned rt_reg = regs.load_mips_register_##ext(_jit, rt); \
	unsigned rd_reg = regs.modify_mips_register(_jit, rd); \
	jit_##op(rd_reg, rs_reg, rt_reg); \
	regs.unlock_mips_register(rs); \
	regs.unlock_mips_register(rt); \
	regs.unlock_mips_register(rd)

		case 040: // ADD
		case 041: // ADDU
		{
			THREE_REG_OP(addr, noext);
			break;
		}

		case 042: // SUB
		case 043: // SUBU
		{
			THREE_REG_OP(subr, noext);
			break;
		}

		case 044: // AND
		{
			THREE_REG_OP(andr, noext);
			break;
		}

		case 045: // OR
		{
			THREE_REG_OP(orr, noext);
			break;
		}

		case 046: // XOR
		{
			THREE_REG_OP(xorr, noext);
			break;
		}

		case 047: // NOR
		{
			NOP_IF_RD_ZERO();
			unsigned rt_reg = regs.load_mips_register_noext(_jit, rt);
			unsigned rs_reg = regs.load_mips_register_noext(_jit, rs);
			unsigned rd_reg = regs.modify_mips_register(_jit, rd);
			jit_orr(rd_reg, rt_reg, rs_reg);
			jit_xori(rd_reg, rd_reg, jit_word_t(-1));
			regs.unlock_mips_register(rt);
			regs.unlock_mips_register(rs);
			regs.unlock_mips_register(rd);
			break;
		}

		case 052: // SLT
		{
			THREE_REG_OP(ltr, sext);
			break;
		}

		case 053: // SLTU
		{
			THREE_REG_OP(ltr_u, zext);
			break;
		}

		default:
			break;
		}
		break;
	}

	case 001: // REGIMM
	{
		unsigned rt = (instr >> 16) & 31;

		switch (rt)
		{
		case 020: // BLTZAL
		{
			FLUSH_IMPOSSIBLE_DELAY_SLOT();
			unsigned rs = (instr >> 21) & 31;
			uint32_t target_pc = (pc + 4 + (instr << 2)) & 0xffc;
			unsigned rs_reg = regs.load_mips_register_sext(_jit, rs);
			unsigned cond_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
			jit_lti(cond_reg, rs_reg, 0);

			regs.unlock_mips_register(rs);
			regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);

			// Link register is written after condition.
			regs.immediate_mips_register(_jit, 31, (pc + 8) & 0xffcu);
			regs.unlock_mips_register(31);

			info.branch = true;
			info.conditional = true;
			info.branch_target = target_pc;
			break;
		}

		case 000: // BLTZ
		{
			FLUSH_IMPOSSIBLE_DELAY_SLOT();
			unsigned rs = (instr >> 21) & 31;
			uint32_t target_pc = (pc + 4 + (instr << 2)) & 0xffc;

			unsigned rs_reg = regs.load_mips_register_sext(_jit, rs);
			unsigned cond_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
			jit_lti(cond_reg, rs_reg, 0);

			regs.unlock_mips_register(rs);
			regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);

			info.branch = true;
			info.conditional = true;
			info.branch_target = target_pc;
			break;
		}

		case 021: // BGEZAL
		{
			FLUSH_IMPOSSIBLE_DELAY_SLOT();
			unsigned rs = (instr >> 21) & 31;
			uint32_t target_pc = (pc + 4 + (instr << 2)) & 0xffc;
			unsigned rs_reg = regs.load_mips_register_sext(_jit, rs);
			unsigned cond_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
			jit_gei(cond_reg, rs_reg, 0);

			regs.unlock_mips_register(rs);
			regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);

			// Link register is written after condition.
			regs.immediate_mips_register(_jit, 31, (pc + 8) & 0xffcu);
			regs.unlock_mips_register(31);

			info.branch = true;
			info.conditional = true;
			info.branch_target = target_pc;
			break;
		}

		case 001: // BGEZ
		{
			FLUSH_IMPOSSIBLE_DELAY_SLOT();
			unsigned rs = (instr >> 21) & 31;
			uint32_t target_pc = (pc + 4 + (instr << 2)) & 0xffc;
			unsigned rs_reg = regs.load_mips_register_sext(_jit, rs);
			unsigned cond_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
			jit_gei(cond_reg, rs_reg, 0);

			regs.unlock_mips_register(rs);
			regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);

			info.branch = true;
			info.conditional = true;
			info.branch_target = target_pc;
			break;
		}

		default:
			break;
		}
		break;
	}

	case 003: // JAL
	{
		FLUSH_IMPOSSIBLE_DELAY_SLOT();
		uint32_t target_pc = (instr & 0x3ffu) << 2;
		regs.immediate_mips_register(_jit, 31, (pc + 8) & 0xffcu);

		info.branch = true;
		info.branch_target = target_pc;
		if (next_instruction_is_branch_target)
		{
			info.conditional = true;
			regs.immediate_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN, 1);
		}
		else
			regs.immediate_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN, 0);

		regs.unlock_mips_register(31);
		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
		break;
	}

	case 002: // J
	{
		FLUSH_IMPOSSIBLE_DELAY_SLOT();
		uint32_t target_pc = (instr & 0x3ffu) << 2;

		info.branch = true;
		info.branch_target = target_pc;
		if (next_instruction_is_branch_target)
		{
			info.conditional = true;
			regs.immediate_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN, 1);
		}
		else
			regs.immediate_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN, 0);

		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
		break;
	}

	case 004: // BEQ
	{
		FLUSH_IMPOSSIBLE_DELAY_SLOT();
		unsigned rs = (instr >> 21) & 31;
		unsigned rt = (instr >> 16) & 31;
		uint32_t target_pc = (pc + 4 + (instr << 2)) & 0xffc;
		unsigned rs_reg = regs.load_mips_register_sext(_jit, rs);
		unsigned rt_reg = regs.load_mips_register_sext(_jit, rt);
		unsigned cond_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
		jit_eqr(cond_reg, rs_reg, rt_reg);
		regs.unlock_mips_register(rs);
		regs.unlock_mips_register(rt);
		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
		info.branch = true;
		info.conditional = true;
		info.branch_target = target_pc;
		break;
	}

	case 005: // BNE
	{
		FLUSH_IMPOSSIBLE_DELAY_SLOT();
		unsigned rs = (instr >> 21) & 31;
		unsigned rt = (instr >> 16) & 31;
		uint32_t target_pc = (pc + 4 + (instr << 2)) & 0xffc;
		unsigned rs_reg = regs.load_mips_register_sext(_jit, rs);
		unsigned rt_reg = regs.load_mips_register_sext(_jit, rt);
		unsigned cond_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
		jit_ner(cond_reg, rs_reg, rt_reg);
		regs.unlock_mips_register(rs);
		regs.unlock_mips_register(rt);
		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
		info.branch = true;
		info.conditional = true;
		info.branch_target = target_pc;
		break;
	}

	case 006: // BLEZ
	{
		FLUSH_IMPOSSIBLE_DELAY_SLOT();
		unsigned rs = (instr >> 21) & 31;
		uint32_t target_pc = (pc + 4 + (instr << 2)) & 0xffc;

		// If using $0, it's an unconditional branch.
		if (rs != 0)
		{
			unsigned rs_reg = regs.load_mips_register_sext(_jit, rs);
			unsigned cond_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
			jit_lei(cond_reg, rs_reg, 0);
			regs.unlock_mips_register(rs);
			regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
			info.conditional = true;
		}

		info.branch = true;
		info.branch_target = target_pc;
		break;
	}

	case 007: // BGTZ
	{
		FLUSH_IMPOSSIBLE_DELAY_SLOT();
		unsigned rs = (instr >> 21) & 31;

		// Meaningless
		if (rs == 0)
			break;

		uint32_t target_pc = (pc + 4 + (instr << 2)) & 0xffc;
		unsigned rs_reg = regs.load_mips_register_sext(_jit, rs);
		unsigned cond_reg = regs.modify_mips_register(_jit, RegisterCache::COND_BRANCH_TAKEN);
		jit_gti(cond_reg, rs_reg, 0);
		regs.unlock_mips_register(rs);
		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);

		info.branch = true;
		info.conditional = true;
		info.branch_target = target_pc;
		break;
	}

#define TWO_REG_RS_IS_ZERO() (((instr >> 21) & 31) == 0)

#define TWO_REG_IMM_OP(op, immtype, ext) \
	unsigned rt = (instr >> 16) & 31; \
	NOP_IF_RT_ZERO(); \
	unsigned rs = (instr >> 21) & 31; \
	unsigned rs_reg = regs.load_mips_register_##ext(_jit, rs); \
	unsigned rt_reg = regs.modify_mips_register(_jit, rt); \
	jit_##op(rt_reg, rs_reg, immtype(instr)); \
	regs.unlock_mips_register(rs); \
	regs.unlock_mips_register(rt)

	case 010: // ADDI
	case 011:
	{
		if (TWO_REG_RS_IS_ZERO())
		{
			unsigned rt = (instr >> 16) & 31;
			NOP_IF_RT_ZERO();
			regs.immediate_mips_register(_jit, rt, int16_t(instr));
			regs.unlock_mips_register(rt);
		}
		else
		{
			TWO_REG_IMM_OP(addi, int16_t, noext);
		}
		break;
	}

	case 012: // SLTI
	{
		TWO_REG_IMM_OP(lti, int16_t, sext);
		break;
	}

	case 013: // SLTIU
	{
		TWO_REG_IMM_OP(lti_u, uint16_t, zext);
		break;
	}

	case 014: // ANDI
	{
		TWO_REG_IMM_OP(andi, uint16_t, noext);
		break;
	}

	case 015: // ORI
	{
		if (TWO_REG_RS_IS_ZERO())
		{
			unsigned rt = (instr >> 16) & 31;
			NOP_IF_RT_ZERO();
			regs.immediate_mips_register(_jit, rt, uint16_t(instr));
			regs.unlock_mips_register(rt);
		}
		else
		{
			TWO_REG_IMM_OP(ori, uint16_t, noext);
		}
		break;
	}

	case 016: // XORI
	{
		TWO_REG_IMM_OP(xori, uint16_t, noext);
		break;
	}

	case 017: // LUI
	{
		unsigned rt = (instr >> 16) & 31;
		NOP_IF_RT_ZERO();
		int16_t imm = int16_t(instr);
		regs.immediate_mips_register(_jit, rt, imm << 16);
		regs.unlock_mips_register(rt);
		break;
	}

	case 020: // COP0
	{
		unsigned rd = (instr >> 11) & 31;
		unsigned rs = (instr >> 21) & 31;
		unsigned rt = (instr >> 16) & 31;

		switch (rs)
		{
		case 000: // MFC0
		{
			regs.flush_register_window(_jit);

			jit_begin_call(_jit);
			jit_pushargr(JIT_REGISTER_STATE);
			jit_pushargi(rt);
			jit_pushargi(rd);
			jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(RSP_MFC0));
			jit_retval(JIT_REGISTER_MODE);

			jit_node_t *noexit = jit_beqi(JIT_REGISTER_MODE, MODE_CONTINUE);
			jit_exit_dynamic(_jit, pc, last_info, first_instruction);
			jit_patch(noexit);

			break;
		}

		case 004: // MTC0
		{
			regs.flush_register_window(_jit);

			jit_begin_call(_jit);
			jit_pushargr(JIT_REGISTER_STATE);
			jit_pushargi(rd);
			jit_pushargi(rt);
			jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(RSP_MTC0));
			jit_retval(JIT_REGISTER_MODE);

			jit_node_t *noexit = jit_beqi(JIT_REGISTER_MODE, MODE_CONTINUE);
			jit_exit_dynamic(_jit, pc, last_info, first_instruction);
			jit_patch(noexit);

			break;
		}

		default:
			break;
		}
		break;
	}

	case 022: // COP2
	{
		unsigned rd = (instr >> 11) & 31;
		unsigned rs = (instr >> 21) & 31;
		unsigned rt = (instr >> 16) & 31;
		unsigned imm = (instr >> 7) & 15;

		switch (rs)
		{
		case 000: // MFC2
		{
			regs.flush_caller_save_registers(_jit);
			regs.flush_mips_register(_jit, rt);
			jit_begin_call(_jit);
			jit_pushargr(JIT_REGISTER_STATE);
			jit_pushargi(rt);
			jit_pushargi(rd);
			jit_pushargi(imm);
			jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(RSP_MFC2));
			break;
		}

		case 002: // CFC2
		{
			regs.flush_caller_save_registers(_jit);
			regs.flush_mips_register(_jit, rt);
			jit_begin_call(_jit);
			jit_pushargr(JIT_REGISTER_STATE);
			jit_pushargi(rt);
			jit_pushargi(rd);
			jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(RSP_CFC2));
			break;
		}

		case 004: // MTC2
		{
			regs.flush_caller_save_registers(_jit);
			regs.flush_mips_register(_jit, rt);
			jit_begin_call(_jit);
			jit_pushargr(JIT_REGISTER_STATE);
			jit_pushargi(rt);
			jit_pushargi(rd);
			jit_pushargi(imm);
			jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(RSP_MTC2));
			break;
		}

		case 006: // CTC2
		{
			regs.flush_caller_save_registers(_jit);
			regs.flush_mips_register(_jit, rt);

			jit_begin_call(_jit);
			jit_pushargr(JIT_REGISTER_STATE);
			jit_pushargi(rt);
			jit_pushargi(rd);
			jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(RSP_CTC2));
			break;
		}

		default:
			break;
		}
		break;
	}

	case 040: // LB
	{
		jit_emit_load_operation(_jit, pc, instr,
		                        [](jit_state_t *_jit, unsigned a, unsigned b, unsigned c) { jit_ldxr_c(a, b, c); },
		                        "lb",
		                        nullptr,
		                        3, last_info);
		break;
	}

	case 041: // LH
	{
		jit_emit_load_operation(_jit, pc, instr,
		                        [](jit_state_t *_jit, unsigned a, unsigned b, unsigned c) { jit_ldxr_s(a, b, c); },
		                        "lh",
		                        reinterpret_cast<jit_pointer_t>(rsp_unaligned_lh),
		                        2, last_info);
		break;
	}

	case 043: // LW
	{
		jit_emit_load_operation(_jit, pc, instr,
		                        [](jit_state_t *_jit, unsigned a, unsigned b, unsigned c) { jit_ldxr_i(a, b, c); },
		                        "lw",
		                        reinterpret_cast<jit_pointer_t>(rsp_unaligned_lw),
		                        0, last_info);
		break;
	}

	case 044: // LBU
	{
		jit_emit_load_operation(_jit, pc, instr,
		                        [](jit_state_t *_jit, unsigned a, unsigned b, unsigned c) { jit_ldxr_uc(a, b, c); },
		                        "lbu",
		                        nullptr,
		                        3, last_info);
		break;
	}

	case 045: // LHU
	{
		jit_emit_load_operation(_jit, pc, instr,
		                        [](jit_state_t *_jit, unsigned a, unsigned b, unsigned c) { jit_ldxr_us(a, b, c); },
		                        "lhu",
		                        reinterpret_cast<jit_pointer_t>(rsp_unaligned_lhu),
		                        2, last_info);
		break;
	}

	case 050: // SB
	{
		jit_emit_store_operation(_jit, pc, instr,
		                         [](jit_state_t *_jit, unsigned a, unsigned b, unsigned c) { jit_stxr_c(a, b, c); },
		                         "sb",
		                         nullptr,
		                         3, last_info);
		break;
	}

	case 051: // SH
	{
		jit_emit_store_operation(_jit, pc, instr,
		                         [](jit_state_t *_jit, unsigned a, unsigned b, unsigned c) { jit_stxr_s(a, b, c); },
		                         "sh",
		                         reinterpret_cast<jit_pointer_t>(rsp_unaligned_sh),
		                         2, last_info);
		break;
	}

	case 053: // SW
	{
		jit_emit_store_operation(_jit, pc, instr,
		                         [](jit_state_t *_jit, unsigned a, unsigned b, unsigned c) { jit_stxr_i(a, b, c); },
		                         "sh",
		                         reinterpret_cast<jit_pointer_t>(rsp_unaligned_sw),
		                         0, last_info);
		break;
	}

	case 062: // LWC2
	{
		unsigned rt = (instr >> 16) & 31;
		int16_t simm = instr;
		// Sign-extend.
		simm <<= 9;
		simm >>= 9;
		unsigned rs = (instr >> 21) & 31;
		unsigned rd = (instr >> 11) & 31;
		unsigned imm = (instr >> 7) & 15;

		using LWC2Op = void (*)(RSP::CPUState *, unsigned rt, unsigned imm, int simm, unsigned rs);
		static const LWC2Op ops[32] = {
			RSP_LBV, RSP_LSV, RSP_LLV, RSP_LDV, RSP_LQV, RSP_LRV, RSP_LPV, RSP_LUV, RSP_LHV, nullptr, nullptr, RSP_LTV,
		};

		auto *op = ops[rd];
		if (op)
		{
			regs.flush_caller_save_registers(_jit);
			regs.flush_mips_register(_jit, rs);
			jit_begin_call(_jit);
			jit_pushargr(JIT_REGISTER_STATE);
			jit_pushargi(rt);
			jit_pushargi(imm);
			jit_pushargi(simm);
			jit_pushargi(rs);
			jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(op));
		}
		break;
	}

	case 072: // SWC2
	{
		unsigned rt = (instr >> 16) & 31;
		int16_t simm = instr;
		// Sign-extend.
		simm <<= 9;
		simm >>= 9;
		unsigned rs = (instr >> 21) & 31;
		unsigned rd = (instr >> 11) & 31;
		unsigned imm = (instr >> 7) & 15;

		using SWC2Op = void (*)(RSP::CPUState *, unsigned rt, unsigned imm, int simm, unsigned rs);
		static const SWC2Op ops[32] = {
			RSP_SBV, RSP_SSV, RSP_SLV, RSP_SDV, RSP_SQV, RSP_SRV, RSP_SPV, RSP_SUV, RSP_SHV, RSP_SFV, nullptr, RSP_STV,
		};

		auto *op = ops[rd];
		if (op)
		{
			regs.flush_caller_save_registers(_jit);
			regs.flush_mips_register(_jit, rs);
			jit_begin_call(_jit);
			jit_pushargr(JIT_REGISTER_STATE);
			jit_pushargi(rt);
			jit_pushargi(imm);
			jit_pushargi(simm);
			jit_pushargi(rs);
			jit_end_call(_jit, reinterpret_cast<jit_pointer_t>(op));
		}
		break;
	}

	default:
		break;
	}
}

void CPU::jit_mark_block_entries(uint32_t pc, uint32_t end, bool *block_entries)
{
	unsigned count = end - pc;

	// Find all places where we need to insert a label.
	// This also affects codegen for static branches.
	// If the delay slot for a static branch is a block entry,
	// it is not actually a static branch, but a conditional one because
	// some other instruction might have branches into the delay slot.
	for (unsigned i = 0; i < count; i++)
	{
		uint32_t instr = state.imem[pc + i];
		uint32_t type = instr >> 26;
		uint32_t target;

		// VU
		if ((instr >> 25) == 0x25)
			continue;

		switch (type)
		{
		case 001: // REGIMM
			switch ((instr >> 16) & 31)
			{
			case 000: // BLTZ
			case 001: // BGEZ
			case 021: // BGEZAL
			case 020: // BLTZAL
				target = (pc + i + 1 + instr) & 0x3ff;
				if (target >= pc && target < end) // goto
					block_entries[target - pc] = true;
				break;

			default:
				break;
			}
			break;

		case 002:
		case 003:
			// J is resolved by goto. Same with JAL.
			target = instr & 0x3ff;
			if (target >= pc && target < end) // goto
				block_entries[target - pc] = true;
			break;

		case 004: // BEQ
		case 005: // BNE
		case 006: // BLEZ
		case 007: // BGTZ
			target = (pc + i + 1 + instr) & 0x3ff;
			if (target >= pc && target < end) // goto
				block_entries[target - pc] = true;
			break;

		default:
			break;
		}
	}
}

void CPU::jit_handle_latent_delay_slot(jit_state_t *_jit, const InstructionInfo &last_info)
{
	unsigned cond_branch_reg = JIT_REGISTER_NEXT_PC;
	if (last_info.branch && last_info.conditional)
	{
		cond_branch_reg = regs.load_mips_register_noext(_jit, RegisterCache::COND_BRANCH_TAKEN);
		regs.unlock_mips_register(RegisterCache::COND_BRANCH_TAKEN);
	}
	regs.flush_register_window(_jit);

	if (last_info.branch)
	{
		// Well then ... two branches in a row just happened. Try to do something sensible.
		if (!last_info.conditional)
			jit_movi(cond_branch_reg, 1);
		jit_stxi_i(offsetof(CPUState, has_delay_slot), JIT_REGISTER_STATE, cond_branch_reg);

		jit_ldxi_i(JIT_REGISTER_NEXT_PC, JIT_REGISTER_STATE, offsetof(CPUState, branch_target));

		if (last_info.indirect)
			jit_load_indirect_register(_jit, JIT_REGISTER_MODE);
		else
			jit_movi(JIT_REGISTER_MODE, last_info.branch_target);

		jit_stxi_i(offsetof(CPUState, branch_target), JIT_REGISTER_STATE, JIT_REGISTER_MODE);
		jit_patch_abs(jit_jmpi(), thunks.enter_thunk);
	}
	else
	{
		jit_movi(JIT_REGISTER_NEXT_PC, 0);
		jit_stxi_i(offsetof(CPUState, has_delay_slot), JIT_REGISTER_STATE, JIT_REGISTER_NEXT_PC);
		jit_ldxi_i(JIT_REGISTER_NEXT_PC, JIT_REGISTER_STATE, offsetof(CPUState, branch_target));
		jit_patch_abs(jit_jmpi(), thunks.enter_thunk);
	}
}

Func CPU::jit_region(uint64_t hash, unsigned pc_word, unsigned instruction_count)
{
	regs.reset();

	mips_disasm.clear();
	jit_state_t *_jit = jit_new_state();

	jit_prolog();
	jit_tramp(JIT_FRAME_SIZE);

	jit_node_t *branch_targets[CODE_BLOCK_WORDS * 2];
	jit_node_t *latent_delay_slot = nullptr;
	local_branches.clear();

	assert(instruction_count <= (CODE_BLOCK_WORDS * 2));

	// Mark which instructions can be branched to via local goto.
	bool block_entry[CODE_BLOCK_WORDS * 2];
	memset(block_entry, 0, instruction_count * sizeof(bool));
	jit_mark_block_entries(pc_word, pc_word + instruction_count, block_entry);

	InstructionInfo last_info = {};
	InstructionInfo first_info = {};

	for (unsigned i = 0; i < instruction_count; i++)
	{
		if (block_entry[i])
		{
			// Before we enter into a new block, we have to flush register window since someone can branch here.
			regs.flush_register_window(_jit);
			regs.reset();
			branch_targets[i] = jit_label();
		}

		uint32_t instr = state.imem[pc_word + i];

#ifdef TRACE_DISASM
		mips_disasm += disassemble((pc_word + i) << 2, instr);
		if (last_info.branch)
		{
			mips_disasm += "  [branch]";
			if (last_info.conditional)
				mips_disasm += "  [cond]";
			if (last_info.indirect)
				mips_disasm += "  [indirect]";
			if (last_info.handles_delay_slot)
				mips_disasm += "  [handles delay slot]";
		}
		if (block_entry[i])
			mips_disasm += "  [block entry]";
		mips_disasm += "\n";
#endif

		InstructionInfo inst_info = {};
		jit_instruction(_jit, (pc_word + i) << 2, instr, inst_info, last_info, i == 0,
		                (i + 1 < instruction_count) && block_entry[i + 1]);

		// Handle all the fun cases with branch delay slots.
		// Not sure if we really need to handle them, but IIRC CXD4 does it and the LLVM RSP as well.

		if (i == 0 && !inst_info.handles_delay_slot)
		{
			unsigned scratch_reg = regs.modify_mips_register(_jit, RegisterCache::SCRATCH_REGISTER0);
			jit_ldxi_i(scratch_reg, JIT_REGISTER_STATE, offsetof(CPUState, has_delay_slot));
			regs.unlock_mips_register(RegisterCache::SCRATCH_REGISTER0);
			regs.flush_register_window(_jit);

			// After the first instruction, we might need to resolve a latent delay slot.
			latent_delay_slot = jit_bnei(scratch_reg, 0);
			first_info = inst_info;
		}
		else if (inst_info.branch && last_info.branch)
		{
			// "Impossible" handling of the delay slot.
			// Happens if we have two branch instructions in a row.
			// Weird magic happens here!
			jit_handle_impossible_delay_slot(_jit, inst_info, last_info, pc_word << 2, (pc_word + instruction_count) << 2);
		}
		else if (!inst_info.handles_delay_slot && last_info.branch)
		{
			// Normal handling of the delay slot.
			jit_handle_delay_slot(_jit, last_info, pc_word << 2, (pc_word + instruction_count) << 2);
		}
		last_info = inst_info;
	}

	regs.flush_register_window(_jit);

	// Jump to another block.
	jit_end_of_block(_jit, (pc_word + instruction_count) << 2, last_info);

	// If we had a latent delay slot, we handle it here.
	if (latent_delay_slot)
	{
		jit_patch(latent_delay_slot);
		jit_handle_latent_delay_slot(_jit, first_info);
	}

	for (auto &b : local_branches)
		jit_patch_at(b.node, branch_targets[b.local_index]);

	auto ret = reinterpret_cast<Func>(jit_emit());

#ifdef TRACE_DISASM
	printf(" === DISASM ===\n");
	printf("%s\n", mips_disasm.c_str());
	jit_disassemble();
	printf(" === DISASM END ===\n\n");
#endif
	jit_clear_state();
	cleanup_jit_states.push_back(_jit);
	return ret;
}

ReturnMode CPU::run()
{
	invalidate_code();
	for (;;)
	{
		int ret = enter(state.pc);
		switch (ret)
		{
		case MODE_BREAK:
			*state.cp0.cr[CP0_REGISTER_SP_STATUS] |= SP_STATUS_BROKE | SP_STATUS_HALT;
			if (*state.cp0.cr[CP0_REGISTER_SP_STATUS] & SP_STATUS_INTR_BREAK)
				*state.cp0.irq |= 1;
#ifndef PARALLEL_INTEGRATION
			print_registers();
#endif
			return MODE_BREAK;

		case MODE_CHECK_FLAGS:
		case MODE_DMA_READ:
			return static_cast<ReturnMode>(ret);

		default:
			break;
		}
	}
}

void CPU::print_registers()
{
#define DUMP_FILE stdout
	fprintf(DUMP_FILE, "RSP state:\n");
	fprintf(DUMP_FILE, "  PC: 0x%03x\n", state.pc);
	for (unsigned i = 1; i < 32; i++)
		fprintf(DUMP_FILE, "  SR[%s] = 0x%08x\n", register_name(i), state.sr[i]);
	fprintf(DUMP_FILE, "\n");
	for (unsigned i = 0; i < 32; i++)
	{
		fprintf(DUMP_FILE, "  VR[%02u] = { 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x }\n", i,
		        state.cp2.regs[i].e[0], state.cp2.regs[i].e[1], state.cp2.regs[i].e[2], state.cp2.regs[i].e[3],
		        state.cp2.regs[i].e[4], state.cp2.regs[i].e[5], state.cp2.regs[i].e[6], state.cp2.regs[i].e[7]);
	}

	fprintf(DUMP_FILE, "\n");

	for (unsigned i = 0; i < 3; i++)
	{
		static const char *strings[] = { "ACC_HI", "ACC_MD", "ACC_LO" };
		fprintf(DUMP_FILE, "  %s = { 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x }\n", strings[i],
		        state.cp2.acc.e[8 * i + 0], state.cp2.acc.e[8 * i + 1], state.cp2.acc.e[8 * i + 2],
		        state.cp2.acc.e[8 * i + 3], state.cp2.acc.e[8 * i + 4], state.cp2.acc.e[8 * i + 5],
		        state.cp2.acc.e[8 * i + 6], state.cp2.acc.e[8 * i + 7]);
	}

	fprintf(DUMP_FILE, "\n");

	for (unsigned i = 0; i < 3; i++)
	{
		static const char *strings[] = { "VCO", "VCC", "VCE" };
		uint16_t flags = rsp_get_flags(state.cp2.flags[i].e);
		fprintf(DUMP_FILE, "  %s = 0x%04x\n", strings[i], flags);
	}

	fprintf(DUMP_FILE, "\n");
	fprintf(DUMP_FILE, "  Div Out = 0x%04x\n", state.cp2.div_out);
	fprintf(DUMP_FILE, "  Div In  = 0x%04x\n", state.cp2.div_in);
	fprintf(DUMP_FILE, "  DP flag = 0x%04x\n", state.cp2.dp_flag);
}

RegisterCache::CacheEntry *RegisterCache::find_live_mips_register(unsigned mips_reg)
{
	for (auto &entry : entries)
		if (entry.is_live && entry.mips_register == mips_reg)
			return &entry;
	return nullptr;
}

RegisterCache::CacheEntry *RegisterCache::find_free_register()
{
	for (auto &entry : entries)
		if (!entry.is_live)
			return &entry;
	return nullptr;
}

RegisterCache::CacheEntry *RegisterCache::find_oldest_unlocked_register()
{
	CacheEntry *best = nullptr;
	for (auto &entry : entries)
	{
		if (entry.is_live && !entry.num_locks)
		{
			if (!best || entry.timestamp < best->timestamp)
				best = &entry;
		}
	}
	return best;
}

RegisterCache::CacheEntry &RegisterCache::find_register(unsigned mips_reg)
{
	auto *reg = find_live_mips_register(mips_reg);
	if (!reg)
		reg = find_free_register();
	if (!reg)
		reg = find_oldest_unlocked_register();
	assert(reg);
	return *reg;
}

void RegisterCache::writeback_register(jit_state_t *_jit, CacheEntry &entry)
{
	// The scratch registers are never flushed out to memory.
	assert(entry.mips_register != 0);
	if (entry.mips_register <= COND_BRANCH_TAKEN)
		jit_stxi_i(offsetof(CPUState, sr) + 4 * entry.mips_register, JIT_REGISTER_STATE, entry_to_jit_register(entry));
	entry.modified = false;
}

unsigned RegisterCache::immediate_mips_register(jit_state_t *_jit, unsigned mips_reg, jit_word_t value)
{
	unsigned jit_reg = modify_mips_register(_jit, mips_reg);
	jit_movi(jit_reg, value);
	entries[jit_register_to_index(jit_reg)].sign = SExt;
	return jit_reg;
}

unsigned RegisterCache::load_mips_register_noext(jit_state_t *_jit, unsigned mips_reg)
{
	auto &reg = find_register(mips_reg);
	unsigned jit_reg = entry_to_jit_register(reg);
	assert(mips_reg <= COND_BRANCH_TAKEN);

	if (reg.is_live && reg.mips_register != mips_reg)
	{
		if (reg.modified)
			writeback_register(_jit, reg);
		reg.mips_register = mips_reg;

		if (mips_reg)
			jit_ldxi_i(jit_reg, JIT_REGISTER_STATE, offsetof(CPUState, sr) + 4 * mips_reg);
		else
			jit_movi(jit_reg, 0);
		reg.modified = false;

		// We know that the input is sign-extended so future opcodes which rely on
		// sign-ness will be able to assume so.
		reg.sign = SExt;
	}
	else if (!reg.is_live)
	{
		reg.mips_register = mips_reg;

		if (mips_reg)
			jit_ldxi_i(jit_reg, JIT_REGISTER_STATE, offsetof(CPUState, sr) + 4 * mips_reg);
		else
			jit_movi(jit_reg, 0);

		reg.sign = SExt;
		reg.is_live = true;
		reg.modified = false;
	}

	// If the register is already live and well, we just need to update the timestamp.

	reg.timestamp = ++timestamp;
	reg.num_locks++;
	return jit_reg;
}

unsigned RegisterCache::modify_mips_register(jit_state_t *_jit, unsigned mips_reg)
{
	auto &reg = find_register(mips_reg);
	unsigned jit_reg = entry_to_jit_register(reg);

	if (reg.is_live && reg.mips_register != mips_reg)
	{
		if (reg.modified)
			writeback_register(_jit, reg);
		reg.mips_register = mips_reg;
	}
	else if (!reg.is_live)
	{
		reg.mips_register = mips_reg;
		reg.is_live = true;
	}

	// If the register is already live and well, we just need to update the timestamp.

	reg.sign = Unknown;
	reg.timestamp = ++timestamp;
	reg.num_locks++;
	reg.modified = true;
	return jit_reg;
}

unsigned RegisterCache::load_mips_register_sext(jit_state_t *_jit, unsigned mips_reg)
{
	auto &reg = find_register(mips_reg);
	unsigned jit_reg = entry_to_jit_register(reg);
	assert(mips_reg <= COND_BRANCH_TAKEN);

	if (reg.is_live && reg.mips_register != mips_reg)
	{
		if (reg.modified)
			writeback_register(_jit, reg);
		reg.mips_register = mips_reg;

		if (mips_reg)
			jit_ldxi_i(jit_reg, JIT_REGISTER_STATE, offsetof(CPUState, sr) + 4 * mips_reg);
		else
			jit_movi(jit_reg, 0);

		reg.modified = false;
		reg.sign = SExt;
	}
	else if (!reg.is_live)
	{
		reg.mips_register = mips_reg;

		if (mips_reg)
			jit_ldxi_i(jit_reg, JIT_REGISTER_STATE, offsetof(CPUState, sr) + 4 * mips_reg);
		else
			jit_movi(jit_reg, 0);

		reg.sign = SExt;
		reg.is_live = true;
		reg.modified = false;
	}
	else if (reg.sign != SExt)
	{
#if __WORDSIZE > 32
		if (mips_reg)
		{
			// Have to sign-extend if we're not sure.
			jit_extr_i(jit_reg, jit_reg);
		}
#endif
		reg.sign = SExt;
	}

	reg.num_locks++;
	reg.timestamp = ++timestamp;
	return jit_reg;
}

unsigned RegisterCache::load_mips_register_zext(jit_state_t *_jit, unsigned mips_reg)
{
	auto &reg = find_register(mips_reg);
	unsigned jit_reg = entry_to_jit_register(reg);
	assert(mips_reg <= COND_BRANCH_TAKEN);

	if (reg.is_live && reg.mips_register != mips_reg)
	{
		if (reg.modified)
			writeback_register(_jit, reg);
		reg.mips_register = mips_reg;

		if (mips_reg)
			jit_ldxi_ui(jit_reg, JIT_REGISTER_STATE, offsetof(CPUState, sr) + 4 * mips_reg);
		else
			jit_movi(jit_reg, 0);

		reg.modified = false;
		reg.sign = ZExt;
	}
	else if (!reg.is_live)
	{
		reg.mips_register = mips_reg;

		if (mips_reg)
			jit_ldxi_ui(jit_reg, JIT_REGISTER_STATE, offsetof(CPUState, sr) + 4 * mips_reg);
		else
			jit_movi(jit_reg, 0);

		reg.sign = ZExt;
		reg.is_live = true;
		reg.modified = false;
	}
	else if (reg.sign != ZExt)
	{
#if __WORDSIZE > 32
		if (mips_reg)
		{
			// Have to zero-extend if we're not sure.
			jit_extr_ui(jit_reg, jit_reg);
		}
#endif
		reg.sign = ZExt;
	}

	reg.num_locks++;
	reg.timestamp = ++timestamp;
	return jit_reg;
}

void RegisterCache::unlock_mips_register(unsigned mips_reg)
{
	auto *live_reg = find_live_mips_register(mips_reg);
	assert(live_reg);
	assert(live_reg->num_locks > 0);
	live_reg->num_locks--;
}

void RegisterCache::flush_register_window(jit_state_t *_jit)
{
	for (auto &entry : entries)
	{
		if (entry.is_live)
		{
			if (entry.modified)
				writeback_register(_jit, entry);
			assert(!entry.num_locks);
			entry = {};
		}
	}
	timestamp = 0;
}

void RegisterCache::flush_caller_save_registers(jit_state_t *_jit)
{
	for (unsigned i = 0; i < JIT_R_NUM; i++)
	{
		auto &entry = entries[jit_register_to_index(JIT_R(i))];
		if (entry.is_live)
		{
			if (entry.modified)
				writeback_register(_jit, entry);
			assert(!entry.num_locks);
			entry = {};
		}
	}
}

void RegisterCache::reset()
{
	for (auto &entry : entries)
		entry = {};
}

void RegisterCache::flush_mips_register(jit_state_t *_jit, unsigned mips_reg)
{
	auto *live_reg = find_live_mips_register(mips_reg);
	if (live_reg)
	{
		if (live_reg->modified)
			writeback_register(_jit, *live_reg);
		assert(!live_reg->num_locks);
		live_reg->is_live = false;
		*live_reg = {};
	}
}

unsigned RegisterCache::jit_register_to_index(unsigned jit_reg)
{
	if (jit_reg >= JIT_R0 && jit_reg < JIT_R(JIT_R_NUM))
		return jit_reg - JIT_R0;
	else
		return JIT_R_NUM + (jit_reg - JIT_V(3));
}

unsigned RegisterCache::entry_to_jit_register(const CacheEntry &entry)
{
	auto index = unsigned(&entry - entries);
	if (index < JIT_R_NUM)
		return JIT_R(index);
	else
		return JIT_V(3 + (index - JIT_R_NUM));
}

} // namespace JIT
} // namespace RSP
