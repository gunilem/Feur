#include "fpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Feur/KeyCodes.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Feur/Application.h"

//Temp
#include <GLFW/glfw3.h>


namespace Feur {

    ImGuiKey KeycodeToImGui(int keycode, int scancode);

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init();
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMouvedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}



	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		
		io.AddMouseButtonEvent(e.GetMouseButton(), true);

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(e.GetMouseButton(), false);

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMouvedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(KeycodeToImGui(e.GetKeyCode(), 0), true);

		
		
		io.AddKeyEvent(ImGuiMod_Ctrl, ImGui::IsKeyDown(KeycodeToImGui(F_KEY_LEFT_CONTROL, 0)) || ImGui::IsKeyDown(KeycodeToImGui(F_KEY_RIGHT_CONTROL, 0)));
		io.AddKeyEvent(ImGuiMod_Shift, ImGui::IsKeyDown(KeycodeToImGui(F_KEY_LEFT_SHIFT, 0)) || ImGui::IsKeyDown(KeycodeToImGui(F_KEY_RIGHT_SHIFT, 0)));
		io.AddKeyEvent(ImGuiMod_Alt, ImGui::IsKeyDown(KeycodeToImGui(F_KEY_LEFT_ALT, 0)) || ImGui::IsKeyDown(KeycodeToImGui(F_KEY_RIGHT_ALT, 0)));
		io.AddKeyEvent(ImGuiMod_Super, ImGui::IsKeyDown(KeycodeToImGui(F_KEY_LEFT_SUPER, 0)) || ImGui::IsKeyDown(KeycodeToImGui(F_KEY_RIGHT_SUPER, 0)));
		
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddKeyEvent(KeycodeToImGui(e.GetKeyCode(), 0), false);

		return false;
	}

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {

        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter(e.GetKeyCode());

        return false;
    }

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}


    ImGuiKey KeycodeToImGui(int keycode, int scancode)
    {
        IM_UNUSED(scancode);
        switch (keycode)
        {
        case F_KEY_TAB: return ImGuiKey_Tab;
        case F_KEY_LEFT: return ImGuiKey_LeftArrow;
        case F_KEY_RIGHT: return ImGuiKey_RightArrow;
        case F_KEY_UP: return ImGuiKey_UpArrow;
        case F_KEY_DOWN: return ImGuiKey_DownArrow;
        case F_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case F_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case F_KEY_HOME: return ImGuiKey_Home;
        case F_KEY_END: return ImGuiKey_End;
        case F_KEY_INSERT: return ImGuiKey_Insert;
        case F_KEY_DELETE: return ImGuiKey_Delete;
        case F_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case F_KEY_SPACE: return ImGuiKey_Space;
        case F_KEY_ENTER: return ImGuiKey_Enter;
        case F_KEY_ESCAPE: return ImGuiKey_Escape;
        case F_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case F_KEY_COMMA: return ImGuiKey_Comma;
        case F_KEY_MINUS: return ImGuiKey_Minus;
        case F_KEY_PERIOD: return ImGuiKey_Period;
        case F_KEY_SLASH: return ImGuiKey_Slash;
        case F_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case F_KEY_EQUAL: return ImGuiKey_Equal;
        case F_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case F_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case F_KEY_WORLD_1: return ImGuiKey_Oem102;
        case F_KEY_WORLD_2: return ImGuiKey_Oem102;
        case F_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case F_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
        case F_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
        case F_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case F_KEY_NUM_LOCK: return ImGuiKey_NumLock;
        case F_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case F_KEY_PAUSE: return ImGuiKey_Pause;
        case F_KEY_KP_0: return ImGuiKey_Keypad0;
        case F_KEY_KP_1: return ImGuiKey_Keypad1;
        case F_KEY_KP_2: return ImGuiKey_Keypad2;
        case F_KEY_KP_3: return ImGuiKey_Keypad3;
        case F_KEY_KP_4: return ImGuiKey_Keypad4;
        case F_KEY_KP_5: return ImGuiKey_Keypad5;
        case F_KEY_KP_6: return ImGuiKey_Keypad6;
        case F_KEY_KP_7: return ImGuiKey_Keypad7;
        case F_KEY_KP_8: return ImGuiKey_Keypad8;
        case F_KEY_KP_9: return ImGuiKey_Keypad9;
        case F_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
        case F_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case F_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case F_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case F_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
        case F_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
        case F_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
        case F_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case F_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case F_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case F_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
        case F_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case F_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case F_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        case F_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
        case F_KEY_MENU: return ImGuiKey_Menu;
        case F_KEY_0: return ImGuiKey_0;
        case F_KEY_1: return ImGuiKey_1;
        case F_KEY_2: return ImGuiKey_2;
        case F_KEY_3: return ImGuiKey_3;
        case F_KEY_4: return ImGuiKey_4;
        case F_KEY_5: return ImGuiKey_5;
        case F_KEY_6: return ImGuiKey_6;
        case F_KEY_7: return ImGuiKey_7;
        case F_KEY_8: return ImGuiKey_8;
        case F_KEY_9: return ImGuiKey_9;
        case F_KEY_A: return ImGuiKey_A;
        case F_KEY_B: return ImGuiKey_B;
        case F_KEY_C: return ImGuiKey_C;
        case F_KEY_D: return ImGuiKey_D;
        case F_KEY_E: return ImGuiKey_E;
        case F_KEY_F: return ImGuiKey_F;
        case F_KEY_G: return ImGuiKey_G;
        case F_KEY_H: return ImGuiKey_H;
        case F_KEY_I: return ImGuiKey_I;
        case F_KEY_J: return ImGuiKey_J;
        case F_KEY_K: return ImGuiKey_K;
        case F_KEY_L: return ImGuiKey_L;
        case F_KEY_M: return ImGuiKey_M;
        case F_KEY_N: return ImGuiKey_N;
        case F_KEY_O: return ImGuiKey_O;
        case F_KEY_P: return ImGuiKey_P;
        case F_KEY_Q: return ImGuiKey_Q;
        case F_KEY_R: return ImGuiKey_R;
        case F_KEY_S: return ImGuiKey_S;
        case F_KEY_T: return ImGuiKey_T;
        case F_KEY_U: return ImGuiKey_U;
        case F_KEY_V: return ImGuiKey_V;
        case F_KEY_W: return ImGuiKey_W;
        case F_KEY_X: return ImGuiKey_X;
        case F_KEY_Y: return ImGuiKey_Y;
        case F_KEY_Z: return ImGuiKey_Z;
        case F_KEY_F1: return ImGuiKey_F1;
        case F_KEY_F2: return ImGuiKey_F2;
        case F_KEY_F3: return ImGuiKey_F3;
        case F_KEY_F4: return ImGuiKey_F4;
        case F_KEY_F5: return ImGuiKey_F5;
        case F_KEY_F6: return ImGuiKey_F6;
        case F_KEY_F7: return ImGuiKey_F7;
        case F_KEY_F8: return ImGuiKey_F8;
        case F_KEY_F9: return ImGuiKey_F9;
        case F_KEY_F10: return ImGuiKey_F10;
        case F_KEY_F11: return ImGuiKey_F11;
        case F_KEY_F12: return ImGuiKey_F12;
        case F_KEY_F13: return ImGuiKey_F13;
        case F_KEY_F14: return ImGuiKey_F14;
        case F_KEY_F15: return ImGuiKey_F15;
        case F_KEY_F16: return ImGuiKey_F16;
        case F_KEY_F17: return ImGuiKey_F17;
        case F_KEY_F18: return ImGuiKey_F18;
        case F_KEY_F19: return ImGuiKey_F19;
        case F_KEY_F20: return ImGuiKey_F20;
        case F_KEY_F21: return ImGuiKey_F21;
        case F_KEY_F22: return ImGuiKey_F22;
        case F_KEY_F23: return ImGuiKey_F23;
        case F_KEY_F24: return ImGuiKey_F24;
        default: return ImGuiKey_None;
        }
    }

}