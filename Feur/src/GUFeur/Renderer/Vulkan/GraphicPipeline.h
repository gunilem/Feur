#pragma once

#include <fstream>

#include "GUFeur/Core/Core.h"
#include "Device.h"

namespace GUFeur {

	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error(std::string{ "échec de l'ouverture du fichier " } + filename + "!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	struct GraphicPipelineCreateInfo {
		GUF_DELETE_COPY_CONSTRUCTORS(GraphicPipelineCreateInfo)

		VkPipelineVertexInputStateCreateInfo VertexInputInfo;
		VkPipelineViewportStateCreateInfo ViewportInfo;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
		std::vector<VkDynamicState> DynamicStateEnables;
		VkPipelineDynamicStateCreateInfo DynamicStateInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkRenderPass RenderPass = nullptr;
		uint32_t Subpass = 0;
	};


	class GraphicPipeline
	{
	public:
		GraphicPipeline(Device& deviceRef, GraphicPipelineCreateInfo& pipelineCreateInfo);
		~GraphicPipeline() {}

		GUF_DELETE_COPY_CONSTRUCTORS(GraphicPipeline)

		void init();
		void cleanup();

		void bind(VkCommandBuffer commandBuffer);

		static void defaultGraphicPipelineConfigInfo(GraphicPipelineCreateInfo& configInfo);


	private:

#pragma region Creation
		void createGraphicsPipeline();
#pragma endregion

#pragma region Cleanup
		void cleanGraphicsPipeline();
#pragma endregion

		VkShaderModule createShaderModule(const std::vector<char>& code);


	private:
		Device& m_Device;
		GraphicPipelineCreateInfo& m_PipelineCreateInfo;
		VkPipeline m_GraphicsPipeline;
	};
}

