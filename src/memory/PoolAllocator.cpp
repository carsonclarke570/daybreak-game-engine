#include "../../include/PoolAllocator.h"

namespace daybreak {

    PoolAllocator::PoolAllocator(size_t total_size, size_t chunk_size) : Allocator(total_size) {
        ASSERT((total_size % chunk_size == 0), "PoolAllocator - Total size must be a multiple of chunk size!");
        this->m_chunk_size = chunk_size;
        this->reset();
    }

    void* PoolAllocator::allocate(size_t size, uint8_t alignment) {
        ASSERT((size == m_chunk_size), "PoolAllocator - Size of allocation must be equal to chunk size!");
        ASSERT((!m_free_blocks.empty()), "PoolAllocator - Allocation pool empty!");

        void* data = m_free_blocks.back();
        m_free_blocks.pop_back();
        m_used += m_chunk_size;

        return data;
    }

    void PoolAllocator::release(void *ptr) {
        memset(ptr, 0, m_chunk_size);
        m_free_blocks.push_back(ptr);
        m_used -= m_chunk_size;
    }

    void PoolAllocator::reset() {
        memset(m_start, 0, m_size);
        m_used = 0;

        const uint32_t n = m_size / m_chunk_size;
        for (uint32_t i = 0; i < n; i++) {
            void* address = (char*) m_start + (i * m_chunk_size);
            m_free_blocks.push_back(address);
        }
    }
}