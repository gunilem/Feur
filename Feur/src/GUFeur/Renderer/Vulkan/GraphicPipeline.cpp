#include "fpch.h"
#include "GraphicPipeline.h"

#include "VulkanAllocator.h"

namespace GUFeur {
	GraphicPipeline::GraphicPipeline(Device& deviceRef, GraphicPipelineCreateInfo& pipelineCreateInfo)
		: m_Device(deviceRef), m_PipelineCreateInfo(pipelineCreateInfo)
	{

	}

	void GraphicPipeline::init()
	{
		createGraphicsPipeline();
	}

	void GraphicPipeline::cleanup()
	{
		cleanGraphicsPipeline();
	}

	void GraphicPipeline::bind(VkCommandBuffer commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline); 
	}

	void GraphicPipeline::defaultGraphicPipelineConfigInfo(GraphicPipelineCreateInfo& configInfo)
	{
		configInfo.VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		configInfo.VertexInputInfo.vertexBindingDescriptionCount = 0;
		configInfo.VertexInputInfo.pVertexBindingDescriptions = nullptr; // Optionnel
		configInfo.VertexInputInfo.vertexAttributeDescriptionCount = 0;
		configInfo.VertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optionnel

		configInfo.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.ViewportInfo.viewportCount = 1;
		configInfo.ViewportInfo.scissorCount = 1;

		configInfo.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.RasterizationInfo.lineWidth = 1.0f;
		configInfo.RasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		configInfo.RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f; // Optionnel
		configInfo.RasterizationInfo.depthBiasClamp = 0.0f; // Optionnel
		configInfo.RasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optionnel

		configInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.MultisampleInfo.minSampleShading = 1.0f; // Optionnel
		configInfo.MultisampleInfo.pSampleMask = nullptr; // Optionnel
		configInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optionnel
		configInfo.MultisampleInfo.alphaToOneEnable = VK_FALSE; // Optionnel

		configInfo.ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.ColorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optionnel
		configInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optionnel
		configInfo.ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optionnel
		configInfo.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optionnel
		configInfo.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optionnel
		configInfo.ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optionnel

		configInfo.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.ColorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optionnel
		configInfo.ColorBlendInfo.attachmentCount = 1;
		configInfo.ColorBlendInfo.pAttachments = &configInfo.ColorBlendAttachment;
		configInfo.ColorBlendInfo.blendConstants[0] = 0.0f; // Optionnel
		configInfo.ColorBlendInfo.blendConstants[1] = 0.0f; // Optionnel
		configInfo.ColorBlendInfo.blendConstants[2] = 0.0f; // Optionnel
		configInfo.ColorBlendInfo.blendConstants[3] = 0.0f; // Optionnel

		configInfo.DynamicStateEnables = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_LINE_WIDTH
		};

		configInfo.DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.DynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.DynamicStateEnables.size());
		configInfo.DynamicStateInfo.pDynamicStates = configInfo.DynamicStateEnables.data();


	}

	void GraphicPipeline::createGraphicsPipeline()
	{
		auto vertShaderCode = readFile("assets/shader/compiled/baseShader.vert.spv");
		auto fragShaderCode = readFile("assets/shader/compiled/baseShader.frag.spv");

		auto vertShaderModule = createShaderModule(vertShaderCode);
		auto fragShaderModule = createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &m_PipelineCreateInfo.VertexInputInfo;
		pipelineInfo.pInputAssemblyState = &m_PipelineCreateInfo.InputAssemblyInfo;
		pipelineInfo.pViewportState = &m_PipelineCreateInfo.ViewportInfo;
		pipelineInfo.pRasterizationState = &m_PipelineCreateInfo.RasterizationInfo;
		pipelineInfo.pMultisampleState = &m_PipelineCreateInfo.MultisampleInfo;
		pipelineInfo.pDepthStencilState = nullptr; // Optionnel
		pipelineInfo.pColorBlendState = &m_PipelineCreateInfo.ColorBlendInfo;
		pipelineInfo.pDynamicState = nullptr; // Optionnel
		pipelineInfo.layout = m_PipelineCreateInfo.PipelineLayout;
		pipelineInfo.renderPass = m_PipelineCreateInfo.RenderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optionnel
		pipelineInfo.basePipelineIndex = -1; // Optionnel

		if (vkCreateGraphicsPipelines(m_Device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, GetCallback(), &m_GraphicsPipeline) != VK_SUCCESS) {
			throw std::runtime_error("échec de la création de la pipeline graphique!");
		}


		vkDestroyShaderModule(m_Device.device(), fragShaderModule, GetCallback());
		vkDestroyShaderModule(m_Device.device(), vertShaderModule, GetCallback());
	}

	void GraphicPipeline::cleanGraphicsPipeline()
	{
		vkDestroyPipeline(m_Device.device(), m_GraphicsPipeline, GetCallback());
	}

	VkShaderModule GraphicPipeline::createShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(m_Device.device(), &createInfo, GetCallback(), &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("échec de la création d'un module shader!");
		}

		return shaderModule;
	}

}