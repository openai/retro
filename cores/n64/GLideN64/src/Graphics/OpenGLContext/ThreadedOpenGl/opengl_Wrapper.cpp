#include "opengl_Wrapper.h"
#include "opengl_WrappedFunctions.h"
#include "Graphics/OpenGLContext/GLFunctions.h"
#include <memory>
#include <set>

namespace opengl {

	bool FunctionWrapper::m_threaded_wrapper = false;
	bool FunctionWrapper::m_shutdown = false;
	int FunctionWrapper::m_swapBuffersQueued = 0;
	bool FunctionWrapper::m_fastVertexAttributes = false;
	std::thread FunctionWrapper::m_commandExecutionThread;
	std::mutex FunctionWrapper::m_condvarMutex;
	std::condition_variable FunctionWrapper::m_condition;
#if defined(GL_DEBUG) && defined(GL_PROFILE)
	std::map<std::string, FunctionWrapper::FunctionProfilingData> FunctionWrapper::m_functionProfiling;
	std::chrono::time_point<std::chrono::high_resolution_clock> FunctionWrapper::m_lastProfilingOutput;
#endif
	BlockingReaderWriterQueue<std::shared_ptr<OpenGlCommand>> FunctionWrapper::m_commandQueue;
	BlockingReaderWriterQueue<std::shared_ptr<OpenGlCommand>> FunctionWrapper::m_commandQueueHighPriority;


	void FunctionWrapper::executeCommand(std::shared_ptr<OpenGlCommand> _command)
	{
#if !defined(GL_DEBUG)
		m_commandQueue.enqueue(_command);
		_command->waitOnCommand();
#elif !defined(GL_PROFILE)
		_command->performCommandSingleThreaded();
#else
		auto callStartTime = std::chrono::high_resolution_clock::now();
		_command->performCommandSingleThreaded();
		std::chrono::duration<double> callDuration = std::chrono::high_resolution_clock::now() - callStartTime;

		++m_functionProfiling[_command->getFunctionName()].m_callCount;
		m_functionProfiling[_command->getFunctionName()].m_totalTime += callDuration.count();

		logProfilingData();
#endif
	}

	void FunctionWrapper::executePriorityCommand(std::shared_ptr<OpenGlCommand> _command)
	{
#if !defined(GL_DEBUG)
		m_commandQueueHighPriority.enqueue(_command);
		m_commandQueue.enqueue(nullptr);
		_command->waitOnCommand();
#elif !defined(GL_PROFILE)
                _command->performCommandSingleThreaded();
#else
		auto callStartTime = std::chrono::high_resolution_clock::now();
		_command->performCommandSingleThreaded();
		std::chrono::duration<double> callDuration = std::chrono::high_resolution_clock::now() - callStartTime;

		++m_functionProfiling[_command->getFunctionName()].m_callCount;
		m_functionProfiling[_command->getFunctionName()].m_totalTime += callDuration.count();

		logProfilingData();
#endif
	}

	void FunctionWrapper::commandLoop()
	{
		bool timeToShutdown = false;
		while (!timeToShutdown) {
			std::shared_ptr<OpenGlCommand> command;

			if (m_commandQueueHighPriority.peek() != nullptr) {
				while (m_commandQueueHighPriority.try_dequeue(command)) {
					command->performCommand();
				}
			} else {
				if (m_commandQueue.wait_dequeue_timed(command, std::chrono::milliseconds(10)) && command != nullptr) {
					command->performCommand();
					timeToShutdown = command->isTimeToShutdown();
				}
			}
		}
	}

#if defined(GL_DEBUG) && defined(GL_PROFILE)
	void FunctionWrapper::logProfilingData()
	{
		std::chrono::duration<double> timeSinceLastOutput = std::chrono::high_resolution_clock::now() - m_lastProfilingOutput;

		static const double profilingOutputInterval = 10.0;
		if (timeSinceLastOutput.count() > profilingOutputInterval) {

			// Declaring the type of Predicate that accepts 2 pairs and return a bool
			typedef std::function<bool(std::pair<std::string, FunctionProfilingData>, std::pair<std::string, FunctionProfilingData>)> Comparator;

			// Defining a lambda function to compare two pairs. It will compare two pairs using second field
			Comparator compFunctor =
					[](std::pair<std::string, FunctionProfilingData> elem1, std::pair<std::string, FunctionProfilingData> elem2)
					{
						return elem1.second.m_totalTime > elem2.second.m_totalTime;
					};
			std::set<std::pair<std::string, FunctionProfilingData>, Comparator> functionSet(m_functionProfiling.begin(), m_functionProfiling.end(), compFunctor);

			LOG(LOG_ERROR, "Profiling output");
			for ( auto element : functionSet ) {
				std::stringstream output;
				output << element.first << ": call_count=" << element.second.m_callCount
					   << " duration=" << element.second.m_totalTime
					   << " average_per_call=" << element.second.m_totalTime/element.second.m_callCount;
				LOG(LOG_ERROR, output.str().c_str());
			}

			m_functionProfiling.clear();
			m_lastProfilingOutput = std::chrono::high_resolution_clock::now();
		}
	}
#endif

	void FunctionWrapper::setThreadedMode(u32 _threaded)
	{
#ifdef GL_DEBUG
		m_threaded_wrapper = true;
		m_shutdown = false;
#ifdef GL_PROFILE
		m_lastProfilingOutput = std::chrono::high_resolution_clock::now();
#endif
#else
		if (_threaded == 1) {
			m_threaded_wrapper = true;
			m_shutdown = false;
			m_commandExecutionThread = std::thread(&FunctionWrapper::commandLoop);

		}
		else {
			m_threaded_wrapper = false;
			m_shutdown = true;
		}
#endif

	}

