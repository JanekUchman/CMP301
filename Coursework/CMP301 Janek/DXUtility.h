#pragma once
#include "DXF.h"
#include "LightBuffers.h"
//Utility functions to save time in the shader classes
class DXUtility
{
public:
	//Sets buffer description up, saves code repetition
	static void CreateBufferDesc(std::size_t size, ID3D11Buffer** buffer, ID3D11Device* renderer);
	static void CreateSamplerDesc(ID3D11SamplerState** sampler, ID3D11Device* renderer);
	static void CreateTextureSamplerDesc(ID3D11SamplerState** sampler, ID3D11Device* renderer);
};

