#pragma once


#include "bred/gpu/frame.h"
#include "bred/gpu/layer.h"


namespace gpu_directx11
{


//#define MAX_LIGHTS 10
//
//	struct PointLight {
//		floating_sequence4 position{};  // ignore w
//		floating_sequence4 color{};     // w is intensity
//	};
//
//
//	//struct GlobalUbo {
//	//	floating_matrix4 projection{ 1.f };
//	//	floating_matrix4 view{ 1.f };
//	//	floating_matrix4 inverseView{ 1.f };
//	//	floating_sequence4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
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
		//::graphics3d::scene_object::map_base& gameObjects;

		//array_base < ::comptr<ID3D11Buffer> > m_buffera;
		//int m_iBuffer;
		//int m_iBufferSize = 1_MiB;
		//int m_iBufferOffset;
	};


   // Created by camilo on 2025-08-15 21:51 <3ThomasBorregaardSørensen!!




} // namespace gpu_directx11 



