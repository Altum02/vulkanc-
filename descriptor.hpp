#pragma once

// #include "device.hpp"
// #include "swapchain.hpp"
#include "model.hpp"
// #include "texture.hpp"

#include <memory>

namespace mv{
    class Descriptor{
        public:
        Descriptor(Device dev, Swapchain sc, Layout lay, Texture tex, Model mod);
        ~Descriptor();
        void createDescriptorPool();
        void createDescriptorSets();
        std::vector<VkDescriptorSet> getDescriptorSets(){return descriptorSets;}
        private:
        Device dev;
        Swapchain sc;
        Model mod;
        Texture tex;
        Layout lay;
        // std::shared_ptr<Device> dev = std::make_shared<Device>();
        // std::shared_ptr<Swapchain> sc = std::make_shared<Swapchain>();
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        // VkDescriptorBufferInfo bufferInfo{};
        // std::shared_ptr<Model> modptr = std::make_shared<Model>();
        // std::shared_ptr<Texture> texptr = std::make_shared<Texture>();
        // // std::string texture;
        // std::string model;
    };
}