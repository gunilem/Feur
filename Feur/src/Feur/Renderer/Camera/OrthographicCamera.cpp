#include "fpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Feur {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearClipPlane, float farClipPlane)
		: Camera(nearClipPlane, farClipPlane)
	{ 
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_NearClipPlane, m_FarClipPlane);
		m_ViewMatrix = 1.0f;

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}