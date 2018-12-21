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

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "VulkanAPI.h"

namespace daybreak {

    class Buffer {
        NO_COPY(Buffer)
    private:
        VkBuffer m_buffer;
        VkDeviceMemory m_memory;
        VkDeviceSize m_size;
        VkBufferUsageFlagBits m_usage;
    public:
        Buffer(VkDeviceSize size, VkBufferUsageFlagBits usage);
        ~Buffer();

        inline void load(const void* data) { load(data, m_size); }
        void load(const void* data, VkDeviceSize size, size_t offset = 0);

        inline VkDeviceSize size() const { return m_size; }
        inline VkBuffer getBuffer() { return m_buffer; }
        inline VkDeviceMemory&getMemory() { return m_memory; }
    };
}

#endif //VULKANPLAYGROUND_BUFFER_H
