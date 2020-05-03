#ifndef STATE_HPP__
#define STATE_HPP__

#include "rsp_common.h"

#define DMEM_SIZE (4 * 1024)
#define IMEM_SIZE (4 * 1024)
#define DMEM_WORDS (DMEM_SIZE / 4)
#define IMEM_WORDS (IMEM_SIZE / 4)
#define CODE_BLOCK_SIZE (256)
#define CODE_BLOCK_WORDS (CODE_BLOCK_SIZE / 4)
#define CODE_BLOCK_SIZE_LOG2 (8)
#define CODE_BLOCKS (IMEM_SIZE / CODE_BLOCK_SIZE)

namespace RSP
{
enum RSPFlags
{
	RSP_VCO = 0,
	RSP_VCC = 1,
	RSP_VCE = 2
};

enum RSPAccumulator
{
	RSP_ACC_LO = 16,
	RSP_ACC_MD = 8,
	RSP_ACC_HI = 0
};

enum CP0Registers
{
	CP0_REGISTER_DMA_CACHE = 0,
	CP0_REGISTER_DMA_DRAM = 1,
	CP0_REGISTER_DMA_READ_LENGTH = 2,
	CP0_REGISTER_DMA_WRITE_LENGTH = 3,
	CP0_REGISTER_SP_STATUS = 4,
	CP0_REGISTER_DMA_FULL = 5,
	CP0_REGISTER_DMA_BUSY = 6,
	CP0_REGISTER_SP_RESERVED = 7,
	CP0_REGISTER_CMD_START = 8,
	CP0_REGISTER_CMD_END = 9,
	CP0_REGISTER_CMD_CURRENT = 10,
	CP0_REGISTER_CMD_STATUS = 11,
	CP0_REGISTER_CMD_CLOCK = 12,
	CP0_REGISTER_CMD_BUSY = 13,
	CP0_REGISTER_CMD_PIPE_BUSY = 14,
	CP0_REGISTER_CMD_TMEM_BUSY = 15,
};

// SP_STATUS read bits.
#define SP_STATUS_HALT 0x0001
#define SP_STATUS_BROKE 0x0002
#define SP_STATUS_DMA_BUSY 0x0004
#define SP_STATUS_DMA_FULL 0x0008
#define SP_STATUS_IO_FULL 0x0010
#define SP_STATUS_SSTEP 0x0020
#define SP_STATUS_INTR_BREAK 0x0040
#define SP_STATUS_SIG0 0x0080
#define SP_STATUS_SIG1 0x0100
#define SP_STATUS_SIG2 0x0200
#define SP_STATUS_SIG3 0x0400
#define SP_STATUS_SIG4 0x0800
#define SP_STATUS_SIG5 0x1000
#define SP_STATUS_SIG6 0x2000
#define SP_STATUS_SIG7 0x4000

// SP_STATUS write bits.
#define SP_CLR_HALT 0x00000001
#define SP_SET_HALT 0x00000002
#define SP_CLR_BROKE 0x00000004
#define SP_CLR_INTR 0x00000008
#define SP_SET_INTR 0x00000010
#define SP_CLR_SSTEP 0x00000020
#define SP_SET_SSTEP 0x00000040
#define SP_CLR_INTR_BREAK 0x00000080
#define SP_SET_INTR_BREAK 0x00000100
#define SP_CLR_SIG0 0x00000200
#define SP_SET_SIG0 0x00000400
#define SP_CLR_SIG1 0x00000800
#define SP_SET_SIG1 0x00001000
#define SP_CLR_SIG2 0x00002000
#define SP_SET_SIG2 0x00004000
#define SP_CLR_SIG3 0x00008000
#define SP_SET_SIG3 0x00010000
#define SP_CLR_SIG4 0x00020000
#define SP_SET_SIG4 0x00040000
#define SP_CLR_SIG5 0x00080000
#define SP_SET_SIG5 0x00100000
#define SP_CLR_SIG6 0x00200000
#define SP_SET_SIG6 0x00400000
#define SP_CLR_SIG7 0x00800000
#define SP_SET_SIG7 0x01000000

template <int N>
struct alignas(rsp_vect_t) AlignedRSPVector
{
	uint16_t e[8 * N];
};

struct CP0
{
	uint32_t *cr[16] = {};
	uint32_t *irq = nullptr;
};

struct alignas(64) CP2
{
	AlignedRSPVector<1> regs[32];
	AlignedRSPVector<2> flags[3];
	AlignedRSPVector<3> acc;
	int16_t div_out;
	int16_t div_in;
	int8_t dp_flag;
};

struct CPUState
{
	uint32_t pc = 0;
	uint32_t dirty_blocks = 0;
	static_assert(CODE_BLOCKS <= 32, "Code blocks must fit in 32-bit register.");

	uint32_t has_delay_slot = 0;
	uint32_t branch_target = 0;

	uint32_t sr[33] = {}; // r32 is used as a scratch register.
	uint32_t *dmem = nullptr;
	uint32_t *imem = nullptr;
	uint32_t *rdram = nullptr;

	CP2 cp2 = {};
	CP0 cp0;
};

enum ReturnMode
{
	MODE_ENTER = 0,
	MODE_CONTINUE = 1,
	MODE_BREAK = 2,
	MODE_DMA_READ = 3,
	MODE_CHECK_FLAGS = 4
};

} // namespace RSP

#endif
