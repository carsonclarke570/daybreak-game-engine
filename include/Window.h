#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Common.h"

namespace daybreak {
    class Window {
    private:
        static GLFWwindow* m_window;
        static bool m_resize;
    public:
        static void create();
        static void destroy();

        static bool should_close();
        static bool should_resize();

        static void resize_callback(GLFWwindow* window, int width, int height);

        static inline void poll_events() { glfwPollEvents(); }
        static inline GLFWwindow* get_window() { return m_window; }
        static inline void set_resize(bool resize) { m_resize = resize; }
    };
}

#endif
