// Horizontal blur shader handler
// Loads horizontal blur shaders (vs and ps)
// Passes screen width to shaders, for sample coordinate calculation
#pragma once

#include "DXF.h"
#include "DXUtility.h"

using namespace std;
using namespace DirectX;

class DepthOfFieldShader : public BaseShader
{
	
public:

	DepthOfFieldShader(ID3D11Device* device, HWND hwnd);
	~DepthOfFieldShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* sceneTexture, ID3D11ShaderResourceView* blurTexture);

private:
	void initShader(WCHAR*, WCHAR*);

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};