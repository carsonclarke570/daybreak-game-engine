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

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Entity.h"
#include "Component.h"
#include "ComponentType.h"
#include "ComponentStore.h"
#include "System.h"

namespace daybreak {
    class Manager {
    private:
        Entity m_last_entity;
        std::unordered_map<Entity, ComponentTypeSet> m_entities;
        std::map<ComponentType, std::unique_ptr<IComponentStore>> m_stores;
        std::vector<std::shared_ptr<System>> m_systems;
    public:
        Manager();
        virtual ~Manager() = default;

        template<class T>
        inline bool create_store() {
            return m_stores.insert(std::make_pair(T::m_type, std::unique_ptr(new ComponentStore<T>()))).second;
        }

        template<class T>
        inline ComponentStore<T>& get_store() {
            auto store = m_stores.find(T::m_type);
            if (m_stores.end() == store) {
                throw std::runtime_error("failed to get component store");
            }
            return reinterpret_cast<ComponentStore<T>&>(*(store->second));
        }

        inline Entity create_entity() {
            ASSERT(m_last_entity < std::numeric_limits<Entity>::max(), "Manager - Unable to create any more entities");
            m_entities.insert(std::make_pair((m_last_entity++), ComponentTypeSet()));
            return m_last_entity;
        }

        template<class T>
        inline bool add_component(const Entity entity, T&& component) {
            auto e = m_entities.find(entity);
            if (m_entities.end() == e) {
                throw std::runtime_error("entity does not exist");
            }
            (*e).second.insert(T::m_type);
            return get_store<T>().add(entity, std::move(component));
        }

        void add_system(const std::shared_ptr<System>& system);
        size_t register_entity(const Entity entity);
        size_t unregister_entity(const Entity entity);
        size_t update(double_t delta);
        size_t render(VkCommandBuffer cmd);
    };
}

#endif
