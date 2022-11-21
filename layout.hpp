#pragma once

#include "swapchain.hpp"
#include <memory>

namespace mv{
    class Layout{
        public:
        Layout(Device &dev);
        ~Layout();
        Layout(const Layout &) = delete;
        Layout &operator=(const Layout &) = delete;
        Layout(Layout &&) = delete;
        Layout &operator=(Layout &&) = delete;

        void createDescriptorSetLayout();
        VkDescriptorSetLayout &getDescriptorSetLayout(){return descriptorSetLayout;}
        private:
        VkDescriptorSetLayout descriptorSetLayout;
        Device &dev;
        // std::shared_ptr<Device> dev = std::make_shared<Device>();
    };
}