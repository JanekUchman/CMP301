#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class DisplacementShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 position;
		float padding;
	};

	struct TimeBufferType
	{
		float time;
		float height;
		float frequency;
		float speed;
	};


	Timer timer;
	float time = 0;
public:
	DisplacementShader(ID3D11Device* device, HWND hwnd);
	~DisplacementShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
		ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* displacementMap, Light* light, float deltaTime, float height, float frequency, float speed);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateDisplacement;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* timeBuffer;
};

