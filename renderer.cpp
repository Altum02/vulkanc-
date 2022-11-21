#include "renderer.hpp"

namespace mv{
    Renderer::Renderer(Window &win, Device &dev, Swapchain &sc): win{win}, dev{dev}, sc{sc}{
        // this->win = win;
        // this->dev = dev;
        // this->sc = sc;
        createCommandBuffers();
    }
    Renderer::~Renderer(){
        freeCommandBuffers();
    }
    void Renderer::createCommandBuffers() {
        commandBuffers.resize(sc.getMFIF());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = dev.getCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        if (vkAllocateCommandBuffers(dev.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }
    void Renderer::recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(win.getWindow(), &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(win.getWindow(), &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(dev.getDevice());

        sc.cleanupSwapChain();

        sc.createSwapChain();
        sc.createImageViews();
        sc.createColorResources();
        sc.createDepthResources();
        sc.createFramebuffers();
    }
    void Renderer::freeCommandBuffers() {
        vkFreeCommandBuffers(
            dev.getDevice(),
            dev.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
        }
}