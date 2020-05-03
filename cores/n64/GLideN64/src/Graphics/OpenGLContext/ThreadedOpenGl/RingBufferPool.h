#pragma once

#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace opengl {
//This class is only thread safe for a single producer and single consumer
class PoolBufferPointer
{
public:
	PoolBufferPointer();
	PoolBufferPointer(const PoolBufferPointer& other);
	PoolBufferPointer& operator=(const PoolBufferPointer& other);

	bool isValid() const;
	size_t getSize() const;
private:
	PoolBufferPointer(size_t _offset, size_t _size, size_t _realSize, bool _isValid);

	size_t m_offset;
	size_t m_size;
	size_t m_realSize;
	bool m_isValid;

	friend class RingBufferPool;
};

class RingBufferPool
{
public:

	explicit RingBufferPool(size_t _poolSize);

	// Create a buffer pool. This method will block if there is not enough space until
	// enough space has been freed from a seperate thread. If a buffer bigger than the
	// maximum is allocated, then a run time exception will be thrown
	PoolBufferPointer createPoolBuffer(const char* _buffer, size_t _bufferSize);

	// Retrieves a buffer from the buffer pool, will return a nullptr if an invalid
	// buffer is provided
	const char* getBufferFromPool(PoolBufferPointer _poolBufferPointer);

	// Removes the give buffer from the pool, no action will be taken if the provided
	// buffer is not valid
	void removeBufferFromPool(PoolBufferPointer _poolBufferPointer);

private:
	std::atomic<size_t> m_inUseStartOffset;
	std::atomic<size_t> m_inUseEndOffset;
	std::vector<char> m_poolBuffer;
	std::mutex m_mutex;
	std::atomic<bool> m_full;
	std::condition_variable_any m_condition;
	size_t m_maxBufferPoolSize;
	static const size_t m_startBufferPoolSize = 1024 * 100;
};

}