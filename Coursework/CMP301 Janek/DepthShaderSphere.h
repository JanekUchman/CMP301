#pragma once
#include "DXF.h"
#include "DXUtility.h"

class DepthShaderSphere : public BaseShader
{
public:

	DepthShaderSphere(ID3D11Device* device, HWND hwnd);
	~DepthShaderSphere();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* displacementMap, float deltaTime, float displacement);

private:
	void initShader(WCHAR*, WCHAR*);

	struct TimeBufferType
	{
		float time;
		float displacement;
		XMFLOAT2 padding;
	};
	float time = 0;

	ID3D11SamplerState * sampleStateDisplacement;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer * matrixBuffer;
};

