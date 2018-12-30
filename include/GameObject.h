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

namespace daybreak {

    class GameObject {
        NO_COPY(GameObject)
    private:
        std::vector<GameComponent*> m_components;
        std::vector<GameObject*> m_children;

        void render_all(VkCommandBuffer cmd);
        void update_all(double_t delta);
    public:
        GameObject() = default;
        ~GameObject();

        void render(VkCommandBuffer cmd);
        void update(double_t delta);

        void add_child(GameObject* child);
        void add_component(GameComponent* component);

        inline std::vector<GameComponent*> get_components() { return m_components; }

        inline std::vector<GameObject*> get_children() { return m_children; }
    };
}

#endif
