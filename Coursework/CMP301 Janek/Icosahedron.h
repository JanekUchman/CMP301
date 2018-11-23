#pragma once
#include <BaseMesh.h>

//http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

class Icosahedron : public BaseMesh
{
public:
	Icosahedron(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Icosahedron();

	void sendData(ID3D11DeviceContext*, D3D_PRIMITIVE_TOPOLOGY top = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


protected:
	void initBuffers(ID3D11Device* device);
	VertexType* vertices = new VertexType[12];
	unsigned long* indices = new unsigned long[60];
	float radius = 1;
	void Subdivide(unsigned long* indices, VertexType* vertices);
};

