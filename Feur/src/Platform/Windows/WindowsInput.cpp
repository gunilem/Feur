#include "fpch.h"
#include "Feur/Core/Input.h"

#include "Feur/Core/Application.h"
#include <glfw/glfw3.h>

namespace Feur {
	bool Input::IsKeyPressed(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}



	glm::vec2 Input::GetMousePosition() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;

		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos,(float)ypos };
	}

	float Input::GetMouseX() {
		glm::vec2 position = GetMousePosition();

		return position.x;
	}
	float Input::GetMouseY() {
		glm::vec2 position = GetMousePosition();

		return position.y;
	}
}