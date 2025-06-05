#pragma once


#include "aura/graphics/gpu/frame.h"


namespace gpu_directx11
{


//#define MAX_LIGHTS 10
//
//	struct PointLight {
//		glm::vec4 position{};  // ignore w
//		glm::vec4 color{};     // w is intensity
//	};
//
//
//	//struct GlobalUbo {
//	//	glm::mat4 projection{ 1.f };
//	//	glm::mat4 view{ 1.f };
//	//	glm::mat4 inverseView{ 1.f };
//	//	glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
//	//	PointLight pointLights[MAX_LIGHTS];
//	//	int numLights;
//	//};

	class frame :
		virtual public ::gpu::frame
	{
	public:

		//int frameIndex;
		//float frameTime;
		//VkCommandBuffer commandBuffer;
		//::graphics3d::camera &camera;
		//VkDescriptorSet globalDescriptorSet;
		//::graphics3d::scene_object::map& gameObjects;
	};


} // namespace gpu_directx11 