	void FunctionWrapper::wrBlendFunc(GLenum sfactor, GLenum dfactor)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBlendFuncCommand::get(sfactor, dfactor));
		else
			ptrBlendFunc(sfactor, dfactor);
	}

	void FunctionWrapper::wrPixelStorei(GLenum pname, GLint param)
	{
		if (m_threaded_wrapper)
			executeCommand(GlPixelStoreiCommand::get(pname, param));
		else
			ptrPixelStorei(pname, param);
	}

	void FunctionWrapper::wrClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		if (m_threaded_wrapper)
			executeCommand(GlClearColorCommand::get(red, green, blue, alpha));
		else
			ptrClearColor(red, green, blue, alpha);
	}

	void FunctionWrapper::wrCullFace(GLenum mode)
	{
		if (m_threaded_wrapper)
			executeCommand(GlCullFaceCommand::get(mode));
		else
			ptrCullFace(mode);
	}

	void FunctionWrapper::wrDepthFunc(GLenum func)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDepthFuncCommand::get(func));
		else
			ptrDepthFunc(func);
	}

	void FunctionWrapper::wrDepthMask(GLboolean flag)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDepthMaskCommand::get(flag));
		else
			ptrDepthMask(flag);
	}

	void FunctionWrapper::wrDisable(GLenum cap)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDisableCommand::get(cap));
		else
			ptrDisable(cap);
	}

	void FunctionWrapper::wrEnable(GLenum cap)
	{
		if (m_threaded_wrapper)
			executeCommand(GlEnableCommand::get(cap));
		else
			ptrEnable(cap);
	}

	void FunctionWrapper::wrDisablei(GLenum target, GLuint index)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDisableiCommand::get(target, index));
		else
			ptrDisablei(target, index);
	}

	void FunctionWrapper::wrEnablei(GLenum target, GLuint index)
	{
		if (m_threaded_wrapper)
			executeCommand(GlEnableiCommand::get(target, index));
		else
			ptrEnablei(target, index);
	}

	void FunctionWrapper::wrPolygonOffset(GLfloat factor, GLfloat units)
	{
		if (m_threaded_wrapper)
			executeCommand(GlPolygonOffsetCommand::get(factor, units));
		else
			ptrPolygonOffset(factor, units);
	}

	void FunctionWrapper::wrScissor(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		if (m_threaded_wrapper)
			executeCommand(GlScissorCommand::get(x, y, width, height));
		else
			ptrScissor(x, y, width, height);
	}

	void FunctionWrapper::wrViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		if (m_threaded_wrapper)
			executeCommand(GlViewportCommand::get(x, y, width, height));
		else
			ptrViewport(x, y, width, height);
	}

	void FunctionWrapper::wrBindTexture(GLenum target, GLuint texture)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBindTextureCommand::get(target, texture));
		else
			ptrBindTexture(target, texture);
	}

	void FunctionWrapper::wrTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
	{
		if (m_threaded_wrapper) {
			int totalBytes = getTextureBytes(format, type, width, height);

			PoolBufferPointer data;
			if (totalBytes > 0 && pixels != nullptr) {
				data = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(pixels), totalBytes);
			}

			if (totalBytes < 0) {
				LOG(LOG_ERROR, "INVALID TEXTURE: format=%d type=%d total=%d", format, type, totalBytes);
			}

			executeCommand(GlTexImage2DCommand::get(target, level, internalformat, width, height, border,
				format, type, data));
		}
		else
			ptrTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	}

	void FunctionWrapper::wrTexParameteri(GLenum target, GLenum pname, GLint param)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTexParameteriCommand::get(target, pname, param));
		else
			ptrTexParameteri(target, pname, param);
	}

	void FunctionWrapper::wrGetIntegerv(GLenum pname, GLint* data)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetIntegervCommand::get(pname, data));
		else
			ptrGetIntegerv(pname, data);
	}

	const GLubyte* FunctionWrapper::wrGetString(GLenum name)
	{
		const GLubyte* returnValue;

		if (m_threaded_wrapper)
			executeCommand(GlGetStringCommand::get(name, returnValue));
		else
			returnValue = ptrGetString(name);

		return returnValue;
	}

	void FunctionWrapper::wrReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
	{
		if (m_threaded_wrapper)
			if (pixels == nullptr) {
				GlReadPixelsAsyncCommand::setBoundBuffer(GlBindBufferCommand::getBoundBuffer(GL_PIXEL_PACK_BUFFER));
				executeCommand(GlReadPixelsAsyncCommand::get(x, y, width, height, format, type));
			} else {
				executeCommand(GlReadPixelsCommand::get(x, y, width, height, format, type, pixels));
			}
		else
			ptrReadPixels(x, y, width, height, format, type, pixels);
	}

	void  FunctionWrapper::wrTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
	{
		if (m_threaded_wrapper) {
			int totalBytes = getTextureBytes(format, type, width, height);

			PoolBufferPointer data;
			if (totalBytes > 0 && pixels != nullptr) {
				data = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(pixels), totalBytes);
			}

			if (totalBytes < 0) {
				LOG(LOG_ERROR, "INVALID TEXTURE: format=%d type=%d total=%d", format, type, totalBytes);
			}

			executeCommand(GlTexSubImage2DUnbufferedCommand::get(target, level, xoffset, yoffset, width, height, format, type, data));
		} else
			ptrTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
	}

	void FunctionWrapper::wrDrawArrays(GLenum mode, GLint first, GLsizei count)
	{
		if (m_threaded_wrapper) {
			if (m_fastVertexAttributes) {
				executeCommand(GlDrawArraysCommand::get(mode, first, count));
			} else {
				const char* ptr = GlVertexAttribPointerManager::getSmallestPtr();
				PoolBufferPointer buffer = OpenGlCommand::m_ringBufferPool.createPoolBuffer(ptr, (count + 1)*GlVertexAttribPointerManager::getStride());
				executeCommand(GlDrawArraysUnbufferedCommand::get(mode, first, count, buffer));
			}
		} else {
			ptrDrawArrays(mode, first, count);
		}
	}

	GLenum FunctionWrapper::wrGetError()
	{
#ifdef GL_DEBUG
		GLenum returnValue;

		if (m_threaded_wrapper)
			executeCommand(GlGetErrorCommand::get(returnValue));
		else
			returnValue = ptrGetError();

		return returnValue;
#else
		return GL_NO_ERROR;
#endif
	}

	void FunctionWrapper::wrDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
	{
		if (!m_threaded_wrapper) {
			ptrDrawElements(mode, count, type, indices);
			return;
		}
		
		int typeSizeBytes;
		unsigned int maxElementIndex;

		switch (type) {
		case GL_UNSIGNED_BYTE:
			typeSizeBytes = 1;
			maxElementIndex = 0;
			for (int index = 0; index < count; ++index) {
				unsigned int value = reinterpret_cast<const uint8_t*>(indices)[index];
				if (maxElementIndex < value) maxElementIndex = value;
			}
			break;
		case GL_UNSIGNED_SHORT:
			typeSizeBytes = 2;
			maxElementIndex = 0;
			for (int index = 0; index < count; ++index) {
				unsigned int value = reinterpret_cast<const uint16_t*>(indices)[index];
				if (maxElementIndex < value) maxElementIndex = value;
			}
			break;
		case GL_UNSIGNED_INT:
			typeSizeBytes = 4;
			maxElementIndex = 0;
			for (int index = 0; index < count; ++index) {
				unsigned int value = reinterpret_cast<const uint32_t*>(indices)[index];
				if (maxElementIndex < value) maxElementIndex = value;
			}
			break;
		default:
			typeSizeBytes = 1;
			maxElementIndex = 0;
			for (int index = 0; index < count; ++index) {
				unsigned int value = reinterpret_cast<const uint8_t*>(indices)[index];
				if (maxElementIndex < value) maxElementIndex = value;
			}
		}

		const char* ptr = GlVertexAttribPointerManager::getSmallestPtr();
		PoolBufferPointer buffer = OpenGlCommand::m_ringBufferPool.createPoolBuffer(ptr, (maxElementIndex + 1)*GlVertexAttribPointerManager::getStride());
		PoolBufferPointer elementsCopy = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(indices), count*typeSizeBytes);

		executeCommand(GlDrawElementsUnbufferedCommand::get(mode, count, type, elementsCopy, buffer));
	}

	void FunctionWrapper::wrLineWidth(GLfloat width)
	{
		if (m_threaded_wrapper)
			executeCommand(GlLineWidthCommand::get(width));
		else
			ptrLineWidth(width);
	}

	void FunctionWrapper::wrClear(GLbitfield mask)
	{
		if (m_threaded_wrapper)
			executeCommand(GlClearCommand::get(mask));
		else
			ptrClear(mask);
	}

	void FunctionWrapper::wrClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value)
	{
		if (!m_threaded_wrapper) {
			ptrClearBufferfv(buffer, drawbuffer, value);
			return;
		}

		int numValues = 0;

		switch (buffer) {
		case GL_COLOR:
			numValues = 4;
			break;
		case GL_DEPTH:
		case GL_STENCIL:
			numValues = 1;
			break;
		default:
			numValues = 1;
		}

		PoolBufferPointer values = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(value), numValues*sizeof(GLfloat));
		executeCommand(GlClearBufferfvCommand::get(buffer, drawbuffer, values));
	}

	void FunctionWrapper::wrGetFloatv(GLenum pname, GLfloat* data)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlGetFloatvCommand::get(pname, data));
		else
			ptrGetFloatv(pname, data);
	}

	void FunctionWrapper::wrDeleteTextures(GLsizei n, const GLuint *textures)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer texture = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(textures), n*sizeof(GLuint));
			executeCommand(GlDeleteTexturesCommand::get(n, texture));
		}
		else
			ptrDeleteTextures(n, textures);
	}

	void FunctionWrapper::wrGenTextures(GLsizei n, GLuint* textures)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlGenTexturesCommand::get(n, textures));
		else
			ptrGenTextures(n, textures);
	}

	void FunctionWrapper::wrTexParameterf(GLenum target, GLenum pname, GLfloat param)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTexParameterfCommand::get(target, pname, param));
		else
			ptrTexParameterf(target, pname, param);
	}

	void FunctionWrapper::wrActiveTexture(GLenum texture)
	{
		if (m_threaded_wrapper)
			executeCommand(GlActiveTextureCommand::get(texture));
		else
			ptrActiveTexture(texture);
	}

	void FunctionWrapper::wrBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBlendColorCommand::get(red, green, blue, alpha));
		else
			ptrBlendColor(red, green, blue, alpha);
	}

	void FunctionWrapper::wrReadBuffer(GLenum src)
	{
		if (m_threaded_wrapper)
			executeCommand(GlReadBufferCommand::get(src));
		else
			ptrReadBuffer(src);
	}

	GLuint FunctionWrapper::wrCreateShader(GLenum type)
	{
		GLuint returnValue;

		if (m_threaded_wrapper)
			executeCommand(GlCreateShaderCommand::get(type, returnValue));
		else
			returnValue = ptrCreateShader(type);

		return returnValue;
	}

	void FunctionWrapper::wrCompileShader(GLuint shader)
	{
		if (m_threaded_wrapper)
			executeCommand(GlCompileShaderCommand::get(shader));
		else
			ptrCompileShader(shader);
	}

	void FunctionWrapper::wrShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
	{
		if (m_threaded_wrapper) {
			std::vector<std::string> stringData(count);

			if (length == nullptr) {
				for (unsigned int index = 0; index < stringData.size(); ++index) {
					stringData[index] = string[index];
				}
			} else {
				for (unsigned int index = 0; index < stringData.size(); ++index) {
					std::string tempString;
					tempString.assign(string[index], length[index]);
					stringData[index] = tempString;
				}
			}

			executeCommand(GlShaderSourceCommand::get(shader, stringData));
		} else
			ptrShaderSource(shader, count, string, length);
	}

	GLuint FunctionWrapper::wrCreateProgram()
	{
		GLuint returnValue;

		if (m_threaded_wrapper)
			executeCommand(GlCreateProgramCommand::get(returnValue));
		else
			returnValue = ptrCreateProgram();

		return returnValue;
	}

	void FunctionWrapper::wrAttachShader(GLuint program, GLuint shader)
	{
		if (m_threaded_wrapper)
			executeCommand(GlAttachShaderCommand::get(program, shader));
		else
			ptrAttachShader(program, shader);
	}

	void FunctionWrapper::wrLinkProgram(GLuint program)
	{
		if (m_threaded_wrapper)
			executeCommand(GlLinkProgramCommand::get(program));
		else
			ptrLinkProgram(program);
	}

	void FunctionWrapper::wrUseProgram(GLuint program)
	{
		if (m_threaded_wrapper)
			executeCommand(GlUseProgramCommand::get(program));
		else
			ptrUseProgram(program);
	}

	GLint FunctionWrapper::wrGetUniformLocation(GLuint program, const GLchar *name)
	{
		GLint returnValue;

		if (m_threaded_wrapper)
			executeCommand(GlGetUniformLocationCommand::get(program, name, returnValue));
		else
			returnValue = ptrGetUniformLocation(program, name);

		return returnValue;
	}

	void FunctionWrapper::wrUniform1i(GLint location, GLint v0)
	{
		if (m_threaded_wrapper)
			executeCommand(GlUniform1iCommand::get(location, v0));
		else
			ptrUniform1i(location, v0);
	}

	void FunctionWrapper::wrUniform1f(GLint location, GLfloat v0)
	{
		if (m_threaded_wrapper)
			executeCommand(GlUniform1fCommand::get(location, v0));
		else
			ptrUniform1f(location, v0);
	}

	void FunctionWrapper::wrUniform2f(GLint location, GLfloat v0, GLfloat v1)
	{
		if (m_threaded_wrapper)
			executeCommand(GlUniform2fCommand::get(location, v0, v1));
		else
			ptrUniform2f(location, v0, v1);
	}

	void FunctionWrapper::wrUniform2i(GLint location, GLint v0, GLint v1)
	{
		if (m_threaded_wrapper)
			executeCommand(GlUniform2iCommand::get(location, v0, v1));
		else
			ptrUniform2i(location, v0, v1);
	}

	void FunctionWrapper::wrUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
	{
		if (m_threaded_wrapper)
			executeCommand(GlUniform4iCommand::get(location, v0, v1, v2, v3));
		else
			ptrUniform4i(location, v0, v1, v2, v3);
	}


	void FunctionWrapper::wrUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		if (m_threaded_wrapper)
			executeCommand(GlUniform4fCommand::get(location, v0, v1, v2, v3));
		else
			ptrUniform4f(location, v0, v1, v2, v3);
	}

	void FunctionWrapper::wrUniform3fv(GLint location, GLsizei count, const GLfloat *value)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer values = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(value), 3 * sizeof(GLfloat) * count);
			executeCommand(GlUniform3fvCommand::get(location, count, values));
		} else
			ptrUniform3fv(location, count, value);
	}

	void FunctionWrapper::wrUniform4fv(GLint location, GLsizei count, const GLfloat *value)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer values = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(value), 4 * sizeof(GLfloat) * count);
			executeCommand(GlUniform4fvCommand::get(location, count, values));
		} else
			ptrUniform4fv(location, count, value);
	}

	void FunctionWrapper::wrDetachShader(GLuint program, GLuint shader)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDetachShaderCommand::get(program, shader));
		else
			ptrDetachShader(program, shader);
	}

	void FunctionWrapper::wrDeleteShader(GLuint shader)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDeleteShaderCommand::get(shader));
		else
			ptrDeleteShader(shader);
	}

	void FunctionWrapper::wrDeleteProgram(GLuint program)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDeleteProgramCommand::get(program));
		else
			ptrDeleteProgram(program);
	}

	void FunctionWrapper::wrGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar *infoLog)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetProgramInfoLogCommand::get(program, bufSize, length, infoLog));
		else
			ptrGetProgramInfoLog(program, bufSize, length, infoLog);
	}

	void FunctionWrapper::wrGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar *infoLog)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetShaderInfoLogCommand::get(shader, bufSize, length, infoLog));
		else
			ptrGetShaderInfoLog(shader, bufSize, length, infoLog);
	}

	void FunctionWrapper::wrGetShaderiv(GLuint shader, GLenum pname, GLint* params)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetShaderivCommand::get(shader, pname, params));
		else
			ptrGetShaderiv(shader, pname, params);
	}

	void FunctionWrapper::wrGetProgramiv(GLuint program, GLenum pname, GLint* params)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetProgramivCommand::get(program, pname, params));
		else
			ptrGetProgramiv(program, pname, params);
	}

	void FunctionWrapper::wrEnableVertexAttribArray(GLuint index)
	{
		if (m_threaded_wrapper) {
			GlVertexAttribPointerManager::enableVertexAttributeIndex(index);
			executeCommand(GlEnableVertexAttribArrayCommand::get(index));
		}
		else
			ptrEnableVertexAttribArray(index);
	}

	void FunctionWrapper::wrDisableVertexAttribArray(GLuint index)
	{
		if (m_threaded_wrapper) {
			GlVertexAttribPointerManager::disableVertexAttributeIndex(index);
			executeCommand(GlDisableVertexAttribArrayCommand::get(index));
		} else
			ptrDisableVertexAttribArray(index);
	}

	void FunctionWrapper::wrVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
	{
		if (m_threaded_wrapper) {
			if (m_fastVertexAttributes) {
				executeCommand(GlVertexAttribPointerBufferedCommand::get(index, size, type, normalized, stride, pointer));
			} else {
				GlVertexAttribPointerManager::update(index, size, type, normalized, stride, pointer);
				executeCommand(GlVertexAttribPointerUnbufferedCommand::get(index, size, type, normalized, stride, pointer));
			}
		}
		else
			ptrVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}

	void FunctionWrapper::wrBindAttribLocation(GLuint program, GLuint index, const GLchar *name)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBindAttribLocationCommand::get(program, index, name));
		else
			ptrBindAttribLocation(program, index, name);
	}

	void FunctionWrapper::wrVertexAttrib1f(GLuint index, GLfloat x)
	{
		if (m_threaded_wrapper)
			executeCommand(GlVertexAttrib1fCommand::get(index, x));
		else
			ptrVertexAttrib1f(index, x);
	}

	void FunctionWrapper::wrVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		if (m_threaded_wrapper)
			executeCommand(GlVertexAttrib4fCommand::get(index, x, y, z, w));
		else
			ptrVertexAttrib4f(index, x, y, z, w);
	}

	void FunctionWrapper::wrVertexAttrib4fv(GLuint index, const GLfloat *v)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer values = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(v), 4 * sizeof(GLfloat));
			executeCommand(GlVertexAttrib4fvCommand::get(index, values));
		} else
			ptrVertexAttrib4fv(index, v);
	}

	void FunctionWrapper::wrDepthRangef(GLfloat n, GLfloat f)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDepthRangefCommand::get(n, f));
		else
			ptrDepthRangef(n, f);
	}

	void FunctionWrapper::wrClearDepthf(GLfloat d)
	{
		if (m_threaded_wrapper)
			executeCommand(GlClearDepthfCommand::get(d));
		else
			ptrClearDepthf(d);
	}

	void FunctionWrapper::wrDrawBuffers(GLsizei n, const GLenum *bufs)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer bufsPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(bufs), n*sizeof(GLenum));
			executeCommand(GlDrawBuffersCommand::get(n, bufsPtr));
		} else
			ptrDrawBuffers(n, bufs);
	}

	void FunctionWrapper::wrGenFramebuffers(GLsizei n, GLuint* framebuffers)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlGenFramebuffersCommand::get(n, framebuffers));
		else
			ptrGenFramebuffers(n, framebuffers);
	}

	void FunctionWrapper::wrBindFramebuffer(GLenum target, GLuint framebuffer)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBindFramebufferCommand::get(target, framebuffer));
		else
			ptrBindFramebuffer(target, framebuffer);
	}

	void FunctionWrapper::wrDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer framebuffersPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(framebuffers), n*sizeof(GLuint));
			executeCommand(GlDeleteFramebuffersCommand::get(n, framebuffersPtr));
		} else
			ptrDeleteFramebuffers(n, framebuffers);
	}

	void FunctionWrapper::wrFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
	{
		if (m_threaded_wrapper)
			executeCommand(GlFramebufferTexture2DCommand::get(target, attachment, textarget, texture, level));
		else
			ptrFramebufferTexture2D(target, attachment, textarget, texture, level);
	}

	void FunctionWrapper::wrTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTexImage2DMultisampleCommand::get(target, samples, internalformat, width, height, fixedsamplelocations));
		else
			ptrTexImage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
	}

	void FunctionWrapper::wrTexStorage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTexStorage2DMultisampleCommand::get(target, samples, internalformat, width, height, fixedsamplelocations));
		else
			ptrTexStorage2DMultisample(target, samples, internalformat, width, height, fixedsamplelocations);
	}

	void FunctionWrapper::wrGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlGenRenderbuffersCommand::get(n, renderbuffers));
		else
			ptrGenRenderbuffers(n, renderbuffers);
	}

	void FunctionWrapper::wrBindRenderbuffer(GLenum target, GLuint renderbuffer)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBindRenderbufferCommand::get(target, renderbuffer));
		else
			ptrBindRenderbuffer(target, renderbuffer);
	}

	void FunctionWrapper::wrRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
	{
		if (m_threaded_wrapper)
			executeCommand(GlRenderbufferStorageCommand::get(target, internalformat, width, height));
		else
			ptrRenderbufferStorage(target, internalformat, width, height);
	}

	void FunctionWrapper::wrDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer renderbuffersPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(renderbuffers), n*sizeof(GLuint));
			executeCommand(GlDeleteRenderbuffersCommand::get(n, renderbuffersPtr));
		} else
			ptrDeleteRenderbuffers(n, renderbuffers);
	}

	void FunctionWrapper::wrFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
	{
		if (m_threaded_wrapper)
			executeCommand(GlFramebufferRenderbufferCommand::get(target, attachment, renderbuffertarget, renderbuffer));
		else
			ptrFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	}

	GLenum FunctionWrapper::wrCheckFramebufferStatus(GLenum target)
	{
#ifdef GL_DEBUG
		GLenum returnValue;

		if (m_threaded_wrapper)
			executeCommand(GlCheckFramebufferStatusCommand::get(target, returnValue));
		else
			returnValue = ptrCheckFramebufferStatus(target);

		return returnValue;
#else
		return GL_FRAMEBUFFER_COMPLETE;
#endif
	}

	void FunctionWrapper::wrBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBlitFramebufferCommand::get(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
		else
			ptrBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
	}

	void FunctionWrapper::wrGenVertexArrays(GLsizei n, GLuint* arrays)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlGenVertexArraysCommand::get(n, arrays));
		else
			ptrGenVertexArrays(n, arrays);
	}

	void FunctionWrapper::wrBindVertexArray(GLuint array)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBindVertexArrayCommand::get(array));
		else
			ptrBindVertexArray(array);
	}

	void FunctionWrapper::wrDeleteVertexArrays(GLsizei n, const GLuint *arrays)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer arraysPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(arrays), n*sizeof(GLuint));
			executeCommand(GlDeleteVertexArraysCommand::get(n, arraysPtr));
		} else
			ptrDeleteVertexArrays(n, arrays);
	}

	void FunctionWrapper::wrGenBuffers(GLsizei n, GLuint* buffers)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlGenBuffersCommand::get(n, buffers));
		else
			ptrGenBuffers(n, buffers);
	}

	void FunctionWrapper::wrBindBuffer(GLenum target, GLuint buffer)
	{
		if (m_threaded_wrapper) {
			GlBindBufferCommand::setBoundBuffer(target, buffer);
			executeCommand(GlBindBufferCommand::get(target, buffer));
		}
		else
			ptrBindBuffer(target, buffer);
	}

	void  FunctionWrapper::wrBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
	{
		if (m_threaded_wrapper) {
			if (target == GL_ARRAY_BUFFER || target == GL_ELEMENT_ARRAY_BUFFER)
				m_fastVertexAttributes = true;

			PoolBufferPointer dataPtr;
			if (data != nullptr) {
				dataPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(data), size);
			}
			executeCommand(GlBufferDataCommand::get(target, size, dataPtr, usage));
		}
		else
			ptrBufferData(target, size, data, usage);
	}

	void FunctionWrapper::wrMapBuffer(GLenum target, GLenum access)
	{
		if (m_threaded_wrapper)
			executeCommand(GlMapBufferCommand::get(target, access));
		else
			ptrMapBuffer(target, access);
	}

	void* FunctionWrapper::wrMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
	{
		void* returnValue;

		if (m_threaded_wrapper) {
			GLuint boundPixelBuffer = GlReadPixelsAsyncCommand::getBoundBuffer();
			if (target == GL_PIXEL_PACK_BUFFER && access == GL_MAP_READ_BIT &&
				GlBindBufferCommand::getBoundBuffer(GL_PIXEL_PACK_BUFFER) != boundPixelBuffer) {
				executeCommand(GlMapBufferRangeReadAsyncCommand::get(target, offset, length, access));
				GLuint buffer = GlBindBufferCommand::getBoundBuffer(target);
				returnValue = GlMapBufferRangeReadAsyncCommand::getData(buffer, length)->data();
			}
			else if (access == GlMapBufferRangeWriteAsyncCommand::WRITE_ACCESS_ASYNC) {
				GlMapBufferRangeWriteAsyncCommand::setTemp(target, offset, length, access);
				returnValue = GlMapBufferRangeWriteAsyncCommand::getData(target, length)->data();
			}
			else {
				executeCommand(GlMapBufferRangeCommand::get(target, offset, length, access, returnValue));
			}
		}
		else {
			returnValue = ptrMapBufferRange(target, offset, length, access);
		}

		return returnValue;
	}

	GLboolean FunctionWrapper::wrUnmapBuffer(GLenum target)
	{
		GLboolean returnValue = GL_TRUE;

		if (m_threaded_wrapper) {

			GLbitfield accessTemp = GlMapBufferRangeWriteAsyncCommand::getTempAccess();
			GLenum targetTemp = GlMapBufferRangeWriteAsyncCommand::getTempTarget();

			if (accessTemp == GlMapBufferRangeWriteAsyncCommand::WRITE_ACCESS_ASYNC &&
				target == targetTemp) {
				GLintptr offsetTemp = GlMapBufferRangeWriteAsyncCommand::getTempOffset();
				GLsizeiptr lengthTemp = GlMapBufferRangeWriteAsyncCommand::getTempLength();
				auto dataToCopy = GlMapBufferRangeWriteAsyncCommand::getData(targetTemp, lengthTemp);

				PoolBufferPointer buffer = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<char*>(dataToCopy->data()), dataToCopy->size());
				executeCommand(GlMapBufferRangeWriteAsyncCommand::get(targetTemp, offsetTemp, lengthTemp, accessTemp, buffer));
				GlMapBufferRangeWriteAsyncCommand::setTemp(0, 0, 0, 0);
			}

			executeCommand(GlUnmapBufferAsyncCommand::get(target));
		} else
			returnValue = ptrUnmapBuffer(target);

		return returnValue;
	}

	void FunctionWrapper::wrDeleteBuffers(GLsizei n, const GLuint *buffers)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer buffersPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(buffers), n*sizeof(GLuint));
			executeCommand(GlDeleteBuffersCommand::get(n, buffersPtr));
		}
		else
			ptrDeleteBuffers(n, buffers);
	}

	void FunctionWrapper::wrBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBindImageTextureCommand::get(unit, texture, level, layered, layer, access, format));
		else
			ptrBindImageTexture(unit, texture, level, layered, layer, access, format);
	}

	void FunctionWrapper::wrMemoryBarrier(GLbitfield barriers)
	{
		if (m_threaded_wrapper)
			executeCommand(GlMemoryBarrierCommand::get(barriers));
		else
			ptrMemoryBarrier(barriers);
	}

	void FunctionWrapper::wrTextureBarrier()
	{
		if (m_threaded_wrapper)
			executeCommand(GlTextureBarrierCommand::get());
		else
			ptrTextureBarrier();
	}

	void FunctionWrapper::wrTextureBarrierNV()
	{
		if (m_threaded_wrapper)
			executeCommand(GlTextureBarrierNVCommand::get());
		else
			ptrTextureBarrierNV();
	}

	const GLubyte* FunctionWrapper::wrGetStringi(GLenum name, GLuint index)
	{
		const GLubyte* returnValue;

		if (m_threaded_wrapper)
			executePriorityCommand(GlGetStringiCommand::get(name, index, returnValue));
		else
			returnValue = ptrGetStringi(name, index);

		return returnValue;
	}

	void FunctionWrapper::wrInvalidateFramebuffer(GLenum target, GLsizei numAttachments, const GLenum *attachments)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer attachmentsPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(attachments), numAttachments*sizeof(GLenum));
			executeCommand(GlInvalidateFramebufferCommand::get(target, numAttachments, attachmentsPtr));
		} else
			ptrInvalidateFramebuffer(target, numAttachments, attachments);
	}

	void  FunctionWrapper::wrBufferStorage(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags)
	{
		if (m_threaded_wrapper) {
			if (target == GL_ARRAY_BUFFER || target == GL_ELEMENT_ARRAY_BUFFER)
				m_fastVertexAttributes = true;

			PoolBufferPointer dataPtr;
			if (data != nullptr) {
				dataPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(data), size);
			}

			executeCommand(GlBufferStorageCommand::get(target, size, dataPtr, flags));
		} else
			ptrBufferStorage(target, size, data, flags);
	}

	GLsync FunctionWrapper::wrFenceSync(GLenum condition, GLbitfield flags)
	{
		GLsync returnValue;

		if (m_threaded_wrapper)
			executePriorityCommand(GlFenceSyncCommand::get(condition, flags, returnValue));
		else
			returnValue = ptrFenceSync(condition, flags);

		return returnValue;
	}

	void FunctionWrapper::wrClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlClientWaitSyncCommand::get(sync, flags, timeout));
		else
			ptrClientWaitSync(sync, flags, timeout);
	}

	void FunctionWrapper::wrDeleteSync(GLsync sync)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDeleteSyncCommand::get(sync));
		else
			ptrDeleteSync(sync);
	}

	GLuint FunctionWrapper::wrGetUniformBlockIndex(GLuint program, GLchar *uniformBlockName)
	{
		GLuint returnValue;

		if (m_threaded_wrapper)
			executeCommand(GlGetUniformBlockIndexCommand::get(program, uniformBlockName, returnValue));
		else
			returnValue = ptrGetUniformBlockIndex(program, uniformBlockName);

		return returnValue;
	}

	void FunctionWrapper::wrUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
	{
		if (m_threaded_wrapper)
			executeCommand(GlUniformBlockBindingCommand::get(program, uniformBlockIndex, uniformBlockBinding));
		else
			ptrUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
	}

	void FunctionWrapper::wrGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetActiveUniformBlockivCommand::get(program, uniformBlockIndex, pname, params));
		else
			ptrGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
	}

	void FunctionWrapper::wrGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint* uniformIndices)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetUniformIndicesCommand::get(program, uniformCount, uniformNames, uniformIndices));
		else
			ptrGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
	}

	void FunctionWrapper::wrGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint* params)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetActiveUniformsivCommand::get(program, uniformCount, uniformIndices, pname, params));
		else
			ptrGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
	}

	void FunctionWrapper::wrBindBufferBase(GLenum target, GLuint index, GLuint buffer)
	{
		if (m_threaded_wrapper)
			executeCommand(GlBindBufferBaseCommand::get(target, index, buffer));
		else
			ptrBindBufferBase(target, index, buffer);
	}

	void  FunctionWrapper::wrBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer dataPtr;
			if (data != nullptr) {
				dataPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(data), size);
			}

			executeCommand(GlBufferSubDataCommand::get(target, offset, size, dataPtr));
		} else
			ptrBufferSubData(target, offset, size, data);
	}

	void FunctionWrapper::wrGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void *binary)
	{
		if (m_threaded_wrapper)
			executeCommand(GlGetProgramBinaryCommand::get(program, bufSize, length, binaryFormat, binary));
		else
			ptrGetProgramBinary(program, bufSize, length, binaryFormat, binary);
	}

	void  FunctionWrapper::wrProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer binaryPtr = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(binary), length);
			executeCommand(GlProgramBinaryCommand::get(program, binaryFormat, binaryPtr, length));
		} else
			ptrProgramBinary(program, binaryFormat, binary, length);
	}

	void FunctionWrapper::wrProgramParameteri(GLuint program, GLenum pname, GLint value)
	{
		if (m_threaded_wrapper)
			executeCommand(GlProgramParameteriCommand::get(program, pname, value));
		else
			ptrProgramParameteri(program, pname, value);
	}

	void FunctionWrapper::wrTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTexStorage2DCommand::get(target, levels, internalformat, width, height));
		else
			ptrTexStorage2D(target, levels, internalformat, width, height);
	}

	void FunctionWrapper::wrTextureStorage2D(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTextureStorage2DCommand::get(texture, levels, internalformat, width, height));
		else
			ptrTextureStorage2D(texture, levels, internalformat, width, height);
	}

	void  FunctionWrapper::wrTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
	{
		if (m_threaded_wrapper) {
			PoolBufferPointer data;
			int totalBytes = getTextureBytes(format, type, width, height);
			if (totalBytes > 0 && pixels != nullptr) {
				data = OpenGlCommand::m_ringBufferPool.createPoolBuffer(reinterpret_cast<const char*>(pixels), totalBytes);
			}

			if (totalBytes < 0) {
				LOG(LOG_ERROR, "INVALID TEXTURE: format=%d type=%d total=%d", format, type, totalBytes);
			}

			executeCommand(GlTextureSubImage2DUnbufferedCommand::get(texture, level, xoffset, yoffset,
				width, height, format, type, data));
		} else
			ptrTextureSubImage2D(texture, level, xoffset, yoffset, width, height, format, type, pixels);
	}

	void FunctionWrapper::wrTextureStorage2DMultisample(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTextureStorage2DMultisampleCommand::get(texture, target, samples, internalformat, width, height, fixedsamplelocations));
		else
			ptrTextureStorage2DMultisample(texture, target, samples, internalformat, width, height, fixedsamplelocations);
	}

	void FunctionWrapper::wrTextureParameteri(GLuint texture, GLenum pname, GLint param)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTextureParameteriCommand::get(texture, pname, param));
		else
			ptrTextureParameteri(texture, pname, param);
	}

	void FunctionWrapper::wrTextureParameterf(GLuint texture, GLenum pname, GLfloat param)
	{
		if (m_threaded_wrapper)
			executeCommand(GlTextureParameterfCommand::get(texture, pname, param));
		else
			ptrTextureParameterf(texture, pname, param);
	}

	void FunctionWrapper::wrCreateTextures(GLenum target, GLsizei n, GLuint* textures)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlCreateTexturesCommand::get(target, n, textures));
		else
			ptrCreateTextures(target, n, textures);
	}

	void FunctionWrapper::wrCreateBuffers(GLsizei n, GLuint* buffers)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlCreateBuffersCommand::get(n, buffers));
		else
			ptrCreateBuffers(n, buffers);
	}

	void FunctionWrapper::wrCreateFramebuffers(GLsizei n, GLuint* framebuffers)
	{
		if (m_threaded_wrapper)
			executePriorityCommand(GlCreateFramebuffersCommand::get(n, framebuffers));
		else
			ptrCreateFramebuffers(n, framebuffers);
	}

	void FunctionWrapper::wrNamedFramebufferTexture(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)
	{
		if (m_threaded_wrapper)
			executeCommand(GlNamedFramebufferTextureCommand::get(framebuffer, attachment, texture, level));
		else
			ptrNamedFramebufferTexture(framebuffer, attachment, texture, level);
	}

	void FunctionWrapper::wrDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,
		const u16* indices, GLint basevertex)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDrawRangeElementsBaseVertexCommand::get(mode, start, end, count, type, indices, basevertex));
		else
			ptrDrawRangeElementsBaseVertex(mode, start, end, count, type, indices, basevertex);
	}

	void FunctionWrapper::wrFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
	{
		if (m_threaded_wrapper)
			executeCommand(GlFlushMappedBufferRangeCommand::get(target, offset, length));
		else
			ptrFlushMappedBufferRange(target, offset, length);
	}

	void FunctionWrapper::wrFinish()
	{
		if (m_threaded_wrapper)
			executeCommand(GlFinishCommand::get());
		else
			ptrFinish();
	}

	void FunctionWrapper::wrCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
	{
		if (m_threaded_wrapper)
			executeCommand(GlCopyTexImage2DCommand::get(target, level, internalformat, x, y, width, height, border));
		else
			ptrCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	}

	void FunctionWrapper::wrDebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDebugMessageCallbackCommand::get(callback, userParam));
		else
			ptrDebugMessageCallback(callback, userParam);
	}

	void FunctionWrapper::wrDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled)
	{
		if (m_threaded_wrapper)
			executeCommand(GlDebugMessageControlCommand::get(source, type, severity, count, ids, enabled));
		else
			ptrDebugMessageControl(source, type, severity, count, ids, enabled);
	}

	void FunctionWrapper::wrEGLImageTargetTexture2DOES(GLenum target, void* image)
	{
		if (m_threaded_wrapper)
			executeCommand(GlEGLImageTargetTexture2DOESCommand::get(target, image));
		else
			ptrEGLImageTargetTexture2DOES(target, image);
	}

	void FunctionWrapper::wrEGLImageTargetRenderbufferStorageOES(GLenum target, void* image)
	{
		if (m_threaded_wrapper)
			executeCommand(GlEGLImageTargetRenderbufferStorageOESCommand::get(target, image));
		else
			ptrEGLImageTargetRenderbufferStorageOES(target, image);
	}

