#include "Icosahedron.h"



Icosahedron::Icosahedron(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}


Icosahedron::~Icosahedron()
{
}
void Icosahedron::initBuffers(ID3D11Device* device, D3D_PRIMITIVE_TOPOLOGY top)
{
	//https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
	const float X=.525731112119133606f;
	const float Z=.850650808352039932f;
	const float N=0.f;

	vertexCount = 12;
	indexCount = 20;
	XMFLOAT3 verticesPosition[12]=
	{
	  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
	  {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
	  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
	};
 
	XMFLOAT3 triangleIndices[20]=
	{
	  {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
	  {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
	  {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
	  {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
	};

	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = verticesPosition[i];
	}

	for (int i = 0; i < indexCount; i ++)
	{
		indices[i] = i;
	}


	D3D11_SUBRESOURCE_DATA vertexData, indexData;


	D3D11_BUFFER_DESC vertexBufferDesc = { sizeof(VertexType) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	vertexData = { vertices, 0 , 0 };
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = { sizeof(unsigned long) * indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	indexData = { indices, 0, 0 };
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

// Send Geometry data to the GPU
void Icosahedron::sendData(ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY top)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType_Colour);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(top);
}

