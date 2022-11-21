#pragma once

#include "device.hpp"

#include <limits>
#include <algorithm>

namespace mv{
    class Swapchain{
        public:
        Swapchain(Window &win, Device &dev);
        ~Swapchain();
        Swapchain(const Swapchain &) = delete;
        Swapchain &operator=(const Swapchain &) = delete;

        void createSwapChain();
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        void cleanupSwapChain();
        void createRenderPass();
        void createImageViews();
        void createDepthResources();
        void createColorResources();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
        VkFormat findDepthFormat();
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void createFramebuffers();
        void createSyncObjects();
        const int &getMFIF(){return MAX_FRAMES_IN_FLIGHT;}
        VkRenderPass &getRenderPass(){return renderPass;}
        std::vector<VkFence> &getIFF(){return inFlightFences;}
        uint32_t &getCurrentFrame(){return currentFrame;}
        VkSwapchainKHR &getSwapChain(){return swapChain;}
        VkExtent2D &getSwapChainExtent(){return swapChainExtent;}
        std::vector<VkSemaphore> &getIAS(){return imageAvailableSemaphores;}
        std::vector<VkFramebuffer> &getSwapChainFrameBuffers(){return swapChainFramebuffers;}
        void changeCurrentFrame(){currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;}
        std::vector<VkSemaphore> &getRFS(){return renderFinishedSemaphores;}
        
        private:
        Device &dev;
        Window &win;
        // std::shared_ptr<Device> dev = std::make_shared<Device>();
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        // std::shared_ptr<Window> win = std::make_shared<Window>();
        VkImage colorImage;
        VkDeviceMemory colorImageMemory;
        VkImageView colorImageView;
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
        VkRenderPass renderPass;
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        uint32_t currentFrame = 0;
        int MAX_FRAMES_IN_FLIGHT = 2;
    };
}