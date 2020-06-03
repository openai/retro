#include <cassert>
#ifndef _WIN32
#include <dlfcn.h>
#endif
#include <fstream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "coreinfo.h"
#include "data.h"
#include "emulator.h"
#include "libretro.h"

#include "logging.h"

#ifndef _WIN32
#define GETSYM dlsym
#else
#define GETSYM GetProcAddress
#endif

using namespace std;

namespace Retro {

static Emulator* s_loadedEmulator = nullptr;

static map<string, const char*> s_envVariables = {
	{ "genesis_plus_gx_bram", "per game" },
	{ "genesis_plus_gx_render", "single field" },
	{ "genesis_plus_gx_blargg_ntsc_filter", "disabled" },
	// { "mupen64plus-rdp-plugin", "angrylion" },
	// { "mupen64plus-rsp-plugin", "hle" },
};

static const char* s_vshader_src =
	"#version 150\n"
	"in vec2 i_pos;\n"
	"in vec2 i_coord;\n"
	"out vec2 o_coord;\n"
	"uniform mat4 u_mvp;\n"
	"void main() {\n"
	"o_coord = i_coord;\n"
	"gl_Position = vec4(i_pos, 0.0, 1.0) * u_mvp;\n"
	"}";

static const char* s_fshader_src =
	"#version 150\n"
	"in vec2 o_coord;\n"
	"uniform sampler2D u_tex;\n"
	"out vec4 color;\n"
	"void main() {\n"
	"color = texture2D(u_tex, o_coord);\n"
	"}";

static void (*retro_init)(void);
static void (*retro_deinit)(void);
static unsigned (*retro_api_version)(void);
static void (*retro_get_system_info)(struct retro_system_info* info);
static void (*retro_get_system_av_info)(struct retro_system_av_info* info);
static void (*retro_reset)(void);
static void (*retro_run)(void);
static size_t (*retro_serialize_size)(void);
static bool (*retro_serialize)(void* data, size_t size);
static bool (*retro_unserialize)(const void* data, size_t size);
static bool (*retro_load_game)(const struct retro_game_info* game);
static void (*retro_unload_game)(void);
static void* (*retro_get_memory_data)(unsigned id);
static size_t (*retro_get_memory_size)(unsigned id);
static void (*retro_cheat_reset)(void);
static void (*retro_cheat_set)(unsigned index, bool enabled, const char* code);
static void (*retro_set_environment)(retro_environment_t);
static void (*retro_set_video_refresh)(retro_video_refresh_t);
static void (*retro_set_audio_sample)(retro_audio_sample_t);
static void (*retro_set_audio_sample_batch)(retro_audio_sample_batch_t);
static void (*retro_set_input_poll)(retro_input_poll_t);
static void (*retro_set_input_state)(retro_input_state_t);

Emulator::Emulator() {
}

Emulator::~Emulator() {
	if (m_corePath) {
		free(m_corePath);
	}
	if (m_romLoaded) {
		unloadRom();
	}
	if (m_coreHandle) {
		unloadCore();
	}
}

bool Emulator::isLoaded() {
	return s_loadedEmulator;
}

bool Emulator::loadRom(const string& romPath) {
	if (m_romLoaded) {
		unloadRom();
	}

	auto core = coreForRom(romPath);
	ZLOG("core: %s", core.c_str());
	if (core.size() == 0) {
		return false;
	}

	if (m_coreHandle && m_core != core) {
		unloadCore();
	}
	if (!m_coreHandle) {
		string lib = libForCore(core) + "_libretro.";
#ifdef __APPLE__
		lib += "dylib";
#elif defined(_WIN32)
		lib += "dll";
#else
		lib += "so";
#endif
		if (!loadCore(corePath() + "/" + lib)) {
			return false;
		}
		m_core = core;
	}
	ZLOG("Core loaded successfully", "");
	retro_game_info gameInfo;
	ifstream in(romPath, ios::binary | ios::ate);
	if (in.fail()) {
		return false;
	}
	ostringstream out;
	gameInfo.size = in.tellg();
	if (in.fail()) {
		return false;
	}
	char* romData = new char[gameInfo.size];
	gameInfo.path = romPath.c_str();
	gameInfo.data = romData;
	in.seekg(0, ios::beg);
	in.read(romData, gameInfo.size);
	if (in.fail()) {
		delete[] romData;
		return false;
	}
	in.close();
	ZLOG("loading game: path: %s", gameInfo.path);
	auto res = retro_load_game(&gameInfo);
	delete[] romData;
	if (!res) {
		ZLOG("failed to load game", "");
		return false;
	}
	retro_get_system_av_info(&m_avInfo);
	fixScreenSize(romPath);
	setupHardwareRender();

	m_romLoaded = true;
	m_romPath = romPath;
	return true;
}

void Emulator::run() {
	assert(s_loadedEmulator == this);
	m_audioData.clear();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	retro_run();
}

void Emulator::reset() {
	assert(s_loadedEmulator == this);

	memset(m_buttonMask, 0, sizeof(m_buttonMask));

	retro_system_info systemInfo;
	retro_get_system_info(&systemInfo);
	if (!strcmp(systemInfo.library_name, "Stella")) {
		// Stella does not properly clear everything when reseting or loading a savestate
		string romPath = m_romPath;

#ifdef _WIN32
		FreeLibrary(m_coreHandle);
#else
		dlclose(m_coreHandle);
#endif
		m_coreHandle = nullptr;
		s_loadedEmulator = nullptr;
		m_romLoaded = false;
		loadRom(m_romPath);
		if (m_addressSpace) {
			m_addressSpace->reset();
			m_addressSpace->addBlock(Retro::ramBase(m_core), retro_get_memory_size(RETRO_MEMORY_SYSTEM_RAM), retro_get_memory_data(RETRO_MEMORY_SYSTEM_RAM));
		}
	}

	retro_reset();
}

void Emulator::unloadCore() {
	if (!m_coreHandle) {
		return;
	}
	if (m_romLoaded) {
		unloadRom();
	}
	retro_deinit();
#ifdef _WIN32
	FreeLibrary(m_coreHandle);
#else
	dlclose(m_coreHandle);
#endif
	m_coreHandle = nullptr;
	s_loadedEmulator = nullptr;
}

void Emulator::unloadRom() {
	if (!m_romLoaded) {
		return;
	}
	retro_unload_game();
	m_romLoaded = false;
	m_romPath.clear();
	m_addressSpace = nullptr;
	m_map.clear();
}

bool Emulator::serialize(void* data, size_t size) {
	assert(s_loadedEmulator == this);
	return retro_serialize(data, size);
}

bool Emulator::unserialize(const void* data, size_t size) {
	assert(s_loadedEmulator == this);
	try {
		retro_system_info systemInfo;
		retro_get_system_info(&systemInfo);
		if (!strcmp(systemInfo.library_name, "Stella")) {
			reset();
		}

		return retro_unserialize(data, size);
	} catch (...) {
		return false;
	}
}

size_t Emulator::serializeSize() {
	assert(s_loadedEmulator == this);
	return retro_serialize_size();
}

void Emulator::clearCheats() {
	assert(s_loadedEmulator == this);
	retro_cheat_reset();
}

void Emulator::setCheat(unsigned index, bool enabled, const char* code) {
	assert(s_loadedEmulator == this);
	retro_cheat_set(index, enabled, code);
}

bool Emulator::loadCore(const string& corePath) {
	ZLOG("loadCore: corePath: %s", corePath.c_str());
	if (s_loadedEmulator) {
		return false;
	}

#ifdef _WIN32
	m_coreHandle = LoadLibrary(corePath.c_str());
#else
	m_coreHandle = dlopen(corePath.c_str(), RTLD_LAZY);
#endif
	if (!m_coreHandle) {
		return false;
	}

	// I guess this is casting the underlying emulator functions to a common, usable form.
	retro_init = reinterpret_cast<void (*)()>(GETSYM(m_coreHandle, "retro_init"));
	retro_deinit = reinterpret_cast<void (*)()>(GETSYM(m_coreHandle, "retro_deinit"));
	retro_api_version = reinterpret_cast<unsigned int (*)()>(GETSYM(m_coreHandle, "retro_api_version"));
	retro_get_system_info = reinterpret_cast<void (*)(struct retro_system_info*)>(GETSYM(m_coreHandle, "retro_get_system_info"));
	retro_get_system_av_info = reinterpret_cast<void (*)(struct retro_system_av_info*)>(GETSYM(m_coreHandle, "retro_get_system_av_info"));
	retro_reset = reinterpret_cast<void (*)()>(GETSYM(m_coreHandle, "retro_reset"));
	retro_run = reinterpret_cast<void (*)()>(GETSYM(m_coreHandle, "retro_run"));
	retro_serialize_size = reinterpret_cast<size_t (*)()>(GETSYM(m_coreHandle, "retro_serialize_size"));
	retro_serialize = reinterpret_cast<bool (*)(void*, size_t)>(GETSYM(m_coreHandle, "retro_serialize"));
	retro_unserialize = reinterpret_cast<bool (*)(const void*, size_t)>(GETSYM(m_coreHandle, "retro_unserialize"));
	retro_load_game = reinterpret_cast<bool (*)(const struct retro_game_info*)>(GETSYM(m_coreHandle, "retro_load_game"));
	retro_unload_game = reinterpret_cast<void (*)()>(GETSYM(m_coreHandle, "retro_unload_game"));
	retro_get_memory_data = reinterpret_cast<void* (*) (unsigned int)>(GETSYM(m_coreHandle, "retro_get_memory_data"));
	retro_get_memory_size = reinterpret_cast<size_t (*)(unsigned int)>(GETSYM(m_coreHandle, "retro_get_memory_size"));
	retro_cheat_reset = reinterpret_cast<void (*)()>(GETSYM(m_coreHandle, "retro_cheat_reset"));
	retro_cheat_set = reinterpret_cast<void (*)(unsigned int, bool, const char*)>(GETSYM(m_coreHandle, "retro_cheat_set"));
	retro_set_environment = reinterpret_cast<void (*)(retro_environment_t)>(GETSYM(m_coreHandle, "retro_set_environment"));
	retro_set_video_refresh = reinterpret_cast<void (*)(retro_video_refresh_t)>(GETSYM(m_coreHandle, "retro_set_video_refresh"));
	retro_set_audio_sample = reinterpret_cast<void (*)(retro_audio_sample_t)>(GETSYM(m_coreHandle, "retro_set_audio_sample"));
	retro_set_audio_sample_batch = reinterpret_cast<void (*)(retro_audio_sample_batch_t)>(GETSYM(m_coreHandle, "retro_set_audio_sample_batch"));
	retro_set_input_poll = reinterpret_cast<void (*)(retro_input_poll_t)>(GETSYM(m_coreHandle, "retro_set_input_poll"));
	retro_set_input_state = reinterpret_cast<void (*)(short (*)(unsigned int, unsigned int, unsigned int, unsigned int))>(GETSYM(m_coreHandle, "retro_set_input_state"));
	// The default according to the docs
	m_imgDepth = 15;
	s_loadedEmulator = this;

	retro_set_environment(cbEnvironment);
	retro_set_video_refresh(cbVideoRefresh);
	retro_set_audio_sample(cbAudioSample);
	retro_set_audio_sample_batch(cbAudioSampleBatch);
	retro_set_input_poll(cbInputPoll);
	retro_set_input_state(cbInputState);
	retro_init();
	return true;
}

void Emulator::fixScreenSize(const string& romName) {
	retro_system_info systemInfo;
	retro_get_system_info(&systemInfo);
	if (!strcmp(systemInfo.library_name, "Genesis Plus GX")) {
		switch (romName.back()) {
		case 'd': // Mega Drive
			// Genesis Plus GX gives us too small a resolution initially
			m_avInfo.geometry.base_width = 320;
			m_avInfo.geometry.base_height = 224;
			break;
		case 's': // Master System
			// Genesis Plus GX gives us too small a resolution initially
			m_avInfo.geometry.base_width = 256;
			m_avInfo.geometry.base_height = 192;
			break;
		case 'g': // Game Gear
			m_avInfo.geometry.base_width = 160;
			m_avInfo.geometry.base_height = 144;
			break;
		}
	} else if (!strcmp(systemInfo.library_name, "Stella")) {
		// Stella gives confusing values to pretend the pixel width is 2x
		m_avInfo.geometry.base_width = 160;
	} else if (!strcmp(systemInfo.library_name, "Mednafen PCE Fast")) {
		m_avInfo.geometry.base_width = 256;
		m_avInfo.geometry.base_height = 242;
	} else if (!strcmp(systemInfo.library_name, "Mupen64Plus-Next OpenGL")) {
		// Default results in bottom half of frame being empty.
		m_avInfo.geometry.base_height = 800; // 236
		m_avInfo.geometry.base_height = 800;
	}
}

void Emulator::reconfigureAddressSpace() {
	if (!m_addressSpace) {
		return;
	}
	if (!m_map.empty() && m_addressSpace->blocks().empty()) {
		for (const auto& desc : m_map) {
			if (desc.flags & RETRO_MEMDESC_CONST) {
				continue;
			}
			size_t len = desc.len;
			if (desc.select) {
				len = ((~desc.select & ~desc.start) + 1) & desc.select;
			}
			if (desc.len && desc.len < len) {
				len = desc.len;
			}
			m_addressSpace->addBlock(desc.start, len, desc.ptr);
		}
	}
}

static void fallback_log(enum retro_log_level level, const char* fmt, ...) {
	(void) level;
	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	va_end(va);
}

GLuint Emulator::compileShader(unsigned type, unsigned count, const char** strings) {
	ZLOG("compileShader", "");
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, count, strings, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		char buffer[4096];
		glGetShaderInfoLog(shader, sizeof(buffer), NULL, buffer);
		ZLOG("Failed to compile %s shader: %s", type == GL_VERTEX_SHADER ? "vertex" : "fragment", buffer);
		exit(EXIT_FAILURE);
	}

