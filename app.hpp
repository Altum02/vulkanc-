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
        void cleanup();

        private:
       
        Window win;
        Device dev{win};
        Swapchain sc{win, dev};
        Layout lay{dev};
        Pipeline pipe{dev, sc, lay};

        Texture tex{"viking_room.png", dev, sc};
        Model mod{"viking_room.obj", dev, sc};
        Descriptor des{dev, sc, lay, tex, mod};


        Texture tex2{"meme.png", dev, sc};
        Model mod2{"smooth_vase.obj", dev, sc};
        Descriptor des2{dev, sc, lay, tex2, mod2};






        Renderer ren{win, dev, sc};

    };
}