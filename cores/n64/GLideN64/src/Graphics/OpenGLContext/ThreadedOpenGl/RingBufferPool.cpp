#include "RingBufferPool.h"
#include <memory>
#include <algorithm>
#include <sstream>
#include <Log.h>

namespace opengl {


PoolBufferPointer::PoolBufferPointer() :
	m_offset(0),
	m_size(0),
	m_realSize(0),
	m_isValid(false)
{

}

PoolBufferPointer::PoolBufferPointer(size_t _offset, size_t _size, size_t _realSize, bool _isValid) :
	m_offset(_offset),
	m_size(_size),
	m_realSize(_realSize),
	m_isValid(_isValid)
{
}

PoolBufferPointer::PoolBufferPointer(const PoolBufferPointer& other) :
	m_offset(other.m_offset),
	m_size(other.m_size),
	m_realSize(other.m_realSize),
	m_isValid(other.m_isValid)
{
}

PoolBufferPointer& PoolBufferPointer::operator=(const PoolBufferPointer& other)
{
	m_offset = other.m_offset;
	m_size = other.m_size;
	m_realSize = other.m_realSize;
	m_isValid = other.m_isValid;
	return *this;
}

bool PoolBufferPointer::isValid() const
{
	return m_isValid;
}

size_t PoolBufferPointer::getSize() const
{
	return m_size;
}

RingBufferPool::RingBufferPool(size_t _poolSize) :
	m_poolBuffer(m_startBufferPoolSize, 0),
	m_inUseStartOffset(0),
	m_inUseEndOffset(0),
	m_full(false),
	m_maxBufferPoolSize(_poolSize)
{

}

PoolBufferPointer RingBufferPool::createPoolBuffer(const char* _buffer, size_t _bufferSize)
{
	size_t realBufferSize = _bufferSize;
	const size_t remainder = _bufferSize % 4;

	if (remainder != 0)
		realBufferSize = _bufferSize + 4 - remainder;

	const size_t tempInUseStart = m_inUseStartOffset;

	const size_t remaining = tempInUseStart > m_inUseEndOffset || m_full ?
		static_cast<size_t>(tempInUseStart - m_inUseEndOffset) :
		m_poolBuffer.size() - m_inUseEndOffset + tempInUseStart;

	bool isValid = remaining >= realBufferSize;

	size_t startOffset = 0;

	// We have determined that it fits
	if (isValid) {

		// We don't want to split data between the end of the ring buffer and the start
		// Re-check buffer size if we are going to start at the beginning of the ring buffer
		if (m_inUseEndOffset + realBufferSize > m_poolBuffer.size()) {
			isValid = realBufferSize < tempInUseStart || tempInUseStart == m_inUseEndOffset;

			if (isValid) {
				startOffset = 0;
				m_inUseEndOffset = realBufferSize;
			} else {
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					m_condition.wait(lock, [this, realBufferSize] {
						const size_t tempInUseStartLocal = m_inUseStartOffset;
						return realBufferSize < tempInUseStartLocal ||
							   tempInUseStartLocal == m_inUseEndOffset;
					});
				}
				return createPoolBuffer(_buffer, _bufferSize);
			}
		} else {
			startOffset = m_inUseEndOffset;
			m_inUseEndOffset += realBufferSize;
		}
	} else {
		// Wait until enough space is avalable
		{
			if (realBufferSize > m_maxBufferPoolSize) {
				std::stringstream errorString;
				errorString << " Attempted to create buffer of invalid size, size=" << realBufferSize << ", max_size=" << m_maxBufferPoolSize;
				LOG(LOG_ERROR, errorString.str().c_str());
				throw std::runtime_error(errorString.str().c_str());
			}

			std::unique_lock<std::mutex> lock(m_mutex);
			if (m_poolBuffer.size() < realBufferSize) {
				std::stringstream errorString;
				errorString << " Increasing buffer size from " << m_poolBuffer.size() << " to " << realBufferSize;
				LOG(LOG_VERBOSE, errorString.str().c_str());

				m_poolBuffer.resize(realBufferSize);
			}

			m_condition.wait(lock, [this, realBufferSize] {
				const size_t tempInUseStartLocal = m_inUseStartOffset;
				const size_t remainingLocal =
					tempInUseStartLocal > m_inUseEndOffset || m_full ? static_cast<size_t>(
						tempInUseStartLocal - m_inUseEndOffset) :
					m_poolBuffer.size() - m_inUseEndOffset + tempInUseStartLocal;

				return remainingLocal >= realBufferSize;
			});
		}

		return createPoolBuffer(_buffer, _bufferSize);
	}

	std::copy_n(_buffer, _bufferSize, &m_poolBuffer[startOffset]);

	m_full = m_inUseEndOffset == tempInUseStart;

	return PoolBufferPointer(startOffset, _bufferSize, realBufferSize, isValid);
}

const char* RingBufferPool::getBufferFromPool(PoolBufferPointer _poolBufferPointer)
{
	if (!_poolBufferPointer.isValid()) {
		return nullptr;
	} else {
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_poolBuffer.data() + _poolBufferPointer.m_offset;
	}
}

void RingBufferPool::removeBufferFromPool(PoolBufferPointer _poolBufferPointer)
{
	if (_poolBufferPointer.isValid()) {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_inUseStartOffset = _poolBufferPointer.m_offset + _poolBufferPointer.m_realSize;
		m_full = false;
		m_condition.notify_one();
	}
}

}
