/**********************************************************************************
Common gfx plugin spec, version #1.3 maintained by zilmar (zilmar@emulation64.com)

All questions or suggestions should go through the mailing list.
http://www.egroups.com/group/Plugin64-Dev
***********************************************************************************

Notes:
------

Setting the approprate bits in the MI_INTR_REG and calling CheckInterrupts which 
are both passed to the DLL in InitiateGFX will generate an Interrupt from with in 
the plugin.

The Setting of the RSP flags and generating an SP interrupt  should not be done in
the plugin

**********************************************************************************/
#ifndef _GFX_H_INCLUDED__
#define _GFX_H_INCLUDED__

#include <stdint.h>
#include "m64p_plugin.h"

/***** Structures *****/
typedef struct {
    uint16_t Version;        /* Set to 0x0103 */
    uint16_t Type;           /* Set to PLUGIN_TYPE_GFX */
    char Name[100];      /* Name of the DLL */

    /* If DLL supports memory these memory options then set them to TRUE or FALSE
       if it does not support it */
    int NormalMemory;    /* a normal uint8_t array */ 
    int MemoryBswaped;  /* a normal uint8_t array where the memory has been pre
                              bswap on a dword (32 bits) boundry */
} PLUGIN_INFO;

extern GFX_INFO gfx_info;

#ifdef __cplusplus
extern "C" {
#endif

EXPORT m64p_error CALL PluginGetVersionVideo(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities);

#ifdef __cplusplus
}
#endif


#endif



