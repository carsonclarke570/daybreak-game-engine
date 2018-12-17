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