	return shader;
}

namespace {

void ortho2d(float m[4][4], float left, float right, float bottom, float top) {
	// clang-format off
        m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
        m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
        m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
        m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

        m[0][0] = 2.0f / (right - left);
        m[1][1] = 2.0f / (top - bottom);
        m[2][2] = -1.0f;
        m[3][0] = -(right + left) / (right - left);
        m[3][1] = -(top + bottom) / (top - bottom);
	// clang-format on
}

static void resize_to_aspect(double ratio, int sw, int sh, int* dw, int* dh) {
	*dw = sw;
	*dh = sh;

	if (ratio <= 0)
		ratio = (double) sw / sh;

	if ((float) sw / sh < 1)
		*dw = *dh * ratio;
	else
		*dh = *dw / ratio;
}

} //  namespace

void Emulator::refreshVertexData() {
	ZLOG("refreshVertexData", "");
	const auto tex_w = m_avInfo.geometry.max_width;
	const auto tex_h = m_avInfo.geometry.max_height;
	const auto clip_w = m_avInfo.geometry.base_width;
	const auto clip_h = m_avInfo.geometry.base_height;
	assert(tex_w);
	assert(tex_h);
	assert(clip_w);
	assert(clip_h);

	float bottom = (float) clip_h / tex_h;
	float right = (float) clip_w / tex_w;

	float vertex_data[] = {
		// pos, coord
		-1.0f, -1.0f, 0.0f, bottom, // left-bottom
		-1.0f, 1.0f, 0.0f, 0.0f,    // left-top
		1.0f, -1.0f, right, bottom, // right-bottom
		1.0f, 1.0f, right, 0.0f,    // right-top
	};

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STREAM_DRAW);

	glEnableVertexAttribArray(m_i_pos);
	glEnableVertexAttribArray(m_i_coord);
	glVertexAttribPointer(m_i_pos, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glVertexAttribPointer(m_i_coord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (2 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Emulator::initShaders() {
	ZLOG("initShaders", "");
	GLuint vshader = Emulator::compileShader(GL_VERTEX_SHADER, 1, &s_vshader_src);
	GLuint fshader = Emulator::compileShader(GL_FRAGMENT_SHADER, 1, &s_fshader_src);
	GLuint program = glCreateProgram();
	assert(program);

	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);

	glDeleteShader(vshader);
	glDeleteShader(fshader);

	glValidateProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		char buffer[4096];
		glGetProgramInfoLog(program, sizeof(buffer), NULL, buffer);
		ZLOG("Failed to link shader program: %s", buffer);
		exit(EXIT_FAILURE);
	}

	m_program = program;
	m_i_pos = glGetAttribLocation(program, "i_pos");
	m_i_coord = glGetAttribLocation(program, "i_coord");
	m_u_tex = glGetUniformLocation(program, "u_tex");
	m_u_mvp = glGetUniformLocation(program, "u_mvp");

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glUseProgram(m_program);

	glUniform1i(m_u_tex, 0);

	float m[4][4];
	if (m_hw.bottom_left_origin) {
		ortho2d(m, -1, 1, 1, -1);
	} else {
		ortho2d(m, -1, 1, -1, 1);
	}

	glUniformMatrix4fv(m_u_mvp, 1, GL_FALSE, (float*) m);
	glUseProgram(0);
}

void Emulator::createWindow() {
	ZLOG("createWindow", "");
	assert(m_hw.context_type == RETRO_HW_CONTEXT_OPENGL_CORE);
	glfwInit();

	// Create window and context.

	// Set configuration variables.
	ZLOG("GLFW VERSION MAJOR: %d", m_hw.version_major);
	ZLOG("GLFW VERSION MINOR: %d", m_hw.version_minor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_hw.version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_hw.version_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// And more variables from glsm.c:
	// hw_render.context_reset      = params->context_reset;
	// hw_render.context_destroy    = params->context_destroy;
	// hw_render.stencil            = params->stencil;
	// hw_render.depth              = true;
	// hw_render.bottom_left_origin = true;
	// hw_render.cache_context      = true;
	// Set window to be invisible:
	// set GLFW_VISIBLE appropriately.
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

	// For some reason the image width / height is zero at this point.
	int width = getImageWidth();
	int height = getImageHeight();
	m_glfw_window = glfwCreateWindow(width, height, "retro", nullptr, nullptr);

	// Check for Valid Context
	if (m_glfw_window == nullptr) {
		int code = glfwGetError(nullptr);
		ZLOG("Failed to Create OpenGL Context. Error code: %#010x\n", code);
		exit(EXIT_FAILURE);
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(m_glfw_window);

	// I think this is equivalent to the below, but whatever.
	// gladLoadGL();
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		ZLOG("Failed to initialize opengl context.", "");
	}

	initShaders();
	glfwSwapInterval(1);
	glfwSwapBuffers(m_glfw_window);

	glGetStringi(GL_EXTENSIONS, 0);
	const char* name = (const char*) glGetStringi(GL_EXTENSIONS, 0);
	printf("name:\n%s\n", name);
}

void Emulator::initFramebuffer() {
	ZLOG("initFramebuffer", "");
	int width = getImageWidth();
	int height = getImageHeight();

	glGenFramebuffers(1, &m_fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex_id, 0);

	if (m_hw.depth && m_hw.stencil) {
		glGenRenderbuffers(1, &m_rbo_id);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo_id);
	} else if (m_hw.depth) {
		glGenRenderbuffers(1, &m_rbo_id);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo_id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo_id);
	}

	if (m_hw.depth || m_hw.stencil) {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Emulator::setupHardwareRender() {
	ZLOG("setupHardwareRender", "");

	auto* geom = &m_avInfo.geometry;
	// int nwidth, nheight;
	// resize_to_aspect(geom->aspect_ratio, geom->base_width * 1, geom->base_height * 1, &nwidth, &nheight);

	int nwidth = getImageWidth();
	int nheight = getImageHeight();

	if (!m_glfw_window) {
		createWindow();
	}

	// wish this was in a function
	if (m_tex_id) {
		glDeleteTextures(1, &m_tex_id);
	}
	m_tex_id = 0;
	if (!m_pixfmt) {
		m_pixfmt = GL_UNSIGNED_SHORT_5_5_5_1;
	}

	// need to add this
	glfwSetWindowSize(m_glfw_window, nwidth, nheight);

	glGenTextures(1, &m_tex_id);
	if (!m_tex_id) {
		ZLOG("Failed to create the video texture", "");
		exit(EXIT_FAILURE);
	}

	m_imgPitch = geom->max_width * m_bpp;

	glBindTexture(GL_TEXTURE_2D, m_tex_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, geom->max_width, geom->max_height, 0,
		m_pixtype, m_pixfmt, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	// end wish this was in a function
	initFramebuffer();
	refreshVertexData();
	m_hw.context_reset();
	return true;

	// Where does the rest of this go?
	// // Rendering Loop
	// while (glfwWindowShouldClose(mWindow) == false) {
	//     if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//         glfwSetWindowShouldClose(mWindow, true);

	//     // Background Fill Color
	//     glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	//     glClear(GL_COLOR_BUFFER_BIT);

	//     // Flip Buffers and Draw
	//     glfwSwapBuffers(mWindow);
	//     glfwPollEvents();
	// }   glfwTerminate();
}

uintptr_t Emulator::cbGetCurrentFramebuffer() {
	ZLOG("cbGetCurrentFramebuffer", "");
	assert(s_loadedEmulator);
	return s_loadedEmulator->m_fbo_id;
}

bool Emulator::cbEnvironment(unsigned cmd, void* data) {
	assert(s_loadedEmulator);
	ZLOG("cbEnvironment", "");
	switch (cmd) {
	case RETRO_ENVIRONMENT_SET_HW_RENDER: {
		ZLOG("RETRO_ENVIRONMENT_SET_HW_RENDER", "");
		struct retro_hw_render_callback* hw = reinterpret_cast<struct retro_hw_render_callback*>(data);
		hw->get_current_framebuffer = s_loadedEmulator->cbGetCurrentFramebuffer;
		hw->get_proc_address = (retro_hw_get_proc_address_t) glfwGetProcAddress;
		s_loadedEmulator->m_hw = *hw;
		return true;
	}
	case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
		ZLOG("RETRO_ENVIRONMENT_SET_PIXEL_FORMAT", "");
		switch (*reinterpret_cast<retro_pixel_format*>(data)) {
		case RETRO_PIXEL_FORMAT_XRGB8888:
			s_loadedEmulator->m_imgDepth = 32;
			s_loadedEmulator->m_pixfmt = GL_UNSIGNED_INT_8_8_8_8_REV;
			s_loadedEmulator->m_pixtype = GL_BGRA;
			s_loadedEmulator->m_bpp = sizeof(uint32_t);
			break;
		case RETRO_PIXEL_FORMAT_RGB565:
			s_loadedEmulator->m_imgDepth = 16;
			s_loadedEmulator->m_pixfmt = GL_UNSIGNED_SHORT_5_6_5;
			s_loadedEmulator->m_pixtype = GL_RGB;
			s_loadedEmulator->m_bpp = sizeof(uint16_t);
			break;
		case RETRO_PIXEL_FORMAT_0RGB1555:
			s_loadedEmulator->m_imgDepth = 15;
			s_loadedEmulator->m_pixfmt = GL_UNSIGNED_SHORT_5_5_5_1;
			s_loadedEmulator->m_pixtype = GL_BGRA;
			s_loadedEmulator->m_bpp = sizeof(uint16_t);
			break;
		default:
			s_loadedEmulator->m_imgDepth = 0;
			break;
		}
		return true;
	case RETRO_ENVIRONMENT_GET_VARIABLE: {
		ZLOG("RETRO_ENVIRONMENT_GET_VARIABLE", "");
		struct retro_variable* var = reinterpret_cast<struct retro_variable*>(data);
		if (s_envVariables.count(string(var->key))) {
			var->value = s_envVariables[string(var->key)];
			ZLOG("key: %s value: %s", var->key, var->value);
			ZLOG("key: %s value: %s", var->key, s_envVariables[string(var->key)]);
			return true;
		}
		return false;
	}
	case RETRO_ENVIRONMENT_GET_LOG_INTERFACE: {
		ZLOG("RETRO_ENVIRONMENT_GET_LOG_INTERFACE", "");
		struct retro_log_callback* log = reinterpret_cast<struct retro_log_callback*>(data);
		log->log = fallback_log;
		return true;
	}
	case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
		ZLOG("RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY", "");
		if (!s_loadedEmulator->m_corePath) {
			s_loadedEmulator->m_corePath = strdup(corePath().c_str());
		}
		*reinterpret_cast<const char**>(data) = s_loadedEmulator->m_corePath;
		return true;
	case RETRO_ENVIRONMENT_GET_CAN_DUPE:
		ZLOG("RETRO_ENVIRONMENT_GET_CAN_DUPE", "");
		*reinterpret_cast<bool*>(data) = true;
		return true;
	case RETRO_ENVIRONMENT_SET_MEMORY_MAPS:
		ZLOG("RETRO_ENVIRONMENT_SET_MEMORY_MAPS", "");
		s_loadedEmulator->m_map.clear();
		for (size_t i = 0; i < static_cast<const retro_memory_map*>(data)->num_descriptors; ++i) {
			s_loadedEmulator->m_map.emplace_back(static_cast<const retro_memory_map*>(data)->descriptors[i]);
		}
		s_loadedEmulator->reconfigureAddressSpace();
		return true;
	case RETRO_ENVIRONMENT_SET_VARIABLES: {
		ZLOG("RETRO_ENVIRONMENT_SET_VARIABLES", "");
		const struct retro_variable* vars = (const struct retro_variable*) data;
		size_t num_vars = 0;

		for (const struct retro_variable* v = vars; v->key; ++v) {
			num_vars++;
		}
		struct retro_variable* g_vars = NULL;
		g_vars = (struct retro_variable*) calloc(num_vars + 1, sizeof(*g_vars));
		for (unsigned i = 0; i < num_vars; ++i) {
			const struct retro_variable* invar = &vars[i];
			struct retro_variable* outvar = &g_vars[i];

			const char* semicolon = strchr(invar->value, ';');
			const char* first_pipe = strchr(invar->value, '|');

			assert(semicolon && *semicolon);
			semicolon++;
			while (isspace(*semicolon))
				semicolon++;

			if (first_pipe) {
				outvar->value = (char*) malloc((first_pipe - semicolon) + 1);
				memcpy((char*) outvar->value, semicolon, first_pipe - semicolon);
				((char*) outvar->value)[first_pipe - semicolon] = '\0';
			} else {
				outvar->value = strdup(semicolon);
			}

			outvar->key = strdup(invar->key);
			assert(outvar->key && outvar->value);
		}

		for (const struct retro_variable* v = g_vars; v->key; ++v) {
			s_envVariables.insert({ string(v->key), v->value });
		}
		for (const auto& [key, value] : s_envVariables) {
			ZLOG("key: %s value: %s", key.c_str(), value);
		}
                // Freeing this breaks stuff so I guess I just won't free them!
		// for (const struct retro_variable* v = g_vars; v->key; ++v) {
		// 	free((char*) v->key);
		// 	free((char*) v->value);
		// }
		// free(g_vars);

		return true;
	}
	case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE: {
		ZLOG("RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE", "");
		bool* bval = (bool*) data;
		*bval = false;
		return true;
	}
	case RETRO_ENVIRONMENT_SET_GEOMETRY: {
		ZLOG("RETRO_ENVIRONMENT_SET_GEOMETRY", "");
		return false;
	}
	default:
		ZLOG("default", "");
		return false;
	}
	ZLOG("end", "");
	return false;
}

void Emulator::cbVideoRefresh(const void* data, unsigned width, unsigned height, size_t pitch) {
	ZLOG("cbVideoRefresh", "");
	assert(s_loadedEmulator);
	if (data) {
		// THIS MIGHT BREAK NOW.
		// Ok it definitely breaks. Need to set it to real image data.
		// s_loadedEmulator->m_imgData = data;
	}
	if (pitch) {
		s_loadedEmulator->m_imgPitch = pitch;
	}

	if (width != s_loadedEmulator->getImageWidth() || height != s_loadedEmulator->getImageHeight()) {
		s_loadedEmulator->refreshVertexData();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, s_loadedEmulator->m_tex_id);

	if (data && data != RETRO_HW_FRAME_BUFFER_VALID) {
		glPixelStorei(GL_UNPACK_ROW_LENGTH, s_loadedEmulator->m_imgPitch / s_loadedEmulator->m_bpp);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
			s_loadedEmulator->m_pixtype, s_loadedEmulator->m_pixfmt, data);
	}

	int w = 0, h = 0;
	glfwGetWindowSize(s_loadedEmulator->m_glfw_window, &w, &h);
	glViewport(0, 0, w, h);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(s_loadedEmulator->m_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, s_loadedEmulator->m_tex_id);
	glBindVertexArray(s_loadedEmulator->m_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glUseProgram(0);
	glfwSwapBuffers(s_loadedEmulator->m_glfw_window);
}

void Emulator::cbAudioSample(int16_t left, int16_t right) {
	assert(s_loadedEmulator);
	s_loadedEmulator->m_audioData.push_back(left);
	s_loadedEmulator->m_audioData.push_back(right);
}

size_t Emulator::cbAudioSampleBatch(const int16_t* data, size_t frames) {
	assert(s_loadedEmulator);
	s_loadedEmulator->m_audioData.insert(s_loadedEmulator->m_audioData.end(), data, &data[frames * 2]);
	return frames;
}

void Emulator::cbInputPoll() {
	assert(s_loadedEmulator);
}

int16_t Emulator::cbInputState(unsigned port, unsigned, unsigned, unsigned id) {
	assert(s_loadedEmulator);
	return s_loadedEmulator->m_buttonMask[port][id];
}

void Emulator::configureData(GameData* data) {
	m_addressSpace = &data->addressSpace();
	m_addressSpace->reset();
	Retro::configureData(data, m_core);
	reconfigureAddressSpace();
	ZLOG("rambase: %zx", Retro::ramBase(m_core));
	ZLOG("memory size: %zx", retro_get_memory_size(RETRO_MEMORY_SYSTEM_RAM));
	if (m_addressSpace->blocks().empty() && retro_get_memory_size(RETRO_MEMORY_SYSTEM_RAM)) {
		ZLOG("adding block to memory", "");
		m_addressSpace->addBlock(Retro::ramBase(m_core), retro_get_memory_size(RETRO_MEMORY_SYSTEM_RAM), retro_get_memory_data(RETRO_MEMORY_SYSTEM_RAM));
	}
}

vector<string> Emulator::buttons() const {
	return Retro::buttons(m_core);
}

vector<string> Emulator::keybinds() const {
	return Retro::keybinds(m_core);
}
}
