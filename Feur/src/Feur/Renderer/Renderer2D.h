#pragma once

#include "Feur/Renderer/Camera/OrthographicCamera.h"
#include "Feur/Renderer/Texture.h"

namespace Feur {

	struct BatchData {
		uint32_t Batch;
		uint32_t Quads;

		void Reset() {
			Batch = 0;
			Quads = 0;
		}
	};


	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const float tillingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const float tillingFactor = 1.0f);

		static const BatchData& GetBatchData();

	private:
		static void Flush();
		static void ResetBatch();
	};
}