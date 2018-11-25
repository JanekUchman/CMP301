#pragma once
#include "DXF.h"
//Utility functions to save time in the shader classes
class DXUtility
{
public:
	//Sets buffer description up, saves code repetition
	static void CreateBuffer(std::size_t size, ID3D11Buffer** buffer, ID3D11Device* renderer);
};

