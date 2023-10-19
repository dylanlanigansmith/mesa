#include "ForceWrite.hpp"

void ForceWrite::write(void* ptr, const char* bytes, std::size_t length)
{
	if (procMem == nullptr)
		procMem = std::make_unique<ProcMem>();

	pwrite(procMem->fd, bytes, length, reinterpret_cast<off_t>(ptr));
}