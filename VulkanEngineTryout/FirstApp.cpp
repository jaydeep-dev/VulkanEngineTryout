#include "FirstApp.h"
#include "SimpleRenderSystem.h"
#include "LveCamera.h"
#include "Keyboard_Movement_Input.h"

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <iostream>
#include <array>
#include <chrono>
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
		LveCamera camera{};

		auto viewerObject = LveGameObject::createGameObject();
		Keyboard_Movement_Input cameraController{};

		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();
			float aspect = lveRenderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 10);
			camera.setPerspectiveProjection(glm::radians(60.0f), aspect, 0.1f, 10.0f);

			auto currentTime = std::chrono::high_resolution_clock::now();

			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				auto newTime = std::chrono::high_resolution_clock::now();
				auto frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
				currentTime = newTime;

				cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), viewerObject, frameTime);
				camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameobjects(commandBuffer, lveGameObjects, camera);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects() {
		std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "VulkanModels/smooth_vase.obj");

		auto smoothVase = LveGameObject::createGameObject();
		smoothVase.model = lveModel;
		smoothVase.transform.translation = { -0.25f, 0.f, 2.5f };
		smoothVase.transform.scale = { 1.0f, 1.0f, 1.0f };
		lveGameObjects.push_back(std::move(smoothVase));

		lveModel = LveModel::createModelFromFile(lveDevice, "VulkanModels/flat_vase.obj");

		auto flatVase = LveGameObject::createGameObject();
		flatVase.model = lveModel;
		flatVase.transform.translation = { 0.25f, 0.f, 2.5f };
		flatVase.transform.scale = { 1.0f, 1.0f, 1.0f };
		lveGameObjects.push_back(std::move(flatVase));
	}
}