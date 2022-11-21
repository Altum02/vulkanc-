#include "window.hpp"

namespace mv{
    Window::Window(){
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }
    Window::~Window(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        win->framebufferResized = true;
    }
}