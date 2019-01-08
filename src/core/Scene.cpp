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

    Scene::Scene(Renderer* renderer) :
            m_renderer(renderer),
            m_root(new GameObject()) {
    }

    Scene::~Scene() {
        delete m_root;
        delete m_renderer;
    }

    void Scene::update(double_t delta) {
        m_root->update(delta);
    }

    void Scene::render() {
        VkCommandBuffer cmd = API::begin_render_command_buffer();
        API::begin_present_pass();

        m_renderer->bind(cmd);

        // Set viewport
        {
            VPMat viewproj = {};
            viewproj.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
            viewproj.proj = glm::perspective(glm::radians(45.0f), API::viewport().width / API::viewport().height, 0.1f,
                                             10.0f);
            m_renderer->update_uniform("vp_mat", &viewproj, sizeof(VPMat));
        }

        { // Resize viewport and scissor
            VkViewport viewport = API::viewport();
            VkRect2D scissor = API::scissor();
            vkCmdSetScissor(cmd, 0, 1, &scissor);
            vkCmdSetViewport(cmd, 0, 1, &viewport);
        }

        m_root->render(m_renderer, cmd);

        API::end_present_pass();
        API::end_render_command_buffer();
    }
}