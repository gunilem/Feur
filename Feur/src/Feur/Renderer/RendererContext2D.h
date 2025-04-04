#pragma once

namespace Feur {

	class RendererContext2D {
		virtual void MakeContext() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		
		virtual void DrawSprite() = 0;
	};
}