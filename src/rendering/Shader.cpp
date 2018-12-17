#include "../../include/Shader.h"

namespace daybreak {
    Shader::Shader(const std::vector<char>& code, VkShaderStageFlagBits stage) : m_stage(stage) {
        VkShaderModuleCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(API::device(), &create_info, nullptr, &m_shader) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
    }

    Shader::~Shader() {
        vkDestroyShaderModule(API::device(), m_shader, nullptr);
    }
}