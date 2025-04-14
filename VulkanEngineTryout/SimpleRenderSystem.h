#pragma once

#include "Pipeline.h"
#include "LveDevice.h"
#include "LveGameObject.h"
#include "LveCamera.h"
#include "Lve_Frame_Info.h"

// Std
#include <memory>
#include <vector>

namespace lve {
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameobjects(FrameInfo& frameInfo, std::vector<LveGameObject>& gameObjects);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		LveDevice& lveDevice;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}

