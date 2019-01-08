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

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "Common.h"
#include "DescriptorSet.h"
#include "GameComponent.h"
#include "Renderer.h"
#include "Transform.h"
#include "UniformBufferObject.h"

namespace daybreak {

    class GameObject {
        NO_COPY(GameObject)
    private:
        std::vector<GameComponent*> m_components;
        std::vector<GameObject*> m_children;

        void render_all(Renderer* renderer, VkCommandBuffer cmd);
        void update_all(double_t delta);
    protected:
        GameObject* m_parent;
        Mesh* m_mesh;
        Transform m_world_transform;
        Transform m_transform;
    public:
        explicit GameObject(Mesh* mesh = nullptr);
        ~GameObject();

        void render(Renderer* renderer, VkCommandBuffer cmd);
        void update(double_t delta);

        void add_child(GameObject* child);
        void add_component(GameComponent* component);

        inline void translate(glm::vec3& translate) { m_transform.translate(translate); }

        inline void scale(glm::vec3& scale) { m_transform.scale(scale); }

        inline void rotate(float_t angle, glm::vec3& axis) { m_transform.rotate(angle, axis); }

        inline void set_position(glm::vec3& position) { m_transform.set_position(position); }

        inline void set_scale(glm::vec3& scale) { m_transform.set_scale(scale); }

        inline void set_rotation(float_t angle, glm::vec3& axis) { m_transform.set_rotation(axis, angle); }

        inline std::vector<GameComponent*> get_components() { return m_components; }
        inline std::vector<GameObject*> get_children() { return m_children; }

        inline Mesh* get_mesh() const { return m_mesh; }
    };
}

#endif
