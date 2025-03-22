#pragma once

#include "Pipeline.h"
#include "LveWindow.h"
#include "LveDevice.h"
#include "Lve_Swap_Chain.h"
#include "LveGameObject.h"

// Std
#include <memory>

namespace lve {
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 1600;
		static constexpr int HEIGHT = 1200;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp &operator=(const FirstApp&) = delete;

		void run();

	private:
		void sierpinski(std::vector<LveModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top);
		void loadGameObjects();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameobjects(VkCommandBuffer commandBuffer);

		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		LveDevice lveDevice{ lveWindow };
		std::unique_ptr<Lve_Swap_Chain> lveSwapChain;
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<LveGameObject> lveGameObjects;
	};
}

