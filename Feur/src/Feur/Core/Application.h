#pragma once

#include "Feur/UI/Widget.h"
#include "Feur/Core/Device.h";
#include "Feur/Core/Pipeline.h"
#include "Feur/Core/Swap_chain.h";
#include "Feur/Core/Window.h"
#include "Feur/Core/Model.h"


namespace Feur {
	class Application
	{
	public:
		static Application* Create(const std::string& name);

		int Init();
		void Run();

		void Cleanup();



		void InitWindow();


		Application(const std::string& name);
		~Application();
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

	private:

		void LoadModels();

		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void freeCommandBuffer();
		void DrawFrame();
		void RecreateSwapChain();
		void RecordCommandBuffer(int imageIndex);

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		std::string m_ApplicationName;
		Window m_Window;
		Device m_Device;
		std::unique_ptr<SwapChain> m_SwapChain;
		std::unique_ptr<Pipeline> m_Pipeline;

		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		std::unique_ptr<Model> m_Model;

		std::shared_ptr<Widget> m_Rootwidget;
	};
}

