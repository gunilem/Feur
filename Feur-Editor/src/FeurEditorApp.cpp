#include "FeurEditorApp.h"
#include "EditorLayer.h"

#include <Feur/Core/EntryPoint.h>

namespace FeurEditor {
	
	//------Sandbox-------------
	FeurEditor::FeurEditor() {
		PushLayer(new EditorLayer());
	}

	FeurEditor::~FeurEditor() {

	}
	//-------------------------
}

Feur::Application* CreateApplication() {
	return new FeurEditor::FeurEditor();
}