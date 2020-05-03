#ifndef GLIDEN64_LIBRETRO_H
#define GLIDEN64_LIBRETRO_H

#ifdef GLES2
#define GL_RGBA8 GL_RGBA
#define NO_BLIT_BUFFER_COPY
#define GL_DRAW_FRAMEBUFFER GL_FRAMEBUFFER
#define GL_READ_FRAMEBUFFER GL_FRAMEBUFFER
#define GLESX
#ifdef PANDORA
typedef char GLchar;
#endif
#elif defined(GLES3)
#define GLESX
#define GL_MULTISAMPLING_SUPPORT
#define GL_UNIFORMBLOCK_SUPPORT
#elif defined(GLES3_1)
#define GLESX
#define GL_IMAGE_TEXTURES_SUPPORT
#define GL_MULTISAMPLING_SUPPORT
#define GL_UNIFORMBLOCK_SUPPORT
#elif defined(EGL)
#define GL_IMAGE_TEXTURES_SUPPORT
#define GL_MULTISAMPLING_SUPPORT
#define GL_UNIFORMBLOCK_SUPPORT
#else
#if defined(OS_MAC_OS_X)
#define GL_GLEXT_PROTOTYPES
#elif defined(OS_LINUX)
#define GL_GLEXT_PROTOTYPES
#define GL_IMAGE_TEXTURES_SUPPORT
#define GL_MULTISAMPLING_SUPPORT
#define GL_UNIFORMBLOCK_SUPPORT
#elif defined(OS_WINDOWS)
#define GL_IMAGE_TEXTURES_SUPPORT
#define GL_MULTISAMPLING_SUPPORT
#define GL_UNIFORMBLOCK_SUPPORT
#endif // OS_MAC_OS_X
#endif // GLES2

extern uint32_t bilinearMode;
extern uint32_t EnableHWLighting;
extern uint32_t CorrectTexrectCoords;
extern uint32_t enableNativeResTexrects;
extern uint32_t enableLegacyBlending;
extern uint32_t EnableCopyColorToRDRAM;
extern uint32_t EnableCopyDepthToRDRAM;
extern uint32_t AspectRatio;
extern uint32_t MaxTxCacheSize;
extern uint32_t txFilterMode;
extern uint32_t txEnhancementMode;
extern uint32_t txHiresEnable;
extern uint32_t txHiresFullAlphaChannel;
extern uint32_t txFilterIgnoreBG;
extern uint32_t EnableFXAA;
extern uint32_t MultiSampling;
extern uint32_t EnableFragmentDepthWrite;
extern uint32_t EnableShadersStorage;
extern uint32_t EnableTextureCache;
extern uint32_t EnableFBEmulation;
extern uint32_t EnableFrameDuping;
extern uint32_t EnableNoiseEmulation;
extern uint32_t EnableLODEmulation;
extern uint32_t EnableFullspeed;
extern uint32_t CountPerOp;
extern uint32_t CountPerScanlineOverride;
extern uint32_t BackgroundMode;
extern uint32_t EnableEnhancedTextureStorage;
extern uint32_t EnableEnhancedHighResStorage;
extern uint32_t EnableTxCacheCompression;
extern uint32_t ForceDisableExtraMem;
extern uint32_t EnableNativeResFactor;
extern uint32_t EnableN64DepthCompare;

// Overscan Options
extern uint32_t EnableOverscan;
extern uint32_t OverscanTop;
extern uint32_t OverscanLeft;
extern uint32_t OverscanRight;
extern uint32_t OverscanBottom;

// Others
#define RETRO_MEMORY_DD 0x100 + 1
#define RETRO_GAME_TYPE_DD  1

#endif
