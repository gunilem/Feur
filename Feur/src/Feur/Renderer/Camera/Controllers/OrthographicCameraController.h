#pragma once

#include "Feur/Renderer/Camera/OrthographicCamera.h"
#include "Feur/Core/Timestep.h"


#include "Feur/Events/ApplicationEvent.h"
#include "Feur/Events/MouseEvent.h"

namespace Feur {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate();
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		inline const OrthographicCamera& GetCamera() const { return m_Camera; }
		inline const float GetZoomLevel() const { return m_ZoomLevel; }

		inline void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResizes(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_RotationEnabled = false;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 2.5f, m_CameraRotationSpeed = 40.0f;
	};
}