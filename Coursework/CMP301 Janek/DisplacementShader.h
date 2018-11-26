#pragma once

#include "DXF.h"
#include "DXUtility.h"

using namespace std;
using namespace DirectX;

class DisplacementShader : public BaseShader
{
	
public:
	DisplacementShader(ID3D11Device* device, HWND hwnd);
	~DisplacementShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
		ID3D11ShaderResourceView* displacementMap, float speed, float lavaColours[3], bool lavaInvert);

private:
	void initShader(WCHAR*, WCHAR*);
	
	struct ColourBufferType
	{
		float red;
		float blue;
		float green;
		float invert;
	};

	struct TimeBufferType
	{
		float time;
		XMFLOAT3 padding;
	};


	Timer timer;
	float time = 0;
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateDisplacement;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* colourBuffer;
};
