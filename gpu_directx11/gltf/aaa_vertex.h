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
   floating_sequence3 mPosition;
   floating_sequence3 mNormal;
   floating_sequence2 mTextureCoordinates;
   floating_sequence3 mTangent;
   floating_sequence3 mBitangent;
};


} // namespace gltf


} // namespace gpu_directx11

