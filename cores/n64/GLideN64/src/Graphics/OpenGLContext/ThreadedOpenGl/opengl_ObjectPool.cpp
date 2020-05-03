#include "opengl_ObjectPool.h"

namespace opengl {


	PoolObject::PoolObject() :
		m_inUse(false), m_poolId(0), m_objectId(0) {
	}

	bool PoolObject::isInUse()
	{
		return m_inUse;
	}

	void PoolObject::setInUse(bool _inUse)
	{
		m_inUse = _inUse;
	}

	int PoolObject::getPoolId()
	{
		return m_poolId;
	}

	void PoolObject::setPoolId(int _poolId)
	{
		m_poolId = _poolId;
	}

	int PoolObject::getObjectId()
	{
		return m_objectId;
	}

	void PoolObject::setObjectId(int _objectId)
	{
		m_objectId = _objectId;
	}

	OpenGlCommandPool &OpenGlCommandPool::get()
	{
		static OpenGlCommandPool commandPool;
		return commandPool;
	}

	int OpenGlCommandPool::getNextAvailablePool()
	{
		m_objectPool.push_back(std::vector<std::shared_ptr<PoolObject>>());
		m_objectPoolIndex.push_back(0);
		return static_cast<int>(m_objectPool.size() - 1);
	}

	std::shared_ptr<PoolObject> OpenGlCommandPool::getAvailableObject(int _poolId)
	{
		auto &currentPool = m_objectPool[_poolId];
		auto &currentIndex = m_objectPoolIndex[_poolId];

		if (currentPool.empty()) {
			return nullptr;
		} else if (!currentPool[currentIndex]->isInUse()) {
			int objectId = currentIndex;
			++currentIndex;

			if (currentIndex == currentPool.size()) {
				currentIndex = 0;
			}

			return currentPool[objectId];

		} else {
			bool found = false;
			unsigned int index;
			//Not found in most common case, so go ahead and search
			for (index = currentIndex; index < currentPool.size() && !found; ++index) {
				found = !currentPool[index]->isInUse();
			}

			if (!found) {
				for (index = 0; index < currentIndex && !found; ++index) {
					found = !currentPool[index]->isInUse();
				}
			}

			if (found) {
				currentIndex = index;

				if (currentIndex == currentPool.size()) {
					currentIndex = 0;
				}
				--index;

				return currentPool[index];
			} else {
				currentIndex = 0;
				return nullptr;
			}
		}
	}

	void OpenGlCommandPool::addObjectToPool(int _poolId, std::shared_ptr<PoolObject> _object)
	{
		_object->setPoolId(_poolId);
		_object->setObjectId(static_cast<int>(m_objectPool[_poolId].size()));
		m_objectPool[_poolId].push_back(_object);
	}
}
