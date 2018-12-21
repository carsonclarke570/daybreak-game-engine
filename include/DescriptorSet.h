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

#ifndef _DESCRIPTOR_SET_H_
#define _DESCRIPTOR_SET_H_

#include "Common.h"
#include "VulkanAPI.h"
#include "Pipeline.h"

namespace daybreak {

    class DescriptorSet {
        NO_COPY(DescriptorSet)
    private:
        const Pipeline& m_pipeline;
        VkDescriptorSet m_set;
        std::unordered_map<std::string, Buffer*> m_buffers;
    public:
        DescriptorSet(const Pipeline& m_pipeline);
        ~DescriptorSet();

        bool set_value(const std::string name, const void* data, size_t size);

        inline VkDescriptorSet set() { return m_set; }
    };
}

#endif
