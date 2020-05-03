#pragma once

#include <memory>
#include <vector>


namespace opengl {

	class PoolObject {
	public:

		PoolObject();

		bool isInUse();
		void setInUse(bool _inUse);

		int getPoolId();

		void setPoolId(int _poolId);

		int getObjectId();

		void setObjectId(int _objectId);
	private:

		bool m_inUse;
		int m_poolId;
		int m_objectId;
	};

	class OpenGlCommandPool
	{
	public:
		static OpenGlCommandPool& get();

		int getNextAvailablePool();

		std::shared_ptr<PoolObject> getAvailableObject(int _poolId);

		void addObjectToPool(int _poolId, std::shared_ptr<PoolObject> _object);

	private:
		std::vector<std::vector<std::shared_ptr<PoolObject>>> m_objectPool;
		std::vector<unsigned int> m_objectPoolIndex;
	};
}
