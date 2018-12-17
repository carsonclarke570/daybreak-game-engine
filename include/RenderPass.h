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

#ifndef _RENDERPASS_H_
#define _RENDERPASS_H_

#include "VulkanAPI.h"

namespace daybreak {

    class RenderPass {
    private:
        std::vector<VkFormat> m_formats;
        std::vector<VkAttachmentReference> m_attachment_references;
        std::vector<VkAttachmentDescription> m_attachment_descriptions;
        VkRenderPass m_renderpass;
        bool m_has_depth;
    public:
        RenderPass(const std::vector<VkFormat>& formats, bool present = false);
        ~RenderPass();

        inline uint32_t color_attachment_count() const { return uint32_t(m_attachment_descriptions.size()) - m_has_depth; }
        inline const std::vector<VkFormat> formats() const { return m_formats; }

        operator VkRenderPass () const { return m_renderpass; }

        static const RenderPass& geometry_pass();
        static const RenderPass& present_pass();
    };
}

#endif
