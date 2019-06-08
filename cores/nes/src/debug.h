#ifndef _FCEU_DEBUG_H
#define _FCEU_DEBUG_H

void FCEUI_DumpMem(const char *fname, uint32 start, uint32 end);
void FCEUI_DumpVid(const char *fname, uint32 start, uint32 end);
void FCEUI_LoadMem(const char *fname, uint32 start, int hl);

#ifdef FCEUDEF_DEBUGGER

/* Type attributes, you can OR them together. */
#define BPOINT_READ     1
#define BPOINT_WRITE    2
#define BPOINT_PC       4
#define BPOINT_EXCLUDE  8

#include "x6502struct.h"

void FCEUI_SetCPUCallback(void (*callb)(X6502 *X));
int FCEUI_DeleteBreakPoint(uint32 w);
int FCEUI_ListBreakPoints(int (*callb)(int type, unsigned int A1, unsigned int A2,
									   void (*Handler)(X6502 *, int type, unsigned int A)));
int FCEUI_GetBreakPoint(uint32 w, int *type, unsigned int *A1, unsigned int *A2,
						void(**Handler) (X6502 *, int type, unsigned int A));
int FCEUI_SetBreakPoint(uint32 w, int type, unsigned int A1, unsigned int A2,
						void (*Handler)(X6502 *, int type, unsigned int A));
int FCEUI_AddBreakPoint(int type, unsigned int A1, unsigned int A2,
						void (*Handler)(X6502 *, int type, unsigned int A));
#endif

#endif
