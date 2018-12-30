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

#ifndef _GAME_COMPONENT_H_
#define _GAME_COMPONENT_H_

#include "Common.h"

namespace daybreak {

    class GameComponent {
        NO_COPY(GameComponent)
    public:
        GameComponent() = default;
        virtual ~GameComponent() = default;

        virtual void render(VkCommandBuffer cmd) = 0;
        virtual void update(double_t delta) = 0;
    };
}


#endif
