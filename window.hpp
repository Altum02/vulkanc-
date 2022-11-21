#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace mv{
    class Window{
        public:
        Window();
        ~Window();
        GLFWwindow* getWindow(){return window;}
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        bool getFramebufferResized(){return framebufferResized;}
        void setFramebufferResizedFalse(){framebufferResized = false;}
        private:
        GLFWwindow* window;
        uint32_t WIDTH = 800;
        uint32_t HEIGHT = 600;
        bool framebufferResized = false;
    };
}