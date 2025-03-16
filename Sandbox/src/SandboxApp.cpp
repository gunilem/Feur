#include <Feur.h>

#include "imgui/imgui.h"

class ExampleLayer : public Feur::Layer {
public:
	ExampleLayer()
		: Layer("Example") { }

	void OnUpdate() override {
		//F_INFO("ExampleLayer::Update");
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();

	}

	void OnEvent(Feur::Event& event) override {
		//F_TRACE("{0}", event.ToString());
	}
};

class Sandbox : public Feur::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Feur::Application* Feur::CreateApplication() {
	return new Sandbox();
}