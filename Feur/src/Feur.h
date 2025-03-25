#pragma once

// For use by Feur applications

#include "Feur/Core/Application.h"
#include "Feur/Core/Layer.h"
#include "Feur/Core/Log.h"

#include "Feur/Core/Timestep.h"

#include "Feur/Debug/Instrumentor.h"

//----Inputs--------------------
#include "Feur/Core/Input.h"
#include "Feur/Core/KeyCodes.h"
#include "Feur/Core/MousebuttonCodes.h"

#include "Feur/Events/Event.h"
#include "Feur/Events/KeyEvent.h"
//--------------------------------


//----Renderer-------------------
#include "Feur/Renderer/Renderer.h"
#include "Feur/Renderer/Renderer2D.h"
#include "Feur/Renderer/RenderCommand.h"

#include "Feur/Renderer/Buffer.h"
#include "Feur/Renderer/FrameBuffer.h"
#include "Feur/Renderer/Shader.h"
#include "Feur/Renderer/Texture.h"
#include "Feur/Renderer/VertexArray.h"

#include "Feur/Renderer/Camera/Controllers/OrthographicCameraController.h"

//--------------------------------

#include "Feur/Imgui/ImGuiLayer.h"