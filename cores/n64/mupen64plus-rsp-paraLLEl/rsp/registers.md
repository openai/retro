//
// rsp/registers.md: RSP register enumerations.
//
// CEN64: Cycle-Accurate Nintendo 64 Emulator.
// Copyright (C) 2015, Tyler J. Stachecki.
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

#ifndef SP_REGISTER_LIST
#define SP_REGISTER_LIST \
  X(SP_MEM_ADDR_REG) \
  X(SP_DRAM_ADDR_REG) \
  X(SP_RD_LEN_REG) \
  X(SP_WR_LEN_REG) \
  X(SP_STATUS_REG) \
  X(SP_DMA_FULL_REG) \
  X(SP_DMA_BUSY_REG) \
  X(SP_SEMAPHORE_REG) \
  X(CMD_START) \
  X(CMD_END) \
  X(CMD_CURRENT) \
  X(CMD_STATUS) \
  X(CMD_CLOCK) \
  X(CMD_BUSY) \
  X(CMD_PIPE_BUSY) \
  X(CMD_TMEM_BUSY) \
  X(SP_PC_REG) \
  X(SP_IBIST_REG)
#endif

SP_REGISTER_LIST

