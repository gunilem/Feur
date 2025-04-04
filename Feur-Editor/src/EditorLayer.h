#pragma once

#include <Feur.h>




namespace FeurEditor {
	class EditorLayer : public Feur::Layer {
	public:
		EditorLayer();

		virtual void OnUpdate() override;
		virtual const std::string& GetName() const override { return name;  }

	private:
		std::string name;
	};

}
