#include "rsp_dump.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

static FILE *file;
static bool in_trace;

void rsp_open_trace(const char *path)
{
   file = fopen(path, "wb");
   fwrite("RSPDUMP1", 1, 8, file);
}

void rsp_close_trace(void)
{
   if (file)
   {
      fwrite("EOF     ", 1, 8, file);
      fclose(file);
   }

   file = nullptr;
}

void rsp_dump_begin_trace(void)
{
   if (!file)
      return;

   fwrite("BEGIN   ", 1, 8, file);
   in_trace = true;
}

int rsp_dump_recording_trace(void)
{
   return in_trace;
}

void rsp_dump_end_trace(void)
{
   if (!file)
      return;

   fwrite("END     ", 1, 8, file);
   in_trace = false;
}

void rsp_dump_block(const char *tag, const void *data, size_t size)
{
   if (!file)
      return;

   uint32_t size_data = size;

   assert(strlen(tag) == 8);
   fwrite(tag, 1, strlen(tag), file);
   fwrite(&size_data, sizeof(size_data), 1, file);
   fwrite(data, size, 1, file);
}

void rsp_dump_begin_read_dma(void)
{
   if (!file)
      return;

   fwrite("BEGINDMA", 1, 8, file);
}

void rsp_dump_poke_mem(unsigned base, const void *data, size_t size)
{
   if (!file)
      return;
   uint32_t size_data = size;
   uint32_t base_data = base;

   fwrite("POKE    ", 1, 8, file);
   fwrite(&base_data, sizeof(base_data), 1, file);
   fwrite(&size_data, sizeof(size_data), 1, file);
   fwrite(data, size, 1, file);
}

void rsp_dump_end_read_dma(void)
{
   if (!file)
      return;

   fwrite("ENDDMA  ", 1, 8, file);
}
