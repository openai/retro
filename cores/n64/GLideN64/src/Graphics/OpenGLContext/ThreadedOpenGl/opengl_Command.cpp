#include "opengl_Command.h"
#include "Graphics/OpenGLContext/GLFunctions.h"
#include "RingBufferPool.h"
#include <chrono>

namespace opengl {

	// Max memory pool size
	RingBufferPool OpenGlCommand::m_ringBufferPool(1024 * 1024 * 200 );

	void OpenGlCommand::performCommandSingleThreaded()
	{
		commandToExecute();

		setInUse(false);
#ifdef GL_DEBUG
		if (m_isGlCommand) {
			auto error = ptrGetError();
			if (error != GL_NO_ERROR) {
				std::stringstream errorString;
				errorString << " OpenGL error: 0x" << std::hex << error << ", on function: " << m_functionName;
				LOG(LOG_ERROR, errorString.str().c_str());
				throw std::runtime_error(errorString.str().c_str());
			}
		}
#endif
	}

	bool OpenGlCommand::isTimeToShutdown()
	{
		return false;
	}

	void OpenGlCommand::performCommand()
	{
		std::unique_lock<std::mutex> lock(m_condvarMutex);
		performCommandSingleThreaded();
		if (m_synced) {
#ifdef GL_DEBUG
			if (m_logIfSynced) {
				std::stringstream errorString;
				errorString << " Executing synced: " << m_functionName;
				LOG(LOG_ERROR, errorString.str().c_str());
			}
#endif
			m_executed = true;
			m_condition.notify_all();
		}
	}

	void OpenGlCommand::waitOnCommand()
	{
		std::unique_lock<std::mutex> lock(m_condvarMutex);

		if (m_synced && !m_executed) {
			m_condition.wait(lock, [this] { return m_executed; });
		}

		m_executed = false;
	}
#ifdef GL_DEBUG
	std::string OpenGlCommand::getFunctionName()
	{
		return m_functionName;
	}
#endif
	OpenGlCommand::OpenGlCommand(bool _synced, bool _logIfSynced, const std::string &_functionName,
		bool _isGlCommand) :
		m_synced(_synced), m_executed(false)
#ifdef GL_DEBUG
		, m_logIfSynced(_logIfSynced)
		, m_functionName(std::move(_functionName))
		, m_isGlCommand(_isGlCommand)
#endif
	{
	}

}
