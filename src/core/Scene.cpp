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

#include "../../include/Scene.h"

namespace daybreak {

    Scene::Scene(std::vector<Shader*> shaders, std::vector<Binding> bindings) : m_pipeline(shaders, bindings),
                                                                                m_set(m_pipeline) {
    }

    Scene::~Scene() {
        // DO NOT DELETE MESHES, THAT'S IS THE ASSET MANAGERS JOB
    }

    void Scene::update(double_t delta) {
        m_manager.update(delta);
    }

    void Scene::render() {
        VkCommandBuffer cmd = API::begin_render_command_buffer();
        API::begin_present_pass();

        // Bind current pipeline & descriptor set
        {
            vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipeline());
            VkDescriptorSet set = m_set.set();
            vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.layout(), 0, 1, &set, 0, nullptr);
        }

        { // Resize viewport and scissor
            VkViewport viewport = API::viewport();
            VkRect2D scissor = API::scissor();
            vkCmdSetScissor(cmd, 0, 1, &scissor);
            vkCmdSetViewport(cmd, 0, 1, &viewport);
        }

        // Call all systems that have render functions
        m_manager.render(cmd);

        API::end_present_pass();
        API::end_render_command_buffer();
    }

    void Scene::add_mesh(daybreak::Mesh* mesh) {
        m_meshes.push_back(mesh);
    }
}