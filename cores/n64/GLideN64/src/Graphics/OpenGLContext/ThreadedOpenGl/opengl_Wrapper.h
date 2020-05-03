#pragma once

#include "Graphics/OpenGLContext/GLFunctions.h"
#include "readerwriterqueue.h"
#include "opengl_WrappedFunctions.h"
#include "opengl_Command.h"
#include <thread>
#include <map>

#ifdef MUPENPLUSAPI
#include <mupenplus/GLideN64_mupenplus.h>
#endif

using namespace moodycamel;

namespace opengl {

	class FunctionWrapper
	{
	private:
		static void executeCommand(std::shared_ptr<OpenGlCommand> _command);

		static void executePriorityCommand(std::shared_ptr<OpenGlCommand> _command);

		static void commandLoop();

		static BlockingReaderWriterQueue<std::shared_ptr<OpenGlCommand>> m_commandQueue;
		static BlockingReaderWriterQueue<std::shared_ptr<OpenGlCommand>> m_commandQueueHighPriority;

		static bool m_threaded_wrapper;
		static bool m_shutdown;
		static int m_swapBuffersQueued;
		static bool m_fastVertexAttributes;
		static std::thread m_commandExecutionThread;
		static std::mutex m_condvarMutex;
		static std::condition_variable m_condition;

#if defined(GL_DEBUG) && defined(GL_PROFILE)
		static void logProfilingData();

		struct FunctionProfilingData
		{
			double m_totalTime;
			int m_callCount;

			FunctionProfilingData()
			{
				m_totalTime = 0;
				m_callCount = 0;
			}
		};
		static std::map<std::string, FunctionProfilingData> m_functionProfiling;
		static std::chrono::time_point<std::chrono::high_resolution_clock> m_lastProfilingOutput;
#endif

		static const int MAX_SWAP = 2;

	public:
		static void setThreadedMode(u32 _threaded);

		static void wrBlendFunc(GLenum sfactor, GLenum dfactor);
		static void wrPixelStorei(GLenum pname, GLint param);
		static void wrClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
		static void wrCullFace(GLenum mode);
		static void wrDepthFunc(GLenum func);
		static void wrDepthMask(GLboolean flag);
		static void wrDisable(GLenum cap);
		static void wrEnable(GLenum cap);
		static void wrDisablei(GLenum target, GLuint index);
		static void wrEnablei(GLenum target, GLuint index);
		static void wrPolygonOffset(GLfloat factor, GLfloat units);

		static void wrScissor(GLint x, GLint y, GLsizei width, GLsizei height);
		static void wrViewport(GLint x, GLint y, GLsizei width, GLsizei height);
		static void wrBindTexture(GLenum target, GLuint texture);
		static void wrTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
		static void wrTexParameteri(GLenum target, GLenum pname, GLint param);
		static void wrGetIntegerv(GLenum pname, GLint* data);
		static const GLubyte* wrGetString(GLenum name);

