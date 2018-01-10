#pragma once

#include "libretro.h"
#include "memory.h"

#include <string>
#include <vector>
#include <cstring>
#include <stdarg.h>
#ifdef _WIN32
#include <windows.h>
#endif

namespace Retro {

const int N_BUTTONS = 16;
const int MAX_PLAYERS = 2;

class GameData;
class Emulator {
public:
	Emulator();
	~Emulator();
	Emulator(const Emulator&) = delete;

	static bool isLoaded();

	bool loadRom(const std::string& romPath);

	void run();
	void reset();
	AddressSpace* getAddressSpace();
	const void* getImageData() { return m_imgData; }
	int getImageHeight() { return m_avInfo.geometry.base_height; }
	int getImageWidth() { return m_avInfo.geometry.base_width; }
	int getImagePitch() { return m_imgPitch; }
	int getImageDepth() { return m_imgDepth; }
	double getFrameRate() { return m_avInfo.timing.fps; }
	int getAudioSamples() { return m_audioData.size() / 2; }
	double getAudioRate() { return m_avInfo.timing.sample_rate; }
	const int16_t* getAudioData() { return m_audioData.data(); }
	void unloadCore();
	void unloadRom();

	bool serialize(void* data, size_t size);
	bool unserialize(const void* data, size_t size);
	size_t serializeSize();

	void setKey(int port, int key, bool active) { m_buttonMask[port][key] = active; }
	bool getKey(int port, int key) { return m_buttonMask[port][key]; }

	void clearCheats();
	void setCheat(unsigned index, bool enabled, const char* code);

	std::string core() const { return m_core; }
	void configureData(GameData*);
	std::vector<std::string> buttons() const;
	std::vector<std::string> keybinds() const;

private:
	bool loadCore(const std::string& corePath);
	void fixScreenSize(const std::string& romName);
	void reconfigureAddressSpace();

	static bool cbEnvironment(unsigned cmd, void* data);
	static void cbVideoRefresh(const void* data, unsigned width, unsigned height, size_t pitch);
	static void cbAudioSample(int16_t left, int16_t right);
	static size_t cbAudioSampleBatch(const int16_t* data, size_t frames);
	static void cbInputPoll();
	static int16_t cbInputState(unsigned port, unsigned device, unsigned index, unsigned id);

	bool m_buttonMask[MAX_PLAYERS][N_BUTTONS]{};

	// Video frame info
	const void* m_imgData = nullptr;
	size_t m_imgPitch = 0;
	int m_imgDepth = 0;

	// Audio buffer; accumulated during run()
	std::vector<int16_t> m_audioData;
	AddressSpace* m_addressSpace = nullptr;

	retro_system_av_info m_avInfo = {};
	std::vector<retro_memory_descriptor> m_map;

#ifdef _WIN32
	HMODULE m_coreHandle = nullptr;
#else
	void* m_coreHandle = nullptr;
#endif
	bool m_romLoaded = false;
	std::string m_core;
	std::string m_romPath;
};
}
