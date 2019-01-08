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

#include "../../include/DescriptorSet.h"

namespace daybreak {

    DescriptorSet::DescriptorSet(const Pipeline* pipeline) : m_set(0), m_pipeline(pipeline) {
        if (!API::find_desc_set(pipeline->pipeline(), m_set)) {
            VkDescriptorSetLayout layouts[] = {pipeline->desc_set_layout()};
            VkDescriptorSetAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            alloc_info.descriptorPool = API::descriptor_pool();
            alloc_info.descriptorSetCount = 1;
            alloc_info.pSetLayouts = layouts;

            if (vkAllocateDescriptorSets(API::device(), &alloc_info, &m_set) != VK_SUCCESS) {
                throw std::runtime_error("failed to create descriptor set");
            }
        }

        for (Binding& binding : pipeline->bindings()) {
            auto buffer = new Buffer(binding.size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
            VkDescriptorBufferInfo buffer_info = {buffer->getBuffer(), 0, binding.size};
            m_buffers[binding.name] = buffer;

            VkWriteDescriptorSet desc_write = {};
            desc_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            desc_write.dstSet = m_set;
            desc_write.dstBinding = binding.binding;
            desc_write.dstArrayElement = 0;
            desc_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            desc_write.descriptorCount = 1;
            desc_write.pBufferInfo = &buffer_info;

            vkUpdateDescriptorSets(API::device(), 1, &desc_write, 0, nullptr);
        }
    }

    DescriptorSet::~DescriptorSet() {
        for (auto& entry : m_buffers) {
            delete entry.second;
        }
        API::destroy_desc_set(m_pipeline->pipeline(), m_set);
    }

    void DescriptorSet::bind(VkCommandBuffer cmd) const {
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->layout(), 0, 1, &m_set, 0, nullptr);
    }

    bool DescriptorSet::set_value(const std::string& name, const void* data, size_t size) {
        if (m_buffers[name]) {
            m_buffers[name]->load(data, size);
            return true;
        }
        return false;
    }
}