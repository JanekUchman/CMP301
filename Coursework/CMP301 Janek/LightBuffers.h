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
		XMMATRIX lightViewMatrix1;
		XMMATRIX lightViewMatrix2;

		XMMATRIX lightProjectionMatrix1;
		XMMATRIX lightProjectionMatrix2;

	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[3];
		XMFLOAT4 diffuse[3];
		XMFLOAT4 lightPosition[3];
		XMFLOAT4 direction[3];
	};
};

