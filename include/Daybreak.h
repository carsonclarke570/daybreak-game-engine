#ifndef _DAYBREAK_H_
#define _DAYBREAK_H_

#include "Game.h"
#include "VulkanAPI.h"
#include "Window.h"

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
