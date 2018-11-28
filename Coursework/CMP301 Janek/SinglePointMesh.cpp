#include "SinglePointMesh.h"

//TODO make this take more than 4 points

SinglePointMesh::SinglePointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}

// Release resources.
SinglePointMesh::~SinglePointMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}
void SinglePointMesh::setPosition(XMINT3 setPos)
{
	startPos = setPos;
	position = XMFLOAT3((rand() % startPos.x) - startPos.x/2, (rand() % startPos.y) - startPos.y, (rand() % startPos.z) - startPos.z/2);
}

void SinglePointMesh::setSpeed(XMINT3 setSpeed)
{
	startSpeed = setSpeed;
	speed = XMINT3((rand() % startSpeed.x) - startSpeed.x / 2, (rand() % startSpeed.y) + startSpeed.y, (rand() % startSpeed.z) - startSpeed.z / 2);
}

XMFLOAT3 SinglePointMesh::updatePosition(float deltaTime, float directionalSpeed)
{

	position.x += deltaTime*speed.x*directionalSpeed;
	position.y += deltaTime*speed.y*directionalSpeed;
	position.z += deltaTime*speed.z*directionalSpeed;

	if (abs(startPos.y - position.y) >= 20)
	{
		position = XMFLOAT3((rand() % startPos.x) - startPos.x / 2, (rand() % startPos.y) - startPos.y, (rand() % startPos.z) - startPos.z / 2);
		speed = XMINT3((rand() % startSpeed.x) - startSpeed.x / 2, (rand() % startSpeed.y) + startSpeed.y, (rand() % startSpeed.z) - startSpeed.z / 2);
	}
	return position;
}

// Build shape and fill buffers.
void SinglePointMesh::initBuffers(ID3D11Device* device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 1;
	indexCount = 1;

	VertexType_Colour* vertices = new VertexType_Colour[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);  // Top left
	vertices[0].colour = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Top/
	


	D3D11_BUFFER_DESC vertexBufferDesc = { sizeof(VertexType_Colour) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
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
void SinglePointMesh::sendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType_Colour);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}