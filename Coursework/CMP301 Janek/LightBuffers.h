#pragma once
#include "DXF.h"
class LightBuffers
{
public:
	struct MatrixBufferType
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
		XMMATRIX lightViewMatrix[2];
		XMMATRIX lightProjectionMatrix[2];
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[3];
		XMFLOAT4 diffuse[3];
		XMFLOAT3 lightPosition[3];
		XMFLOAT3 direction[3];
		XMFLOAT2 padding;
	};
};

