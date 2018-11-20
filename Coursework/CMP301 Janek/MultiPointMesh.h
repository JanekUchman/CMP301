#pragma once
#include "BaseMesh.h"
class MultiPointMesh : public BaseMesh
{
public:
	MultiPointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~MultiPointMesh();

	void sendData(ID3D11DeviceContext*);

protected:
	void initBuffers(ID3D11Device* device);

};

