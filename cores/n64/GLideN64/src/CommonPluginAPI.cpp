#ifdef OS_WINDOWS
# include <windows.h>
#else
# include "winlnxdefs.h"
#endif // OS_WINDOWS

#include "PluginAPI.h"

extern "C" {

EXPORT BOOL CALL gln64InitiateGFX (GFX_INFO Gfx_Info)
{
	return api().InitiateGFX(Gfx_Info);
}

EXPORT void CALL gln64MoveScreen (int xpos, int ypos)
{
	api().MoveScreen(xpos, ypos);
}

EXPORT void CALL gln64ProcessDList(void)
{
	api().ProcessDList();
}

EXPORT void CALL gln64ProcessRDPList(void)
{
	api().ProcessRDPList();
}

EXPORT void CALL gln64RomClosed (void)
{
	api().RomClosed();
}

EXPORT void CALL gln64ShowCFB (void)
{
	api().ShowCFB();
}

EXPORT void CALL gln64UpdateScreen (void)
{
	api().UpdateScreen();
}

EXPORT void CALL gln64ViStatusChanged (void)
{
	api().ViStatusChanged();
}

EXPORT void CALL gln64ViWidthChanged (void)
{
	api().ViWidthChanged();
}

EXPORT void CALL gln64ChangeWindow(void)
{
	api().ChangeWindow();
}

EXPORT void CALL gln64FBWrite(unsigned int addr, unsigned int size)
{
	api().FBWrite(addr, size);
}

EXPORT void CALL gln64FBRead(unsigned int addr)
{
	api().FBRead(addr);
}

EXPORT void CALL gln64FBGetFrameBufferInfo(void *pinfo)
{
	api().FBGetFrameBufferInfo(pinfo);
}

#ifndef MUPENPLUSAPI
EXPORT void CALL gln64FBWList(FrameBufferModifyEntry *plist, unsigned int size)
{
	api().FBWList(plist, size);
}
#endif
}
