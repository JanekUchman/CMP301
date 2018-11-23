#include "Icosahedron.h"



Icosahedron::Icosahedron(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	initBuffers(device);
}


Icosahedron::~Icosahedron()
{
}
void Icosahedron::Subdivide(unsigned long* indices, VertexType* vertices)
{
	
	meshData.Vertices.resize(0);
	meshData.Indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = inputCopy.Indices.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex v0 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 0]];
		Vertex v1 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 1]];
		Vertex v2 = inputCopy.Vertices[inputCopy.Indices[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vertex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.Position = XMFLOAT3(
			0.5f*(v0.Position.x + v1.Position.x),
			0.5f*(v0.Position.y + v1.Position.y),
			0.5f*(v0.Position.z + v1.Position.z));

		m1.Position = XMFLOAT3(
			0.5f*(v1.Position.x + v2.Position.x),
			0.5f*(v1.Position.y + v2.Position.y),
			0.5f*(v1.Position.z + v2.Position.z));

		m2.Position = XMFLOAT3(
			0.5f*(v0.Position.x + v2.Position.x),
			0.5f*(v0.Position.y + v2.Position.y),
			0.5f*(v0.Position.z + v2.Position.z));

		//
		// Add new geometry.
		//

		meshData.Vertices.push_back(v0); // 0
		meshData.Vertices.push_back(v1); // 1
		meshData.Vertices.push_back(v2); // 2
		meshData.Vertices.push_back(m0); // 3
		meshData.Vertices.push_back(m1); // 4
		meshData.Vertices.push_back(m2); // 5

		meshData.Indices.push_back(i * 6 + 0);
		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 5);

		meshData.Indices.push_back(i * 6 + 5);
		meshData.Indices.push_back(i * 6 + 4);
		meshData.Indices.push_back(i * 6 + 2);

		meshData.Indices.push_back(i * 6 + 3);
		meshData.Indices.push_back(i * 6 + 1);
		meshData.Indices.push_back(i * 6 + 4);
	}
}

void Icosahedron::initBuffers(ID3D11Device* device)
{
	//https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
	//https://books.google.co.uk/books?id=gj6TDgAAQBAJ&pg=PT450&lpg=PT450&dq=icosahedron+directx&source=bl&ots=qbgqeAxAKe&sig=JoptQJ0qZlSKLUSdCLWpc7Wmkpw&hl=en&sa=X&ved=2ahUKEwj3-bj1kujeAhUQC-wKHeJyC2MQ6AEwAXoECAIQAQ#v=onepage&q=icosahedron%20directx&f=false
	const float X=.525731112119133606f;
	const float Z=.850650808352039932f;
	const float N=0.f;
	
	vertexCount = 12;
	indexCount = 60;

	
	XMFLOAT3 verticesPosition[12]=
	{
	  {-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
	  {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
	  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
	};
 
	/*XMFLOAT3 triangleIndices[20]=
	{
	  {1,4,0},{4,9,0},{4,5,9},{8,5,4},{1,8,4},
	  {1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
	  {3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
	  {10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}
	};*/
	//XMFLOAT3 triangleIndices[20] =
	//{
	//  {0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
	//  {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
	//  {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
	//  {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
	//};

	int triangleIndices[60] =
	{
	1,4,0, 4,9,0, 4,5,9, 8,5,4, 1,8,4,
	1,10,8, 10,3,8, 8,3,5, 3,2,5, 3,7,2,
	3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
	10,1,6, 11,0,9, 2,11,9, 5,2,9, 11,2,7
	};


	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = verticesPosition[i];
	}

	for (int i = 0; i < indexCount; i ++)
	{
		indices[i] = triangleIndices[i];
	}

	/*for (int i = 0; i < indexCount; i+=3)
	{
		indices[i] = triangleIndices[i].x;
		indices[i+1] = triangleIndices[i].y;
		indices[i+2] = triangleIndices[i].z;

	}*/

	// Project vertices onto sphere and scale.
	for (int i = 0; i < 12; ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n =
			XMVector3Normalize(XMLoadFloat3(&vertices[i].position));
		// Project onto sphere.
		XMVECTOR p = radius * n;
		XMStoreFloat3(&vertices[i].position, p);
		XMStoreFloat3(&vertices[i].normal, n);
		// Derive texture coordinates from spherical coordinates.
		float theta = atan2f(vertices[i].position.z, vertices[i].position.x);
		// Put in [0, 2pi].
		if (theta < 0.0f)
			theta += XM_2PI;
		float phi = acosf(vertices[i].position.y / radius);
		vertices[i].texture.x = theta / XM_2PI;
		vertices[i].texture.y = phi / XM_PI;
		// Partial derivative of P with respect to theta
		vertices[i].normal.x = - radius * sinf(phi)*sinf(theta);
		vertices[i].normal.y = 0.0f;
		vertices[i].normal.z = +radius * sinf(phi)*cosf(theta);
		XMVECTOR T =
			XMLoadFloat3(&vertices[i].normal);
		XMStoreFloat3(&vertices[i].normal, XMVector3Normalize(T));
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

