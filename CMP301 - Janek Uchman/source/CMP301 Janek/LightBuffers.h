#pragma once
#include "DXF.h"
//Created in case light buffers need to be used across multiple classes
//For example if an object required special shading and lighting
class LightBuffers
{
public:
	struct MatrixBufferType
	{


		XMMATRIX lightViewMatrix1;
		XMMATRIX lightProjectionMatrix1;

		XMMATRIX lightViewMatrix2;
		XMMATRIX lightProjectionMatrix2;


	};

	struct LightBufferType
	{
		XMFLOAT4 ambient1;
		XMFLOAT4 ambient2;
		XMFLOAT4 ambient3;

		XMFLOAT4 diffuse1;
		XMFLOAT4 diffuse2;
		XMFLOAT4 diffuse3;

		XMFLOAT4 lightPosition1;
		XMFLOAT4 lightPosition2;
		XMFLOAT4 lightPosition3;

		XMFLOAT4 lightDirection1;
		XMFLOAT4 lightDirection2;
		XMFLOAT4 lightDirection3;
	};
};

