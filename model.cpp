#include "model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace mv{
    Model::Model(std::string MODEL_PATH, Device &dev, Swapchain &sc): dev{dev}, sc{sc}{
        // this->dev = dev;
        // this->sc = sc;
        loadModel(MODEL_PATH);
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
    }
    Model::~Model(){
        vkDestroyBuffer(dev.getDevice(), indexBuffer, nullptr);
        vkFreeMemory(dev.getDevice(), indexBufferMemory, nullptr);
        vkDestroyBuffer(dev.getDevice(), vertexBuffer, nullptr);
        vkFreeMemory(dev.getDevice(), vertexBufferMemory, nullptr);
        for (size_t i = 0; i < sc.getMFIF(); i++) {
            vkDestroyBuffer(dev.getDevice(), getUniformBuffers()[i], nullptr);
            vkFreeMemory(dev.getDevice(), getUniformBuffersMemory()[i], nullptr);
        }

    }
    void Model::loadModel(std::string MODEL_PATH) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
    void Model::createVertexBuffer() {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        dev.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(dev.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(dev.getDevice(), stagingBufferMemory);

        dev.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        dev.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(dev.getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(dev.getDevice(), stagingBufferMemory, nullptr);
    }
    void Model::createIndexBuffer() {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        dev.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(dev.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(dev.getDevice(), stagingBufferMemory);

        dev.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        dev.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(dev.getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(dev.getDevice(), stagingBufferMemory, nullptr);
    }
    void Model::createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(sc.getMFIF());
        uniformBuffersMemory.resize(sc.getMFIF());

        for (size_t i = 0; i < sc.getMFIF(); i++) {
            dev.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
        }
    }
}