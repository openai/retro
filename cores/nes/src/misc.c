#include <stdio.h>

#include "fceu-types.h"
#include "fceu.h"

#include "driver.h"

/* wave.c */

void FCEU_WriteWaveData(int32 *Buffer, int Count) { }
int FCEUI_EndWaveRecord(void) { return 0; }
int FCEUI_BeginWaveRecord(char *fn) { return 0; }

/* netplay.c */
int FCEUNET_SendFile(uint8 cmd, char *fn) { return 0; }

/* movie.c */
void FCEUMOV_AddJoy(uint8 *js) { }
void FCEUMOV_Stop(void) {}
void FCEUI_SelectMovie(int w) { }
