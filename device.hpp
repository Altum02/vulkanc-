#pragma once

#include "window.hpp"

#include <memory>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>
#include <set>
#include <optional>
#include <vulkan/vulkan_core.h>

namespace mv{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    class Device{
        public:
        Device(const Device &) = delete;
        Device &operator=(const Device &) = delete;
        Device(Device &&) = delete;
        Device &operator=(Device &&) = delete;
                // Device (const Device& ) = default;
        // Device (Device& ) = default;	
        Device(Window &win);
        ~Device();
        void createInstance();
        const bool &getEnableValidationLayers(){return  enableValidationLayers;}
        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        const std::vector<const char*> &getValidationLayers(){return validationLayers;}
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        void setupDebugMessenger();
        void createSurface();
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        void createLogicalDevice();
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSampleCountFlagBits getMaxUsableSampleCount();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        void createCommandPool();
        VkPhysicalDevice getPhysicalDevice(){return physicalDevice;}
        VkSurfaceKHR getSurface(){return surface;}
        VkDevice getDevice(){return device;}
        VkSampleCountFlagBits getMsaaSamples(){return msaaSamples;}
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        VkCommandPool getCommandPool(){return commandPool;}
        VkQueue getPresentQueue(){return presentQueue;}
        VkQueue getGraphicsQueue(){return graphicsQueue;}
        
        private:
        bool enableValidationLayers = true;
        std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
        VkDevice device;
        VkApplicationInfo appInfo{};
        VkInstanceCreateInfo createInfo{};
        const VkAllocationCallbacks* pAllocator = nullptr;
        Window &win;
        // std::shared_ptr<Window> win = std::make_shared<Window>();
        // std::shared_ptr<Window> win = std::make_shared<Window>();
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        VkCommandPool commandPool;
    };
}