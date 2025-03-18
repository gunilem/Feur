#include "fpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Feur {
	PerspectiveCamera::PerspectiveCamera(float FOV, float aspectRatio, float nearClipPlane, float farClipPlane)
		: Camera(nearClipPlane, farClipPlane), m_FOV(FOV), m_AspectRatio(aspectRatio)
	{
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_NearClipPlane, m_FarClipPlane);
		m_ViewMatrix = 1.0f;

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}