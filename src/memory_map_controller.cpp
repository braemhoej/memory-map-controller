#include "memory_map_controller.h"

#include <csignal>

template <typename Type>
Type* memory_map_controller::falloc(unsigned long size) {
    return falloc<Type>(size, 0);
}
template <typename Type>
Type* memory_map_controller::falloc(unsigned long size, int madvise) {
    FILE* temporary_file = std::tmpfile();
    int file_descriptor = temporary_file->_fileno;
    if (file_descriptor < 0) {
        perror("Could not create temporary file for falloc");
        exit(1);
    }
    unsigned long bytes = size * sizeof(Type);
    ftruncate(file_descriptor, bytes);
    Type* begin = (Type*) mmap64(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE, file_descriptor, 0);

    if ((unsigned char*) begin == MAP_FAILED) {
        perror("Could not memory map file");
        exit(1);
    }
    ::madvise(begin, bytes, madvise);
    files_.emplace(begin, {temporary_file, bytes});
    return begin;
}

void memory_map_controller::free(void* address) {
    if (!files_.contains(address)) {
        perror("Address not registered by controller");
        exit(1);
    }
    unsigned long bytes = files_[address].second;
    if (munmap(address, bytes) < 0) {
        perror("Could not unmap file");
        exit(1);
    }
}

void memory_map_controller::purge() {
    for (auto mapping : files_) {
        free(mapping.first);
    }
}