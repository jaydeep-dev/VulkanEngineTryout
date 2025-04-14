#pragma once

#include "LveCamera.h"

// lib
#include <vulkan/vulkan.h>

namespace lve {
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		LveCamera& camera;
	};
}
