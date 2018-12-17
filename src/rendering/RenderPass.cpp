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

#include "../../include/RenderPass.h"

namespace daybreak {

    RenderPass::RenderPass(const std::vector<VkFormat>& formats, bool present) : m_formats(formats) {
        const size_t format_count = formats.size();
        m_has_depth = false;
        for (uint32_t i = 0; i < format_count; i++) {
            VkAttachmentDescription color_attach = {};
            color_attach.format = formats[i];
            color_attach.samples = VK_SAMPLE_COUNT_1_BIT;
            color_attach.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            color_attach.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            color_attach.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            color_attach.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            color_attach.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

            VkAttachmentReference color_attach_ref = {};
            color_attach_ref.attachment = i;

            if (API::is_depth_format(formats[i])) {
                color_attach.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                color_attach_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                m_has_depth = true;
            } else {
                color_attach.finalLayout = present ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                color_attach_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            }
            m_attachment_descriptions.push_back(color_attach);
            m_attachment_references.push_back(color_attach_ref);
        }

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = color_attachment_count();
        subpass.pColorAttachments = m_attachment_references.data();
        if (m_has_depth)
            subpass.pDepthStencilAttachment = &m_attachment_references[m_attachment_references.size()-1];

        VkRenderPassCreateInfo render_pass_info = {};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = uint32_t(m_attachment_descriptions.size());
        render_pass_info.pAttachments = m_attachment_descriptions.data();
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;

        if (vkCreateRenderPass(API::device(), &render_pass_info, nullptr, &m_renderpass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
    }

    RenderPass::~RenderPass() {
        vkDestroyRenderPass(API::device(), m_renderpass, nullptr);
    }

    const RenderPass& RenderPass::geometry_pass() {
        static RenderPass render_pass({
            VK_FORMAT_B8G8R8A8_UNORM, // Color
        });
        return render_pass;
    }

    const RenderPass& RenderPass::present_pass() {
        static RenderPass render_pass({
            VK_FORMAT_B8G8R8A8_UNORM, // Final Color
        }, true);
        return render_pass;
    }
}