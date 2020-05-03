#include "opengl_WrappedFunctions.h"

namespace opengl {

	std::vector<char> GlDrawArraysUnbufferedCommand::m_attribsData(2 * 1024 * 1024, 0);
	std::vector<char> GlDrawElementsUnbufferedCommand::m_attribsData(2 * 1024 * 1024, 0);
	GLenum GlMapBufferRangeWriteAsyncCommand::m_targetTemp;
	GLintptr GlMapBufferRangeWriteAsyncCommand::m_offsetTemp;
	GLsizeiptr GlMapBufferRangeWriteAsyncCommand::m_lengthTemp;
	GLbitfield GlMapBufferRangeWriteAsyncCommand::m_accessTemp;
	std::unordered_map<int, std::shared_ptr<std::vector<u8>>> GlMapBufferRangeWriteAsyncCommand::m_TempData;
	std::unordered_map<int, std::shared_ptr<std::vector<u8>>> GlMapBufferRangeReadAsyncCommand::m_data;
	std::mutex GlMapBufferRangeReadAsyncCommand::m_mapMutex;
	std::unordered_map<int, GlVertexAttribPointerManager::VertexAttributeData> GlVertexAttribPointerManager::m_vertexAttributePointers;
	const void* GlVertexAttribPointerManager::smallestDataPtr = nullptr;
	std::unordered_map<int, GlVertexAttribPointerManager::VertexAttributeData> GlVertexAttribPointerManager::m_vertexAttributePointersRender;
	const void* GlVertexAttribPointerManager::smallestDataPtrRender = nullptr;
	std::unordered_map<GLenum, GLuint> GlBindBufferCommand::m_boundBuffersRender;
	std::unordered_map<GLenum, GLuint> GlBindBufferCommand::m_boundBuffers;
	GLuint GlReadPixelsAsyncCommand::m_readPixelsBoundBuffer = 0;
}
