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

#ifndef _DAYBREAK_H_
#define _DAYBREAK_H_

#include "Game.h"
#include "VulkanAPI.h"
#include "Window.h"
#include "Timer.h"

namespace daybreak {

    class Daybreak {
    private:
        static Game* m_game;
        static bool m_is_running;

        static void run();
        static bool quit();
    public:
        static void load(Game* game);
        static void start();
        static void stop();
    };

}

#endif
