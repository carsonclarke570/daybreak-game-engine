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

#include "../../include/Window.h"

namespace daybreak {

    GLFWwindow* Window::m_window = nullptr;
    bool Window::m_resize = false;

    void Window::create() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
        glfwSetFramebufferSizeCallback(m_window, resize_callback);
    }

    void Window::destroy() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    bool Window::should_close() {
        return glfwWindowShouldClose(m_window) == 0;
    }

    bool Window::should_resize() {
        return m_resize;
    }

    void Window::resize_callback(GLFWwindow* window, int width, int height) {
        m_resize = true;
    }
}