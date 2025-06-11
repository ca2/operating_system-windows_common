// Created by camilo on 2025-06-10 18:26 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "swap_chain.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


const char* fullscreen_vertex_shader = R"hlsl(// fullscreen_vs.hlsl
      struct VSOut {
         float4 pos : SV_POSITION;
         float2 uv : TEXCOORD0;
      };

      VSOut main(uint vid : SV_VertexID) {
         float2 verts[3] = {
             float2(-1, -1),
             float2(-1, +3),
             float2(+3, -1)
         };
         float2 uvs[3] = {
             float2(0, 1),
             float2(0, -1),
             float2(2, 1)
         };

         VSOut o;
         o.pos = float4(verts[vid], 0, 1);
         o.uv = 0.5 * (verts[vid] + 1.0);
         return o;
      }
)hlsl";

const char* fullscreen_pixel_shader = R"hlsl(// fullscreen_ps.hlsl
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target {
    return tex.Sample(samp, uv);
}
)hlsl";


namespace gpu_directx11
{



   swap_chain::swap_chain()
   {

   }


   swap_chain::~swap_chain()
   {

   }


   void swap_chain::_update_swap_chain(ID3D11Device * pdevice)
   {

		comptr<ID3DBlob> vsBlob;
		comptr<ID3DBlob> psBlob;
		comptr<ID3DBlob> errorBlob;
		// Vertex Shader
		HRESULT hr = D3DCompile(
			fullscreen_vertex_shader, strlen(fullscreen_vertex_shader),
			nullptr,                       // optional source name
			nullptr,                       // macro definitions
			nullptr,                       // include handler

			"main", "vs_5_0",
			0, 0,
			&vsBlob, &errorBlob
		);
		if (FAILED(hr)) {
			if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			return;
		}

		// Pixel Shader
		hr = D3DCompile(
			fullscreen_pixel_shader, strlen(fullscreen_vertex_shader),
			nullptr,                       // optional source name
			nullptr,                       // macro definitions
			nullptr,                       // include handler

			"main", "ps_5_0",
			0, 0,
			&psBlob, &errorBlob
		);
		if (FAILED(hr)) {
			if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			return;
		}

		pdevice->CreateVertexShader(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			nullptr,
			&m_pvertexshaderFullscreen
		);

		pdevice->CreatePixelShader(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			&m_ppixelshaderFullscreen
		);

		hr = pdevice->CreateShaderResourceView(
			m_ptextureShared, nullptr, &m_pshaderresourceviewShader);
		if (FAILED(hr)) {
			OutputDebugStringA("Failed to create SRV from shared D2D texture\n");
			return;
		}


		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = sampDesc.AddressV = sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		pdevice->CreateSamplerState(&sampDesc, &m_psamplerstateLinear);


   }


} // namespace gpu_directx11