#if defined(OS_ANDROID)
	EGLClientBuffer FunctionWrapper::ewrGetNativeClientBufferANDROID(const AHardwareBuffer *buffer)
	{
		EGLClientBuffer returnValue;

		if (m_threaded_wrapper)
			executeCommand(EglGetNativeClientBufferANDROIDCommand::get(buffer, returnValue));
		else
			returnValue = ptrGetNativeClientBufferANDROID(buffer);

		return returnValue;
	}

#endif

#ifdef MUPENPLUSAPI

	m64p_error FunctionWrapper::CoreVideo_Init()
	{
		m64p_error returnValue;
		if (m_threaded_wrapper)
			executeCommand(CoreVideoInitCommand::get(returnValue));
		else
			CoreVideoInitCommand::get(returnValue)->performCommandSingleThreaded();
		return returnValue;
	}

	void FunctionWrapper::CoreVideo_Quit()
	{
		if (m_threaded_wrapper) {
			executeCommand(CoreVideoQuitCommand::get());
			executeCommand(ShutdownCommand::get());
		} 
		else
			CoreVideoQuitCommand::get()->performCommandSingleThreaded();

		m_shutdown = true;

#ifndef GL_DEBUG
		if (m_threaded_wrapper) {
			m_condition.notify_all();
			m_commandExecutionThread.join();
		}
#endif
	}

	m64p_error FunctionWrapper::CoreVideo_SetVideoMode(int screenWidth, int screenHeight, int bitsPerPixel, m64p_video_mode mode, m64p_video_flags flags)
	{
		m64p_error returnValue;

		if (m_threaded_wrapper)
			executeCommand(CoreVideoSetVideoModeCommand::get(screenWidth, screenHeight, bitsPerPixel, mode, flags, returnValue));
		else
			CoreVideoSetVideoModeCommand::get(screenWidth, screenHeight, bitsPerPixel, mode, flags, returnValue)->performCommandSingleThreaded();

		return returnValue;
	}

	void FunctionWrapper::CoreVideo_GL_SetAttribute(m64p_GLattr attribute, int value)
	{
		if (m_threaded_wrapper)
			executeCommand(CoreVideoGLSetAttributeCommand::get(attribute, value));
		else
			CoreVideoGLSetAttributeCommand::get(attribute, value)->performCommandSingleThreaded();
	}

	void FunctionWrapper::CoreVideo_GL_GetAttribute(m64p_GLattr attribute, int *value)
	{
		if (m_threaded_wrapper)
			executeCommand(CoreVideoGLGetAttributeCommand::get(attribute, value));
		else
			CoreVideoGLGetAttributeCommand::get(attribute, value)->performCommandSingleThreaded();
	}

	void FunctionWrapper::CoreVideo_GL_SwapBuffers()
	{
		++m_swapBuffersQueued;

		if (m_threaded_wrapper)
			executeCommand(CoreVideoGLSwapBuffersCommand::get([]{ReduceSwapBuffersQueued();}));
		else
			CoreVideoGLSwapBuffersCommand::get([]{ReduceSwapBuffersQueued();})->performCommandSingleThreaded();

	}
