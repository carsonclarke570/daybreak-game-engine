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

#include "../../include/GameObject.h"

namespace daybreak {

    GameObject::GameObject(Mesh* mesh) :
            m_components(std::vector<GameComponent*>()),
            m_children(std::vector<GameObject*>()) {
        m_mesh = mesh;
        m_parent = nullptr;
        m_transform = Transform();
        m_world_transform = Transform();
    }

    GameObject::~GameObject() {
        for (GameComponent* component : m_components) {
            delete component;
        }

        for (GameObject* object : m_children) {
            delete object;
        }
    }

    void GameObject::render_all(Renderer* renderer, VkCommandBuffer cmd) {
        for (GameComponent* component : m_components) {
            component->render(renderer, cmd);
        }
    }

    void GameObject::update_all(double_t delta) {
        for (GameComponent* component : m_components) {
            component->update(delta);
        }
    }

    void GameObject::render(Renderer* renderer, VkCommandBuffer cmd) {
        if (m_mesh) {
            MMat ubo = {};
            ubo.model = m_transform.get_transform();
            renderer->update_uniform("m_mat", &ubo, sizeof(MMat));
            m_mesh->render(cmd);
        }

        render_all(renderer, cmd);
        for (GameObject* object : m_children) {
            object->render(renderer, cmd);
        }
    }

    void GameObject::update(double_t delta) {
        if (m_parent)
            m_world_transform.set_transform(m_parent->m_world_transform.get_transform() * m_transform.get_transform());
        else
            m_world_transform.set_transform(m_transform.get_transform());

        update_all(delta);
        for (GameObject* object : m_children) {
            object->update(delta);
        }
    }

    void GameObject::add_child(GameObject* child) {
        m_children.push_back(child);
        child->m_parent = this;
    }

    void GameObject::add_component(GameComponent* component) {
        m_components.push_back(component);
    }
}