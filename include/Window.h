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
