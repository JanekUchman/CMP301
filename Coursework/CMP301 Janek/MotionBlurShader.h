// Horizontal blur shader handler
// Loads horizontal blur shaders (vs and ps)
// Passes screen width to shaders, for sample coordinate calculation
#pragma once

#include "DXF.h"
#include "DXUtility.h"

using namespace std;
using namespace DirectX;

class MotionBlurShader : public BaseShader
{
private:
	struct BlurBufferType
	{
		float numSamples;
		XMFLOAT3 padding;
	};

	struct ViewProjBufferType
	{
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;

		XMMATRIX prevViewMatrix;
		XMMATRIX prevProjectionMatrix;
	};

public:

	MotionBlurShader(ID3D11Device* device, HWND hwnd);
	~MotionBlurShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float samples);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenSizeBuffer;
	ID3D11Buffer* viewProjBuffer;

	XMMATRIX prevViewMatrix;
	XMMATRIX prevProjectionMatrix;
};