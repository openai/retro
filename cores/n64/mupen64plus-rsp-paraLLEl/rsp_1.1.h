#ifndef __RSP_1_1_H__
#define __RSP_1_1_H__

#if defined(__cplusplus)
extern "C"
{
#endif

#define PLUGIN_TYPE_RSP 1
#define PLUGIN_TYPE_GFX 2
#define PLUGIN_TYPE_AUDIO 3
#define PLUGIN_TYPE_CONTROLLER 4

#if !defined(M64P_PLUGIN_API)
	/*
 * slight changes to zilmar's spec file for portability
 *
 * The raw plugin spec headers by zilmar required WIN32 definitions.
 *
 * Here, the sufficient ANSI approximations are given so that this header
 * will operate more independently.
 */
	struct HWND__
	{
		int unused;
	};
	typedef struct HWND__ *HWND;
	struct HINSTANCE__
	{
		int unused;
	};
	typedef struct HINSTANCE__ *HINSTANCE;
	struct HMENU__
	{
		int unused;
	};
	typedef struct HMENU__ *HMENU;
	struct HDC__
	{
		int unused;
	};
	typedef struct HDC__ *HDC;
#endif

#if defined(_STDINT_H) || defined(M64P_PLUGIN_API)
	typedef uint32_t RCPREG;
#elif (0)
typedef unsigned long RCPREG; /* necessary for 16-bit targets */
#else
typedef unsigned int RCPREG; /* ANSI approximation of 32-bit size */
#endif

	typedef struct
	{
		unsigned short Version; /* Should be set to 0x0101 */
		unsigned short Type; /* Set to PLUGIN_TYPE_RSP */
		char Name[100]; /* Name of the DLL */

		/* If DLL supports memory these memory options then set them to TRUE or FALSE
       if it does not support it */
		int NormalMemory; /* a normal BYTE array */
		int MemoryBswaped; /* a normal BYTE array where the memory has been pre-
                              byte-swapped on a DWORD (32 bits) boundary */
	} PLUGIN_INFO;

#if !defined(M64P_PLUGIN_API)
	typedef struct
	{
		HINSTANCE hInst;
		int MemoryBswaped; /* If this is set to TRUE, then the memory has been
                              pre-byte-swapped on a DWORD (32 bits) boundary */
		unsigned char *RDRAM;
		unsigned char *DMEM;
		unsigned char *IMEM;

		RCPREG *MI_INTR_REG;

		RCPREG *SP_MEM_ADDR_REG;
		RCPREG *SP_DRAM_ADDR_REG;
		RCPREG *SP_RD_LEN_REG;
		RCPREG *SP_WR_LEN_REG;
		RCPREG *SP_STATUS_REG;
		RCPREG *SP_DMA_FULL_REG;
		RCPREG *SP_DMA_BUSY_REG;
		RCPREG *SP_PC_REG; /* This was SUPPOSED to be defined after the next. */
		RCPREG *SP_SEMAPHORE_REG;

		/** RCPREG *SP_PC_REG; // CPU-mapped between SP and DP command buffer regs **/

		RCPREG *DPC_START_REG;
		RCPREG *DPC_END_REG;
		RCPREG *DPC_CURRENT_REG;
		RCPREG *DPC_STATUS_REG;
		RCPREG *DPC_CLOCK_REG;
		RCPREG *DPC_BUFBUSY_REG;
		RCPREG *DPC_PIPEBUSY_REG;
		RCPREG *DPC_TMEM_REG;

		void (*CheckInterrupts)(void);
		void (*ProcessDList)(void);
		void (*ProcessAList)(void);
		void (*ProcessRdpList)(void);
		void (*ShowCFB)(void);
	} RSP_INFO;
#endif

	typedef struct
	{
		void (*UpdateBreakPoints)(void);
		void (*UpdateMemory)(void);
		void (*UpdateR4300iRegisters)(void);
		void (*Enter_BPoint_Window)(void);
		void (*Enter_R4300i_Commands_Window)(void);
		void (*Enter_R4300i_Register_Window)(void);
		void (*Enter_RSP_Commands_Window)(void);
		void (*Enter_Memory_Window)(void);
	} DEBUG_INFO;

#if defined(M64P_PLUGIN_API)
#define M64P_PLUGIN_PROTOTYPES 1
#include "m64p_common.h"
#include "m64p_config.h"
#include "m64p_plugin.h"
#include "m64p_types.h"
#else
#if defined(WIN32)
#define EXPORT __declspec(dllexport)
#define CALL __cdecl
#else
#define EXPORT __attribute__((visibility("default")))
#define CALL
#endif
#endif

#if !defined(M64P_PLUGIN_API)
	/******************************************************************
  Function: CloseDLL
  Purpose:  This function is called when the emulator is closing
            down allowing the DLL to de-initialise.
  input:    none
  output:   none
*******************************************************************/
	EXPORT void CALL CloseDLL(void);

	/******************************************************************
  Function: DllAbout
  Purpose:  This function is optional function that is provided
            to give further information about the DLL.
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/
	EXPORT void CALL DllAbout(HWND hParent);

	/******************************************************************
  Function: DllConfig
  Purpose:  This function is optional function that is provided
            to allow the user to configure the DLL
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/
	EXPORT void CALL DllConfig(HWND hParent);

	/******************************************************************
  Function: DllTest
  Purpose:  This function is optional function that is provided
            to allow the user to test the DLL
  input:    a handle to the window that calls this function
  output:   none
*******************************************************************/
	EXPORT void CALL DllTest(HWND hParent);
#endif

	/******************************************************************
  Function: DoRspCycles
  Purpose:  This function is to allow the RSP to run in parallel
            with the r4300 switching control back to the r4300 once
            the function ends.
  input:    The number of cycles that is meant to be executed
  output:   The number of cycles that was executed. This value can
            be greater than the number of cycles that the RSP
            should have performed.
            (this value is ignored if the RSP is stopped)
*******************************************************************/
	EXPORT unsigned int CALL DoRspCycles(unsigned int Cycles);

	/******************************************************************
  Function: GetDllInfo
  Purpose:  This function allows the emulator to gather information
            about the DLL by filling in the PluginInfo structure.
  input:    a pointer to a PLUGIN_INFO structure that needs to be
            filled by the function. (see def above)
  output:   none
*******************************************************************/
	EXPORT void CALL GetDllInfo(PLUGIN_INFO *PluginInfo);

	/*
 * `GetRspDebugInfo` -- customarily deprecated by cxd4
 *
 * It was extraordinarily easy to re-invent debug facilities without
 * depending on the Microsoft-Windows-themed debug functions from this spec.
 *
 * What's more?  No emulators supporting RSP plugins require this function.
 * It can be safely ignored as a non-portable custom extension to the spec.
 */

	/******************************************************************
  Function: InitiateRSP
  Purpose:  This function is called when the DLL is started to give
            information from the emulator that the n64 RSP 
            interface needs
  input:    Rsp_Info is passed to this function which is defined
            above.
            CycleCount is the number of cycles between switching
            control between the RSP and r4300i core.
  output:   none
*******************************************************************/
	EXPORT void CALL InitiateRSP(RSP_INFO Rsp_Info, unsigned int *CycleCount);

	/*
 * `InitiateRSPDebugger` -- customarily deprecated by cxd4
 *
 * Here, again, nothing about the full features of debugging this RSP
 * emulator needed to depend on any WIN32 fixations in this plugin spec.
 *
 * Also, again, as with the case of `GetRspDebugInfo`, the test of time has
 * passed the conclusion that no emulators require the RSP plugin to export
 * this procedure's symbol to be considered a valid RSP plugin.
 */

	/******************************************************************
  Function: RomClosed
  Purpose:  This function is called when a rom is closed.
  input:    none
  output:   none
*******************************************************************/
	EXPORT void CALL RomClosed(void);

#if defined(__cplusplus)
}
#endif

#endif
