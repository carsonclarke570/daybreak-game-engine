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