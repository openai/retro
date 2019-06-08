#ifndef _FCEU_VIDEO_H
#define _FCEU_VIDEO_H

int FCEU_InitVirtualVideo(void);
void FCEU_KillVirtualVideo(void);
int SaveSnapshot(void);
extern uint8 *XBuf;
void FCEU_DrawNumberRow(uint8 *XBuf, int *nstatus, int cur);

#endif