		static void wrReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
		static void wrTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
		static void wrDrawArrays(GLenum mode, GLint first, GLsizei count);
        static GLenum wrGetError();
		static void wrDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);
		static void wrLineWidth(GLfloat width);
		static void wrClear(GLbitfield mask);
		static void wrClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value);
		static void wrGetFloatv(GLenum pname, GLfloat* data);
		static void wrDeleteTextures(GLsizei n, const GLuint *textures);
		static void wrGenTextures(GLsizei n, GLuint* textures);
		static void wrTexParameterf(GLenum target, GLenum pname, GLfloat param);
		static void wrActiveTexture(GLenum texture);
		static void wrBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
		static void wrReadBuffer(GLenum src);

		static GLuint wrCreateShader(GLenum type);
		static void wrCompileShader(GLuint shader);
		static void wrShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
		static GLuint wrCreateProgram();
		static void wrAttachShader(GLuint program, GLuint shader);
		static void wrLinkProgram(GLuint program);
		static void wrUseProgram(GLuint program);
		static GLint wrGetUniformLocation(GLuint program, const GLchar *name);
		static void wrUniform1i(GLint location, GLint v0);
		static void wrUniform1f(GLint location, GLfloat v0);
		static void wrUniform2f(GLint location, GLfloat v0, GLfloat v1);
		static void wrUniform2i(GLint location, GLint v0, GLint v1);
		static void wrUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

		static void wrUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		static void wrUniform3fv(GLint location, GLsizei count, const GLfloat *value);
		static void wrUniform4fv(GLint location, GLsizei count, const GLfloat *value);
		static void wrDetachShader(GLuint program, GLuint shader);
		static void wrDeleteShader(GLuint shader);
		static void wrDeleteProgram(GLuint program);
		static void wrGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar *infoLog);
		static void wrGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar *infoLog);
		static void wrGetShaderiv(GLuint shader, GLenum pname, GLint* params);
		static void wrGetProgramiv(GLuint program, GLenum pname, GLint* params);

		static void wrEnableVertexAttribArray(GLuint index);
		static void wrDisableVertexAttribArray(GLuint index);
		static void wrVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
		static void wrBindAttribLocation(GLuint program, GLuint index, const GLchar *name);
		static void wrVertexAttrib1f(GLuint index, GLfloat x);
		static void wrVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		static void wrVertexAttrib4fv(GLuint index, const GLfloat *v);

		static void wrDepthRangef(GLfloat n, GLfloat f);
		static void wrClearDepthf(GLfloat d);

		static void wrDrawBuffers(GLsizei n, const GLenum *bufs);
		static void wrGenFramebuffers(GLsizei n, GLuint* framebuffers);
		static void wrBindFramebuffer(GLenum target, GLuint framebuffer);
		static void wrDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
		static void wrFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
		static void wrTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
		static void wrTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
		static void wrGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
		static void wrBindRenderbuffer(GLenum target, GLuint renderbuffer);
		static void wrRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
		static void wrDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
		static void wrFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
		static GLenum wrCheckFramebufferStatus(GLenum target);
		static void wrBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
		static void wrGenVertexArrays(GLsizei n, GLuint* arrays);
		static void wrBindVertexArray(GLuint array);
		static void wrDeleteVertexArrays(GLsizei n, const GLuint *arrays);
		static void wrGenBuffers(GLsizei n, GLuint* buffers);
		static void wrBindBuffer(GLenum target, GLuint buffer);
		static void wrBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
		static void wrMapBuffer(GLenum target, GLenum access);
		static void* wrMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
		static GLboolean wrUnmapBuffer(GLenum target);
		static void wrDeleteBuffers(GLsizei n, const GLuint *buffers);
		static void wrBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
		static void wrMemoryBarrier(GLbitfield barriers);
		static void wrTextureBarrier();
		static void wrTextureBarrierNV();
		static const GLubyte* wrGetStringi(GLenum name, GLuint index);
		static void wrInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments);
		static void wrBufferStorage(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
		static GLsync wrFenceSync(GLenum condition, GLbitfield flags);
		static void wrClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
		static void wrDeleteSync(GLsync sync);

		static GLuint wrGetUniformBlockIndex(GLuint program, GLchar *uniformBlockName);
		static void wrUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
		static void wrGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
		static void wrGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint* uniformIndices);
		static void wrGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint* params);
		static void wrBindBufferBase(GLenum target, GLuint index, GLuint buffer);
		static void wrBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);

		static void wrGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void *binary);
		static void wrProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
		static void wrProgramParameteri(GLuint program, GLenum pname, GLint value);

		static void wrTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
		static void wrTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
		static void wrTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
		static void wrTextureStorage2DMultisample(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
		static void wrTextureParameteri(GLuint texture, GLenum pname, GLint param);
		static void wrTextureParameterf(GLuint texture, GLenum pname, GLfloat param);
		static void wrCreateTextures(GLenum target, GLsizei n, GLuint* textures);
		static void wrCreateBuffers(GLsizei n, GLuint* buffers);
		static void wrCreateFramebuffers(GLsizei n, GLuint* framebuffers);
		static void wrNamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
		static void wrDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const u16* indices, GLint basevertex);
		static void wrFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);
		static void wrFinish();
		static void wrCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
		static void wrDebugMessageCallback(GLDEBUGPROC callback, const void *userParam);
		static void wrDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
		static void wrEGLImageTargetTexture2DOES(GLenum target, void* image);
		static void wrEGLImageTargetRenderbufferStorageOES(GLenum target, void* image);

#if defined(OS_ANDROID)
		static EGLClientBuffer ewrGetNativeClientBufferANDROID(const AHardwareBuffer *buffer);
#endif

#ifdef MUPENPLUSAPI
		//Vid_ext functions
		static m64p_error CoreVideo_Init();
		static void CoreVideo_Quit();
		static m64p_error CoreVideo_SetVideoMode(int screenWidth, int screenHeight, int bitsPerPixel, m64p_video_mode mode, m64p_video_flags flags);
		static void CoreVideo_GL_SetAttribute(m64p_GLattr attribute, int value);
		static void CoreVideo_GL_GetAttribute(m64p_GLattr attribute, int *value);
		static void CoreVideo_GL_SwapBuffers();
#else
		//Windows GL context functions
		static bool windowsStart();
		static void windowsStop();
		static void windowsSwapBuffers();
#endif

		static void ReduceSwapBuffersQueued();
		static void WaitForSwapBuffersQueued();

		static int getTextureBytes(GLenum format, GLenum type, int width, int height);
	};
}
