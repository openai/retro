#ifndef _FCEU_PPUVIEW_H
#define _FCEU_PPUVIEW_H

extern int PPUViewScanline;
extern int PPUViewer;
extern int scanline;

void PPUViewDoBlit();
void DoPPUView();
void UpdatePPUView(int refreshchr);


#endif
