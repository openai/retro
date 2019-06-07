#ifndef _FCEU_FDS_H
#define _FCEU_FDS_H

void FDSSoundReset(void);

void FCEU_FDSInsert(int oride);
void FCEU_FDSEject(void);
void FCEU_FDSSelect(void);

extern uint32 lastDiskPtrRead, lastDiskPtrWrite;

#endif
