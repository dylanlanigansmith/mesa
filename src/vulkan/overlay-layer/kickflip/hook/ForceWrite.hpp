#ifndef FORCEWRITE_HPP
#define FORCEWRITE_HPP

#include <cstdint>
#include <fcntl.h>
#include <memory>
#include <unistd.h>

namespace ForceWrite {
	struct ProcMem {
		int fd{};

		ProcMem()
			: fd{ open("/proc/self/mem", O_WRONLY) }
		{
		}

		~ProcMem()
		{
			close(fd);
		}
	};

	static thread_local std::unique_ptr<ProcMem> procMem{ nullptr };

	void write(void* ptr, const char* bytes, std::size_t length);

	template <typename T>
	void write(T* ptr, const T& value)
	{
		write(ptr, reinterpret_cast<const char*>(&value), sizeof(T));
	}

}

#endif