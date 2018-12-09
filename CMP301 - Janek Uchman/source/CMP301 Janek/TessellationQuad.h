#pragma once
#include "TessellationMesh.h"


class TessellationQuad : public BaseMesh
{
public:
	TessellationQuad(ID3D11Device* device, ID3D11DeviceContext* deviceContext, XMFLOAT2 positionModifier);
	~TessellationQuad();

	void sendData(ID3D11DeviceContext*, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

protected:

	void initBuffers(ID3D11Device* device);

	VertexType_Colour* vertices = new VertexType_Colour[4];
	unsigned long* indices = new unsigned long[4];

};

