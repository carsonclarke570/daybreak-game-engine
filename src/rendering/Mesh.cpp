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

#include "../../include/Mesh.h"

namespace daybreak {

    Vertex::Vertex() {
        position = {0.0f, 0.0f, 0.0f};
        normal = {0.0f, 0.0f, 0.0f};
        color = {0.0f, 0.0f, 0.0f};
        texcoord = {0.0f, 0.0f};
    }

    VkVertexInputBindingDescription Vertex::get_binding() {
        VkVertexInputBindingDescription binding = {};
        binding.binding = 0;
        binding.stride = sizeof(Vertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return binding;
    }

    std::array<VkVertexInputAttributeDescription, 4> Vertex::get_attributes() {
        std::array<VkVertexInputAttributeDescription, 4> attribute = {};

        attribute[0].binding = 0;
        attribute[0].location = 0;
        attribute[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute[0].offset = offsetof(Vertex, position);

        attribute[1].binding = 0;
        attribute[1].location = 1;
        attribute[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute[1].offset = offsetof(Vertex, normal);

        attribute[2].binding = 0;
        attribute[2].location = 2;
        attribute[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute[2].offset = offsetof(Vertex, color);

        attribute[3].binding = 0;
        attribute[3].location = 3;
        attribute[3].format = VK_FORMAT_R32G32_SFLOAT;
        attribute[3].offset = offsetof(Vertex, texcoord);

        return attribute;
    }

    bool Vertex::operator==(const Vertex& other) const {
        return position == other.position && color == other.color && texcoord == other.texcoord && normal == other.normal;
    }

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) :
        m_vertices(Buffer(vertices.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)),
        m_indices(Buffer(indices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT)) {
        m_vertices.load(vertices.data());
        m_indices.load(indices.data());
        m_count = indices.size();
    }

    void Mesh::render(VkCommandBuffer cmd) {
        const VkBuffer buffers[] { m_vertices.getBuffer() };
        const VkDeviceSize offsets[] {0};
        vkCmdBindVertexBuffers(cmd, 0, 1, buffers, offsets);
        vkCmdBindIndexBuffer(cmd, m_indices.getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmd, m_count, 1, 0, 0, 0);
    }
}