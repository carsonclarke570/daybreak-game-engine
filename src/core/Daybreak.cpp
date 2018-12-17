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

#include "../../include/Daybreak.h"

namespace daybreak {

    Game* Daybreak::m_game = nullptr;
    bool Daybreak::m_is_running = false;

    void Daybreak::run() {

        LOG("Daybreak - Initializing game")
        m_game->init();
        LOG("Daybreak - Initialized game")

        while (!quit()) {
            Window::poll_events();
            m_game->update();
            m_game->render();
        }
        m_game->end();
        Window::destroy();
        LOG("Daybreak - Game quit")
    }

    bool Daybreak::quit() {
        return !Window::should_close() || m_game->quit();
    }

    void Daybreak::load(Game* game) {
        Window::create();
        API::init_vulkan();

        m_game = game;
    }

    void Daybreak::start() {
        if (m_is_running)
            return;
        run();
    }

    void Daybreak::stop() {
        if (!m_is_running)
            return;
        m_is_running = false;
    }
}

