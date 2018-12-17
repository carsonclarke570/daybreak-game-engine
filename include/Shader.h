#ifndef _SHADER_H_
#define _SHADER_H_

#include "VulkanAPI.h"

namespace daybreak {
    class Shader {
        NO_COPY(Shader)
    private:
        VkShaderModule m_shader;
        VkShaderStageFlagBits m_stage;
    public:
        Shader(const std::vector<char>& code, VkShaderStageFlagBits stage);
        ~Shader();

        inline VkShaderModule module() const { return m_shader; }
        inline VkShaderStageFlagBits stage() const { return m_stage; }
    };
}

#endif
