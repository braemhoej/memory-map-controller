#ifndef MM_CONTROLLER_MEMORY_MAP_CONTROLLER_H
#define MM_CONTROLLER_MEMORY_MAP_CONTROLLER_H

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>      /* Definition of MADV_* constants */
#include <sys/syscall.h>   /* Definition of SYS_* constants */
#include <sys/uio.h>       /* Definition of struct iovec type */
#include <unistd.h>
#include <map>

class memory_map_controller {
public:
    /**
     * Creates and memory-maps a new temporary file of size n * sizeof(Type).
     * @tparam Type element type.
     * @param n number of elements.
     * @return Begin address of memory-mapped region.
     */
    template <typename Type>
    Type* falloc(unsigned long n);

    /**
     * Creates and memory-maps a new temporary file of size n * sizeof(Type). Advises kernel of expected behaviour.
     * @tparam Type element type.
     * @param n number of elements.
     * @param madvise advise to kernel.
     * @return Begin address of memory-mapped region.
     */
    template <typename Type>
    Type* falloc(unsigned long n, int madvise);

    /**
     * Unmaps a previously memory-mapped region of memory.
     * Underlying file is automatically deleted upon program termination.
     * @param address
     */
    void free(void* address);

    /**
     * Unmaps all regions mapped by this controller.
     */
    void purge();
private:
    std::map<void*, std::pair<FILE*, unsigned long>> files_;
};

#endif //MM_CONTROLLER_MEMORY_MAP_CONTROLLER_H
