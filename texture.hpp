#pragma once

// #include "device.hpp"
#include "pipeline.hpp"

#include <iostream>
#include <memory>

namespace mv{
    class Texture{
        public:
        Texture(std::string texturePath, Device &dev, Swapchain &sc);
        ~Texture();
        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;
        Texture(Texture &&) = delete;
        Texture &operator=(Texture &&) = delete;

        void createTextureImage(std::string TEXTURE_PATH);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
        void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
        void createTextureImageView();
        void createTextureSampler();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
        VkImageView &getTextureImageView(){return textureImageView;}
        VkSampler &getTextureSampler(){return textureSampler;}
        VkImage &getTextureImage(){return textureImage;}
        VkDeviceMemory &getTextureImageMemory(){return textureImageMemory;}
        private:
        uint32_t mipLevels;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;
        Device &dev;
        Swapchain &sc;
        // std::shared_ptr<Device> dev = std::make_shared<Device>();
        // std::shared_ptr<Swapchain> sc = std::make_shared<Swapchain>();
    };
}