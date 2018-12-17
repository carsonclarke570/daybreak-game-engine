#ifndef _POOL_ALLOCATOR_H_
#define _POOL_ALLOCATOR_H_

#include "Allocator.h"

namespace daybreak {

    class PoolAllocator : public Allocator {
    private:
        size_t m_chunk_size;
        std::vector<void*> m_free_blocks;
    public:
        explicit PoolAllocator(size_t total_size, size_t chunk_size);

        void* allocate(size_t size, uint8_t alignment) override;
        inline void* allocate() { return allocate(m_chunk_size, 0); }
        void release(void* ptr) override;
        void reset() override;
    };
}

#endif
