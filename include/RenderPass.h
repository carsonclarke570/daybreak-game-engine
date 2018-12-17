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