#else
	bool FunctionWrapper::windowsStart()
	{
		bool returnValue;

		if (m_threaded_wrapper)
			executeCommand(WindowsStartCommand::get(returnValue));
		else
			WindowsStartCommand::get(returnValue)->performCommandSingleThreaded();

		return returnValue;
	}

	void FunctionWrapper::windowsStop()
	{
		if (m_threaded_wrapper) {
			executeCommand(WindowsStopCommand::get());
			executeCommand(ShutdownCommand::get());
		} else
			WindowsStopCommand::get()->performCommandSingleThreaded();

		m_shutdown = true;

#ifndef GL_DEBUG
		if (m_threaded_wrapper) {
			m_condition.notify_all();
			m_commandExecutionThread.join();
		}
#endif
	}

	void FunctionWrapper::windowsSwapBuffers()
	{
		++m_swapBuffersQueued;

		if (m_threaded_wrapper)
			executeCommand(WindowsSwapBuffersCommand::get([]{ReduceSwapBuffersQueued(); }));
		else
			WindowsSwapBuffersCommand::get([]{ReduceSwapBuffersQueued(); })->performCommandSingleThreaded();
	}

#endif

	void FunctionWrapper::ReduceSwapBuffersQueued()
	{
		--m_swapBuffersQueued;

		if (m_swapBuffersQueued <= MAX_SWAP) {
			m_condition.notify_all();
		}
	}

	void FunctionWrapper::WaitForSwapBuffersQueued()
	{
		std::unique_lock<std::mutex> lock(m_condvarMutex);

		if (!m_shutdown && m_swapBuffersQueued > MAX_SWAP) {
			m_condition.wait(lock, []{return FunctionWrapper::m_swapBuffersQueued <= MAX_SWAP; });
		}
	}


	int FunctionWrapper::getTextureBytes(GLenum format, GLenum type, int width, int height)
	{
		int components = 0;
		int bytesPerPixel = 0;

		switch (format)
		{
		case GL_RED:
			components = 1;
			break;
		case GL_RG:
			components = 2;
			break;
		case GL_RGB:
		case GL_BGR:
			components = 3;
			break;
		case GL_RGBA:
		case GL_BGRA:
			components = 4;
			break;
		case GL_RED_INTEGER:
			components = 1;
			break;
		case GL_RG_INTEGER:
			components = 2;
			break;
		case GL_RGB_INTEGER:
		case GL_BGR_INTEGER:
			components = 3;
			break;
		case GL_RGBA_INTEGER:
		case GL_BGRA_INTEGER:
			components = 4;
			break;
		case GL_STENCIL_INDEX:
		case GL_DEPTH_COMPONENT:
			components = 1;
			break;
		case GL_DEPTH_STENCIL:
			components = 2;
			break;
		case GL_LUMINANCE:
			components = 1;
			break;
		default:
			components = -1;
		}

		switch (type)
		{
		case GL_UNSIGNED_BYTE:
		case GL_BYTE:
			bytesPerPixel = components * 1;
			break;
		case GL_UNSIGNED_SHORT:
		case GL_SHORT:
			bytesPerPixel = components * 2;
			break;
		case GL_UNSIGNED_INT:
		case GL_INT:
			bytesPerPixel = components * 4;
			break;
		case GL_HALF_FLOAT:
			bytesPerPixel = components * 2;
			break;
		case GL_FLOAT:
			bytesPerPixel = components * 4;
			break;
		case GL_UNSIGNED_BYTE_3_3_2:
		case GL_UNSIGNED_BYTE_2_3_3_REV:
			bytesPerPixel = 1;
			break;
		case GL_UNSIGNED_SHORT_5_6_5:
		case GL_UNSIGNED_SHORT_5_6_5_REV:
		case GL_UNSIGNED_SHORT_4_4_4_4:
		case GL_UNSIGNED_SHORT_4_4_4_4_REV:
		case GL_UNSIGNED_SHORT_5_5_5_1:
		case GL_UNSIGNED_SHORT_1_5_5_5_REV:
			bytesPerPixel = 2;
			break;
		case GL_UNSIGNED_INT_8_8_8_8:
		case GL_UNSIGNED_INT_8_8_8_8_REV:
		case GL_UNSIGNED_INT_10_10_10_2:
		case GL_UNSIGNED_INT_2_10_10_10_REV:
			bytesPerPixel = 4;
			break;
		default:
			bytesPerPixel = -1;
		}

		int textureSize = bytesPerPixel*width*height;

		if (textureSize < 0)
		{
			std::stringstream log;
			log << "Invalid texture params: component=" << std::dec << components
				<< " bytesPerPixel=" << bytesPerPixel
				<< " format=0x" << std::hex << format
				<< " type=0x" << type
				<< " width=" << std::dec << width
				<< " height=" << height
				<< " size=" << bytesPerPixel*width*height;
			LOG(LOG_ERROR, log.str().c_str());
		}

		return textureSize;
	}
}
