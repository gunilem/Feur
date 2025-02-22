#include <Feur.h>

class Sandbox : public Feur::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Feur::Application* Feur::CreateApplication() {
	return new Sandbox();
}