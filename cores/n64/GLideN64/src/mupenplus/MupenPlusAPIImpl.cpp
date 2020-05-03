#include "GLideN64_mupenplus.h"
#include "../PluginAPI.h"
#include "../GLideN64.h"
#include "../Config.h"
#include <DisplayWindow.h>

void(*renderCallback)(int) = nullptr;

m64p_error PluginAPI::PluginStartup(m64p_dynlib_handle _CoreLibHandle)
{
	return M64ERR_SUCCESS;
}

m64p_error PluginAPI::PluginShutdown()
{
#ifdef RSPTHREAD
	_callAPICommand(acRomClosed);
	delete m_pRspThread;
	m_pRspThread = nullptr;
#endif
	return M64ERR_SUCCESS;
}

m64p_error PluginAPI::PluginGetVersion(
	m64p_plugin_type * _PluginType,
	int * _PluginVersion,
	int * _APIVersion,
	const char ** _PluginNamePtr,
	int * _Capabilities
)
{
	/* set version info */
	if (_PluginType != nullptr)
		*_PluginType = M64PLUGIN_GFX;

	if (_PluginVersion != nullptr)
		*_PluginVersion = PLUGIN_VERSION;

	if (_APIVersion != nullptr)
		*_APIVersion = VIDEO_PLUGIN_API_VERSION;

	if (_PluginNamePtr != nullptr)
		*_PluginNamePtr = pluginName;

	if (_Capabilities != nullptr)
	{
		*_Capabilities = 0;
	}

	return M64ERR_SUCCESS;
}

void PluginAPI::SetRenderingCallback(void (*callback)(int))
{
	renderCallback = callback;
}

void PluginAPI::ResizeVideoOutput(int _Width, int _Height)
{
	dwnd().setWindowSize(_Width, _Height);
}

void PluginAPI::ReadScreen2(void * _dest, int * _width, int * _height, int _front)
{
	dwnd().readScreen2(_dest, _width, _height, _front);
}
