#pragma once

#include "DXF.h"
#include "DXUtility.h"

using namespace std;
using namespace DirectX;
//Handles the lava sphere displacement and tessellation shaders
class DisplacementTessellationShader : public BaseShader
{
	
public:
	DisplacementTessellationShader(ID3D11Device* device, HWND hwnd);
	~DisplacementTessellationShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
		ID3D11ShaderResourceView* displacementMap, float speed, float lavaColours[3], bool lavaInvert, float displacement, int tessFactor);

private:
	void initShader(WCHAR*, WCHAR*);
	void initShader(WCHAR*, WCHAR*, WCHAR*, WCHAR*);
	

	struct ColourBufferType
	{
		float red;
		float blue;
		float green;
		float invert;
	};
	//For calculating the displacement texture's position
	struct TimeBufferType
	{
		float time;
		float displacement;
		XMFLOAT2 padding;
	};

	struct TessellationBufferType
	{
		float tessellationFactor;
		XMFLOAT3 padding;
	};



	float time = 0;
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateDisplacement;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* colourBuffer;
	ID3D11Buffer* tessellationBuffer;
};

