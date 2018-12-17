#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "VulkanAPI.h"

namespace daybreak {

    class Buffer {
    private:
        VkBuffer m_buffer;
        VkDeviceMemory m_memory;
        VkDeviceSize m_size;
        VkBufferUsageFlagBits m_usage;
    public:
        Buffer(VkDeviceSize size, VkBufferUsageFlagBits usage);
        Buffer(const Buffer& other);
        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(const Buffer& other);
        Buffer& operator=(Buffer&& other) noexcept;
        ~Buffer();

        inline void load(const void* data) { load(data, m_size); }
        void load(const void* data, VkDeviceSize size, size_t offset = 0);

        inline VkDeviceSize size() const { return m_size; }
        inline VkBuffer getBuffer() { return m_buffer; }
        inline VkDeviceMemory&getMemory() { return m_memory; }
    };
}

#endif //VULKANPLAYGROUND_BUFFER_H
