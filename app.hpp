#pragma once

// #include "descriptor.hpp"
// #include "device.hpp"
// #include "model.hpp"
// #include "pipeline.hpp"
// #include "renderer.hpp"
#include "renderer.hpp"
// #include "texture.hpp"

#include <memory>
#include <chrono>

namespace mv{
    class App{
        public:
        void run();
        void mainLoop();
        void drawFrame();
        void updateUniformBuffer(uint32_t currentImage);
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        private:
        // std::unique_ptr<Window> win = std::make_unique<Window>();
        // std::unique_ptr<Device> dev = std::make_unique<Device>();
        // std::unique_ptr<Swapchain> sc = std::make_unique<Swapchain>();
        // std::unique_ptr<Layout> lay = std::make_unique<Layout>();
        // std::shared_ptr<Renderer> ren = std::make_shared<Renderer>(dev, win, sc);
        // std::shared_ptr<Pipeline> pipe = std::make_shared<Pipeline>(dev, sc, des);
        // std::shared_ptr<Texture> tex = std::make_shared<Texture>("viking_room.png", dev, sc);
        // std::shared_ptr<Model> mod = std::make_shared<Model>("viking_room.obj", dev, sc);
        // std::shared_ptr<Descriptor> des = std::make_shared<Descriptor>(dev, sc, mod, tex);
        Window win;
        Device dev{win};
        Swapchain sc{win, dev};
        Layout lay{dev};
        Pipeline pipe{dev, sc, lay};
        Texture tex{"viking_room.png", dev, sc};
        Model mod{"viking_room.obj", dev, sc};
        Descriptor des{dev, sc, lay, tex, mod};
        Renderer ren{win, dev, sc};
        // Swapchain sc;
        // Renderer& ren;
        // Pipeline& pipe;
        // Texture& tex;
        // Model& mod;
        // Descriptor& des;
    };
}