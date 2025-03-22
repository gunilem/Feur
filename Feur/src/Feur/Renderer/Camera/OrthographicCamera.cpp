#include "fpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Feur {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearClipPlane, float farClipPlane)
		: m_ViewMatrix(1.0f)
	{ 
		SetProjection(left, right, bottom, top, nearClipPlane, farClipPlane);
	}
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float nearClipPlane, float farClipPlane)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearClipPlane, farClipPlane);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}
}