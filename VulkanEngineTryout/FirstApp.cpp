#include "FirstApp.h"
#include "SimpleRenderSystem.h"
#include "LveCamera.h"
#include "Keyboard_Movement_Input.h"
#include "Lve_Buffer.h"

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

	struct GlobalUBO {
		glm::mat4 projectionView{ 1.0f };
		glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f, -1.0f});
		//alignas(16) glm::vec3 lightColor{ 1.0f, 1.0f, 1.0f };
	};;

	FirstApp::FirstApp()
	{
		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<Lve_Buffer>> uboBuffers(Lve_Swap_Chain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < uboBuffers.size(); i++)
		{
			uboBuffers[i] = std::make_unique<Lve_Buffer>(
				lveDevice,
				sizeof(GlobalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			uboBuffers[i]->map();
		}

		std::cout << "Max Push Constant Size: " << lveDevice.properties.limits.maxPushConstantsSize << std::endl;

		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };
		LveCamera camera{};

		auto viewerObject = LveGameObject::createGameObject();
		Keyboard_Movement_Input cameraController{};

		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();

			// FPS related
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto newTime = std::chrono::high_resolution_clock::now();
			auto frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			// Camera movement
			cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), viewerObject, frameTime);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			// Camera projection
			float aspect = lveRenderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 10);
			camera.setPerspectiveProjection(glm::radians(60.0f), aspect, 0.1f, 10.0f);

			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				int frameIndex = lveRenderer.GetFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera
				};

				// Update
				GlobalUBO ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo, sizeof(ubo));
				uboBuffers[frameIndex]->flush();

				// Render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameobjects(frameInfo, lveGameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects() {
		std::shared_ptr<LveModel> lveModel;

		lveModel = LveModel::createModelFromFile(lveDevice, "VulkanModels/smooth_vase.obj");
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