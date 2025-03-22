#pragma once

#include "LveModel.h"

// std
#include <memory>

namespace lve {

	struct Transform2DComponent {
		glm::vec2 translation{};
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation{ 0.0f };
		glm::mat2 mat2() 
		{
			const float cos = std::cos(rotation);
			const float sin = std::sin(rotation);
			glm::mat2 rotMat{ {cos, sin}, {-sin, cos} };
			glm::mat2 scaleMat{ {scale.x, 0}, {0, scale.y} };
			return rotMat * scaleMat;
		}
	};

	class LveGameObject
	{
	public:
		using id_t = unsigned int;

		static LveGameObject createGameObject() {
			static id_t currentId = 0;
			return LveGameObject(currentId++);
		}

		LveGameObject(const LveGameObject&) = delete;
		LveGameObject& operator=(const LveGameObject&) = delete;
		LveGameObject(LveGameObject&&) = default;
		LveGameObject& operator=(LveGameObject&&) = default;

		id_t getId() const { return id; }

		std::shared_ptr<LveModel> model{};
		glm::vec3 color{};
		Transform2DComponent transform2D{};

	private:
		LveGameObject(id_t objId) : id(objId) {}

		id_t id;
	};
}

