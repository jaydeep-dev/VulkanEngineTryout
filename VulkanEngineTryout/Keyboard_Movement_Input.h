#pragma once

#include "LveGameObject.h"
#include "LveWindow.h"

namespace lve {
	class Keyboard_Movement_Input
	{
	public:
		struct KeyMappings {
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;

			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;

			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		void moveInPlaneXZ(GLFWwindow* window, LveGameObject& gameObject, float deltaTime);

		KeyMappings keys{};
		float moveSpeed{ 30.0f };
		float lookSpeed{ 10.5f };
	};
}
