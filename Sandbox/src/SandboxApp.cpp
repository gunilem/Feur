#include <Feur.h>

class ExampleLayer : public Feur::Layer {
public:
	ExampleLayer()
		: Layer("Example") { }

	void OnUpdate() override {
		F_INFO("ExampleLayer::Update");
	}

	void OnEvent(Feur::Event& event) override {
		F_TRACE("{0}", event.ToString());
	}
};

class Sandbox : public Feur::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Feur::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Feur::Application* Feur::CreateApplication() {
	return new Sandbox();
}