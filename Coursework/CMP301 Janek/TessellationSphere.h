#pragma once
#include "DXF.h"
class TessellationSphere : public BaseMesh
{
public:
	TessellationSphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution);
	~TessellationSphere();
	void sendData(ID3D11DeviceContext*, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
protected:
	void initBuffers(ID3D11Device* device);
	int m_resolution;
};

