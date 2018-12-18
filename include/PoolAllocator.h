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

#include "Common.h"

namespace daybreak {

    class PoolAllocator  {
        NO_COPY(PoolAllocator)
    private:
        size_t m_size, m_chunk_size;
        std::vector<void*> m_memory, m_free_chunks;

        void resize();
    public:
        PoolAllocator(size_t total_size, size_t chunk_size);
        ~PoolAllocator();

        void* allocate();
        void release(void* ptr);
        void reset();
    };
}

#endif
