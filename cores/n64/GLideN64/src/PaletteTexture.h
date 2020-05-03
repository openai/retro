#pragma once
#include <memory>

#ifdef IOS
#include <stdlib.h>
#else
#include <malloc.h>
#endif // IOS

struct CachedTexture;

class PaletteTexture
{
public:
	PaletteTexture();

	void init();
	void destroy();
	void update();

private:
	CachedTexture * m_pTexture;
	u8* m_pbuf;
	u32 m_paletteCRC256;
};

extern PaletteTexture g_paletteTexture;
