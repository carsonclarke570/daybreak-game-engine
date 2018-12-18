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

    PoolAllocator::PoolAllocator(size_t total_size, size_t chunk_size) : m_chunk_size(chunk_size), m_size(total_size) {
        ASSERT((total_size % chunk_size == 0), "PoolAllocator - Total size must be a multiple of chunk size!");
        m_memory.push_back(malloc(total_size));
        this->reset();
    }

    PoolAllocator::~PoolAllocator() {
        for (void* ptr : m_memory) {
            free(ptr);
        }
    }

    void* PoolAllocator::allocate() {
        if (m_free_chunks.empty()) {
            resize();
        }

        void* data = m_free_chunks.back();
        m_free_chunks.pop_back();
        return data;
    }

    void PoolAllocator::release(void *ptr) {
        m_free_chunks.push_back(ptr);
    }

    void PoolAllocator::reset() {
        for (void* ptr : m_memory) {
            const uint32_t n = sizeof(ptr) / m_chunk_size;
            for (uint32_t i = 0; i < n; i++) {
                m_free_chunks.push_back((uint8_t*)ptr + (i * m_chunk_size));
            }
        }
    }

    void PoolAllocator::resize() {
        void* data = malloc(m_size);

        const uint32_t n = m_size / m_chunk_size;
        for (uint32_t i = 0; i < n; i++) {
            m_free_chunks.push_back((uint8_t*)data + (i * m_chunk_size));
        }

        m_size = m_size * 2;
        m_memory.push_back(data);
    }
}