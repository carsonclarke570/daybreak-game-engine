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

#include "../../include/Manager.h"

namespace daybreak {

    Manager::Manager() :
            m_last_entity(invalid_entity),
            m_entities(),
            m_stores(),
            m_systems() {
    }

    void Manager::add_system(const std::shared_ptr<System>& system) {
        ASSERT(((!system) || (system->get_components().empty())), "Manager - System must specify required components");
        m_systems.push_back(system);
    }

    size_t Manager::register_entity(const Entity entity) {
        size_t n = 0;

        auto e = m_entities.find(entity);
        ASSERT((e != m_entities.end()), "Manager - Entity does not exist")
        auto c = (*e).second;

        for (auto& system : m_systems) {
            auto components = system->get_components();

            if (std::includes(c.begin(), c.end(), components.begin(), components.end())) {
                system->register_entity(entity);
                n++;
            }
        }
        return n;
    }

    size_t Manager::unregister_entity(const Entity entity) {
        size_t n = 0;

        auto e = m_entities.find(entity);
        ASSERT((e != m_entities.end()), "Manager - Entity does not exist")
        auto c = (*e).second;

        for (auto& system : m_systems) {
            n += system->unregister_entity(entity);
        }
        return n;
    }

    size_t Manager::update(double_t delta) {
        size_t n = 0;
        for (auto& system : m_systems) {
            n += system->update(delta);
        }
        return n;
    }

    size_t Manager::render(VkCommandBuffer cmd) {
        size_t n = 0;
        for (auto& system : m_systems) {
            n += system->render(cmd);
        }
        return n;
    }
}

