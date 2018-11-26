// Light shader.h
// Geometry shader example.
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;


class ParticleShader : public BaseShader
{

public:

	ParticleShader(ID3D11Device* device, HWND hwnd);
	~ParticleShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light, XMFLOAT3 cameraPos);
	void render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* gsFilename, WCHAR* psFilename);

private:

	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float padding;
	};

	struct GeometryBufferType
	{
		XMFLOAT3 cameraPos;
		float padding;
	};

	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* geometryBuffer;
	ID3D11SamplerState* sampleState;
};
