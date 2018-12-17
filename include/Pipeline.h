#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include "Common.h"
#include "Shader.h"
#include "Mesh.h"
#include "RenderPass.h"

namespace daybreak {

    class Pipeline {
        NO_COPY(Pipeline)
    private:
        VkPipeline m_pipeline;
        VkPipelineLayout m_layout;
    public:
        Pipeline(std::vector<Shader*>& shaders);
        ~Pipeline();

        inline VkPipeline pipeline() const { return m_pipeline; }
        inline VkPipelineLayout layout() const { return m_layout; }
    };
}

#endif
