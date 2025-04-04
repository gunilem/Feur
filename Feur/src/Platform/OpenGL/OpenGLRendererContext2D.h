#pragma once

#include "Feur/Renderer/RendererContext2D.h"

namespace Feur {

	class OpenGLRendererContext2D : public RendererContext2D
	{
		virtual void MakeContext() override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void DrawSprite() override;
	}; 
}

