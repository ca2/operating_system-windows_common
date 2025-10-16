// From github:/tristancalderbank/OpenGL-PBR-Renderer/vertex.h by
// camilo on 2025-09-26 18:35 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 16:45 <3ThomasBorregaardSorensen!!
#pragma once


namespace gpu_directx11
{


namespace gltf
{

/**
 * gltf::vertex attributes.
 */
struct gltf::vertex {
   glm::vec3 mPosition;
   glm::vec3 mNormal;
   glm::vec2 mTextureCoordinates;
   glm::vec3 mTangent;
   glm::vec3 mBitangent;
};


} // namespace gltf


} // namespace gpu_directx11

