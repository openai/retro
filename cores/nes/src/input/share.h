#include "../fceu-types.h"
#include "../input.h"
#include "../fceu.h"
#include "../ppu.h"
#include "../x6502.h"
#include "../palette.h"
#include "../state.h"

void FCEUMOV_AddJoy(uint8 *js);
void FCEU_DrawCursor(uint8 *buf, int xc, int yc);
void FCEU_DrawGunSight(uint8 *buf, int xc, int yc);
