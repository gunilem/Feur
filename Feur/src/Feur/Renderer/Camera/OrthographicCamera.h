#pragma once

#include "Camera.h"

namespace Feur {

	class OrthographicCamera : public Camera
	{
	public: 
		OrthographicCamera(float left, float right, float bottom, float top, float nearClipPlane = -1.0f, float farClipPlane = 1.0f);
		void SetProjection(float left, float right, float bottom, float top, float nearClipPlane = -1.0f, float farClipPlane = 1.0f);

		virtual const glm::vec3 GetPosition() const override { return m_Position;  }
		virtual const float GetRotation() const override { return m_Rotation; }

		virtual void SetPosition(glm::vec3 position) override { m_Position = position; RecalculateViewMatrix(); }
		virtual void SetRotation(float rotation) override { m_Rotation = rotation; RecalculateViewMatrix();}

		virtual const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

	protected:
		virtual void RecalculateViewMatrix() override;

	private:
		glm::mat4 m_ProjectionMatrix = 1.0f;
		glm::mat4 m_ViewMatrix = 1.0f;
		glm::mat4 m_ViewProjectionMatrix = 1.0f;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};

}
