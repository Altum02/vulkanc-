#include "app.hpp"
#include "device.hpp"

namespace mv{
    void App::run(){
        mainLoop();
    }
    void App::mainLoop() {
        while (!glfwWindowShouldClose(win.getWindow())) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(dev.getDevice());
    }
    void App::drawFrame() {
        vkWaitForFences(dev.getDevice(), 1, &sc.getIFF()[sc.getCurrentFrame()], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(dev.getDevice(), sc.getSwapChain(), UINT64_MAX, sc.getIAS()[sc.getCurrentFrame()], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            ren.recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(sc.getCurrentFrame());

        vkResetFences(dev.getDevice(), 1, &sc.getIFF()[sc.getCurrentFrame()]);

        vkResetCommandBuffer(ren.getCommandBuffers()[sc.getCurrentFrame()], /*VkCommandBufferResetFlagBits*/ 0);
        recordCommandBuffer(ren.getCommandBuffers()[sc.getCurrentFrame()], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {sc.getIAS()[sc.getCurrentFrame()]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &ren.getCommandBuffers()[sc.getCurrentFrame()];

        VkSemaphore signalSemaphores[] = {sc.getRFS()[sc.getCurrentFrame()]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(dev.getGraphicsQueue(), 1, &submitInfo, sc.getIFF()[sc.getCurrentFrame()]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {sc.getSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(dev.getPresentQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || win.getFramebufferResized()) {
            win.setFramebufferResizedFalse();
            ren.recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        sc.changeCurrentFrame();
    }
    void App::updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), sc.getSwapChainExtent().width / (float) sc.getSwapChainExtent().height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        void* data;
        vkMapMemory(dev.getDevice(), mod.getUniformBuffersMemory()[currentImage], 0, sizeof(ubo), 0, &data);
            memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(dev.getDevice(), mod.getUniformBuffersMemory()[currentImage]);
    }
    void App::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = sc.getRenderPass();
        renderPassInfo.framebuffer = sc.getSwapChainFrameBuffers()[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = sc.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe.getGraphicsPipeline());

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float) sc.getSwapChainExtent().width;
            viewport.height = (float) sc.getSwapChainExtent().height;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = sc.getSwapChainExtent();
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            VkBuffer vertexBuffers[] = {mod.getVertexBuffer()};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

            vkCmdBindIndexBuffer(commandBuffer, mod.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe.getPipelineLayout(), 0, 1, &des.getDescriptorSets()[sc.getCurrentFrame()], 0, nullptr);

            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mod.getIndices().size()), 1, 0, 0, 0);

            VkBuffer vertexBuffers2[] = {mod2.getVertexBuffer()};
            // VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets);

            vkCmdBindIndexBuffer(commandBuffer, mod2.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipe.getPipelineLayout(), 0, 1, &des2.getDescriptorSets()[sc.getCurrentFrame()], 0, nullptr);

            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mod2.getIndices().size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
    void App::cleanup() {
        sc.cleanupSwapChain();

        vkDestroyPipeline(dev.getDevice(), pipe.getGraphicsPipeline(), nullptr);
        vkDestroyPipelineLayout(dev.getDevice(), pipe.getPipelineLayout(), nullptr);
        vkDestroyRenderPass(dev.getDevice(), sc.getRenderPass(), nullptr);

        for (size_t i = 0; i < sc.getMFIF(); i++) {
            vkDestroyBuffer(dev.getDevice(), mod.getUniformBuffers()[i], nullptr);
            vkFreeMemory(dev.getDevice(), mod.getUniformBuffersMemory()[i], nullptr);
        }

        vkDestroyDescriptorPool(dev.getDevice(), des.getDescriptorPool(), nullptr);

        vkDestroySampler(dev.getDevice(), tex.getTextureSampler(), nullptr);
        vkDestroyImageView(dev.getDevice(), tex.getTextureImageView(), nullptr);

        vkDestroyImage(dev.getDevice(), tex.getTextureImage(), nullptr);
        vkFreeMemory(dev.getDevice(), tex.getTextureImageMemory(), nullptr);

        vkDestroyDescriptorSetLayout(dev.getDevice(), lay.getDescriptorSetLayout(), nullptr);

        vkDestroyBuffer(dev.getDevice(), mod.getIndexBuffer(), nullptr);
        vkFreeMemory(dev.getDevice(), mod.getIndexBufferMemory(), nullptr);

        vkDestroyBuffer(dev.getDevice(), mod.getVertexBuffer(), nullptr);
        vkFreeMemory(dev.getDevice(), mod.getVertexBufferMemory(), nullptr);

        for (size_t i = 0; i < sc.getMFIF(); i++) {
            vkDestroySemaphore(dev.getDevice(), sc.getRFS()[i], nullptr);
            vkDestroySemaphore(dev.getDevice(), sc.getIAS()[i], nullptr);
            vkDestroyFence(dev.getDevice(), sc.getIFF()[i], nullptr);
        }

        vkDestroyCommandPool(dev.getDevice(), dev.getCommandPool(), nullptr);

        vkDestroyDevice(dev.getDevice(), nullptr);

        if (dev.getEnableValidationLayers()) {
            dev.DestroyDebugUtilsMessengerEXT(dev.getInstance(), dev.getDebug(), nullptr);
        }

        vkDestroySurfaceKHR(dev.getInstance(), dev.getSurface(), nullptr);
        vkDestroyInstance(dev.getInstance(), nullptr);

        glfwDestroyWindow(win.getWindow());

        glfwTerminate();
    }
}