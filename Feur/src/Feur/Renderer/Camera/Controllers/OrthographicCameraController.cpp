#include "fpch.h"
#include "OrthographicCameraController.h"

#include "Feur/Core/Input.h"
#include "Feur/Core/KeyCodes.h"

namespace Feur {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),  m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_RotationEnabled(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate()
	{
		F_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(F_KEY_D)) {
			m_CameraPosition.x += m_CameraTranslationSpeed * Time::GetDeltaTime();
		}
		else if (Input::IsKeyPressed(F_KEY_A)) {
			m_CameraPosition.x -= m_CameraTranslationSpeed * Time::GetDeltaTime();
		}
		if (Input::IsKeyPressed(F_KEY_S)) {
			m_CameraPosition.y -= m_CameraTranslationSpeed * Time::GetDeltaTime();
		}
		else if (Input::IsKeyPressed(F_KEY_W)) {
			m_CameraPosition.y += m_CameraTranslationSpeed * Time::GetDeltaTime();
		}

		m_Camera.SetPosition(m_CameraPosition);

		if (m_RotationEnabled) {

			if (Input::IsKeyPressed(F_KEY_R)) {
				m_CameraRotation += m_CameraRotationSpeed * Time::GetDeltaTime();
			}
			else if (Input::IsKeyPressed(F_KEY_T)) {
				m_CameraRotation -= m_CameraRotationSpeed * Time::GetDeltaTime();
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		F_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResizes));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		F_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResizes(WindowResizeEvent& e)
	{
		F_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}


	void OrthographicCameraController::OnResize(float width, float height) {
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
}