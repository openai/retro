#include "../MemoryStatus.h"

/* Code taken from stackoverflow.com/questions/18394647 */
bool isMemoryWritable(void * ptr, size_t byteCount)
{
	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(ptr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
		return false;

	if (mbi.State != MEM_COMMIT)
		return false;

	if ((mbi.Protect & (PAGE_NOACCESS | PAGE_EXECUTE | PAGE_READONLY)) != 0)
		return false;

	// Check that the start of memory block is in the same "region" as the end.
	// If it isn't, "simplify" the problem into checking that the rest of the memory is writeable.
	size_t blockOffset = (size_t)(reinterpret_cast<u8*>(ptr) - reinterpret_cast<u8*>(mbi.BaseAddress));
	size_t blockBytesPostPtr = mbi.RegionSize - blockOffset;

	if (blockBytesPostPtr < byteCount)
		return isMemoryWritable(reinterpret_cast<u8*>(ptr) + blockBytesPostPtr, byteCount - blockBytesPostPtr);

	return true;
}
