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

