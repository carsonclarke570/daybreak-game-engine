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

#ifndef _SCENE_H_
#define _SCENE_H_

#include "DescriptorSet.h"
#include "Manager.h"
#include "Pipeline.h"

namespace daybreak {

    class Scene {
    private:
        Pipeline m_pipeline;
        DescriptorSet m_set;
        Manager m_manager;
    public:
        Scene(std::vector<Shader*> shaders, std::vector<Binding> bindings);
        ~Scene();

        void update(double_t delta);
        void render();

        inline bool update_uniform(std::string name, const void* data, size_t size) {
            return m_set.set_value(name, data, size);
        }

        template<class T>
        inline bool create_store() {
            return m_manager.create_store<T>();
        }

        inline void add_system(std::shared_ptr<System>& system) {
            m_manager.add_system(system);
        }

        inline Entity create_entity() {
            return m_manager.create_entity();
        }

        template<class T>
        inline void add_component(const Entity entity, T&& component) {
            m_manager.add_component(entity, component);
        }

        inline size_t register_entity(const Entity entity) {
            return m_manager.register_entity(entity);
        }

        inline size_t unregister_entity(const Entity entity) {
            return m_manager.unregister_entity(entity);
        }

        inline Manager& get_manager() {
            return m_manager;
        }
    };
}

#endif
