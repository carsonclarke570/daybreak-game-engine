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

#include "../../include/Buffer.h"

namespace daybreak {
    Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlagBits usage) : m_size(size), m_usage(usage) {
        if (API::find_buffer(m_size, m_usage, m_buffer, m_memory)) {
            return;
        }

        VkBufferCreateInfo buffer_info = {};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = size;
        buffer_info.usage = usage;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(API::device(), &buffer_info, nullptr, &m_buffer)) {
            throw std::runtime_error("failed to create vertex buffer!");
        }
        VkMemoryRequirements reqs;
        vkGetBufferMemoryRequirements(API::device(), m_buffer, &reqs);

        VkMemoryAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = reqs.size;
        alloc_info.memoryTypeIndex = API::find_memory_type(reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(API::device(), &alloc_info, nullptr, &m_memory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }
        vkBindBufferMemory(API::device(), m_buffer, m_memory, 0);
    }

    Buffer::~Buffer() {
        API::destroy_buffer(m_size, m_usage, m_buffer, m_memory);
    }

    void Buffer::load(const void* data, VkDeviceSize size, size_t offset) {
        void* dst;
        vkMapMemory(API::device(), m_memory, offset, size, 0, &dst);
        memcpy(dst, data, (size_t) size);
        vkUnmapMemory(API::device(), m_memory);
    }
}