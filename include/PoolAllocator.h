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
