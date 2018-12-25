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

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "ComponentType.h"
#include "Entity.h"

namespace daybreak {
    class Manager;

    class System {
    private:
        ComponentTypeSet m_components;
        std::set<Entity> m_entities;
    protected:
        Manager& m_manager;

        inline void set_components(ComponentTypeSet&& components) { m_components = std::move(components); }

    public:
        explicit System(Manager& manager);
        virtual ~System() = default;

        inline const ComponentTypeSet& get_components() const { return m_components; }

        inline bool register_entity(Entity entity) { return m_entities.insert(entity).second; }

        inline bool unregister_entity(Entity entity) { return m_entities.erase(entity); }

        inline bool has_entity(Entity entity) const { return (m_entities.end() != m_entities.find(entity)); }

        size_t update(double_t delta);
        virtual void update(double_t delta, Entity entity) = 0;

        size_t render(VkCommandBuffer cmd);
        virtual void render(VkCommandBuffer cmd, Entity entity) = 0;
    };
}


#endif
