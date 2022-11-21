#pragma once

// #include "device.hpp"
// #include "descriptor.hpp"
#include "texture.hpp"
// #include "swapchain.hpp"

#include <memory>
#include <unordered_map>


namespace mv{
    

    class Model{
        public:
        Model(std::string MODEL_PATH, Device &dev, Swapchain &sc);
        ~Model();
        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;

        void loadModel(std::string MODEL_PATH);
        void createVertexBuffer();
        void createIndexBuffer();
        void createUniformBuffers();
        std::vector<VkBuffer> &getUniformBuffers(){return uniformBuffers;}
        std::vector<VkDeviceMemory> &getUniformBuffersMemory(){return uniformBuffersMemory;}
        std::vector<Vertex> &getVertices(){return vertices;}
        std::vector<uint32_t> &getIndices(){return indices;}
        VkBuffer &getVertexBuffer(){return vertexBuffer;}
        VkBuffer &getIndexBuffer(){return indexBuffer;}
        std::vector<VkBuffer> uniformBuffers;
        private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        Device &dev;
        Swapchain &sc;
        // std::shared_ptr<Device> dev = std::make_shared<Device>();
        // std::shared_ptr<Swapchain> sc = std::make_shared<Swapchain>();
        // friend class Descriptor;
    };
}