#pragma once

#include <glm/glm.hpp>

namespace Feur {

	class Camera
	{
	public:
		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

		virtual void SetPosition(glm::vec3 position) = 0;
		virtual void SetRotation(float rotation) = 0;

		virtual const glm::vec3 GetPosition() const = 0;
		virtual const float GetRotation() const = 0;

	protected:
		virtual void RecalculateViewMatrix() = 0;
	};

}
