#pragma once

// #include "device.hpp"
#include "descriptor.hpp"
#include <memory>

namespace mv{
    class Renderer{
        public:
        Renderer(Window &win, Device &dev, Swapchain &sc);
        ~Renderer();
        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        void createCommandBuffers();
        void recreateSwapChain();
        std::vector<VkCommandBuffer> &getCommandBuffers(){return commandBuffers;}
        void freeCommandBuffers();
        private:
        Device &dev;
        Window &win;
        Swapchain &sc;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}