#pragma once
#include "Camera.h"

namespace Feur {

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float FOV, float aspectRatio, float nearClipPlane = -1.0f, float farClipPlane = 1.0f);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		float m_FOV = 45.0f;
		float m_AspectRatio = 1.6f / 0.9f;
	};

}
