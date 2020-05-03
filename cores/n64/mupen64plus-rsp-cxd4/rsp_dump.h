#ifndef RSP_DUMP_H__
#define RSP_DUMP_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void rsp_open_trace(const char *path);
void rsp_close_trace(void);

void rsp_dump_begin_trace(void);
void rsp_dump_end_trace(void);

void rsp_dump_block(const char *tag, const void *data, size_t size);

void rsp_dump_begin_read_dma(void);
void rsp_dump_poke_mem(unsigned base, const void *data, size_t size);
void rsp_dump_end_read_dma(void);

int rsp_dump_recording_trace(void);

#ifdef __cplusplus
}
#endif

#endif
