#pragma once

#include "LveWindow.h"
#include "LveDevice.h"
#include "Lve_Swap_Chain.h"

// Std
#include <memory>
#include <cassert>

namespace lve {
	class LveRenderer
	{
	public:
		LveRenderer(LveWindow& lveWindow, LveDevice& lveDevice);
		~LveRenderer();

		LveRenderer(const LveRenderer&) = delete;
		LveRenderer& operator=(const LveRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); }

		float getAspectRatio() const { return lveSwapChain->extentAspectRatio(); }

		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer getCurrentCommandBuffer() const 
		{
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress.");
			return commandBuffers[currentFrameIndex];
		}

		int GetFrameIndex() const 
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress.");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();

		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		LveWindow& lveWindow;
		LveDevice& lveDevice;
		std::unique_ptr<Lve_Swap_Chain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{0};
		bool isFrameStarted;
	};
}

