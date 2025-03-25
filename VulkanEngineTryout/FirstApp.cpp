#include "FirstApp.h"
#include "SimpleRenderSystem.h"
// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <iostream>
#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

	FirstApp::FirstApp()
	{
		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}

	void FirstApp::run()
	{
		std::cout << "Max Push Constant Size: " << lveDevice.properties.limits.maxPushConstantsSize << std::endl;

		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };

		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();

			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameobjects(commandBuffer, lveGameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::sierpinski(
		std::vector<LveModel::Vertex>& vertices,
		int depth,
		glm::vec2 left,
		glm::vec2 right,
		glm::vec2 top) {
		if (depth <= 0) {
			vertices.push_back({ {top}, {1.0f,.37f,.22f} });
			vertices.push_back({ {right}, {0.15f, 0.5f, 0.85f} });
			vertices.push_back({ {left}, { .42f, .6f, .1f } });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);
			sierpinski(vertices, depth - 1, left, leftRight, leftTop);
			sierpinski(vertices, depth - 1, leftRight, right, rightTop);
			sierpinski(vertices, depth - 1, leftTop, rightTop, top);
		}
	}
	void FirstApp::loadGameObjects() {
		//std::vector<LveModel::Vertex> vertices{ {{ -0.9f, 0.9f }, {1,0,0}}, {{ 0.9f, 0.9f }, {0,1,0}}, {{ 0.0f, -0.9f },{0,0,1}} };
		std::vector<LveModel::Vertex> vertices{};
		sierpinski(vertices, 2, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
		auto lveGameModel = std::make_shared<LveModel>(lveDevice, vertices);

		auto triangle = LveGameObject::createGameObject();
		triangle.model = lveGameModel;
		triangle.color = { 1.0f, 1.0f, 1.0f };
		triangle.transform2D.translation.x = .2f;
		triangle.transform2D.scale = { 1.f, 1.f };
		triangle.transform2D.rotation = 0.25f * glm::two_pi<float>();

		lveGameObjects.push_back(std::move(triangle));
	}
}