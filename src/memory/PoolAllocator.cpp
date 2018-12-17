/**
 * Copyright 2018 Carson Clarke-Magrab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
**/

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