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

#ifndef _COMPONENT_STORE_H
#define _COMPONENT_STORE_H

#include "Entity.h"
#include "Component.h"
#include "PoolAllocator.h"

namespace daybreak {

    class IComponentStore {
    };

    template<typename T>
    class ComponentStore : public IComponentStore {
    private:
        std::unordered_map<Entity, T> m_store;
        static const ComponentType m_type = T::m_type;
    public:

        inline bool add(const Entity entity, T&& component) {
            return m_store.insert(std::pair(entity, std::move(component))).second;
        }

        inline bool remove(Entity entity) {
            return (0 < m_store.erase(entity));
        }

        inline T& get(Entity entity) {
            return m_store.at(entity);
        }

        inline T extract(Entity entity) {
            T component = std::move(m_store.at(entity));
            m_store.erase(entity);
            return component;
        }

        inline bool has(Entity entity) const {
            return (m_store.end() != m_store.find(entity));
        }

        inline const std::unordered_map<Entity, T>& get_components() { return m_store; }
    };
}

#endif
