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
        // std::shared_ptr<Device> dev = std::make_shared<Device>();
        // std::shared_ptr<Swapchain> sc = std::make_shared<Swapchain>();
        std::vector<VkCommandBuffer> commandBuffers;
        // std::shared_ptr<Window> win = std::make_shared<Window>();
    };
}