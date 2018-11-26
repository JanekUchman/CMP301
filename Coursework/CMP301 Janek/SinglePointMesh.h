#pragma once
#include "BaseMesh.h"
class SinglePointMesh : public BaseMesh
{
public:
	SinglePointMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~SinglePointMesh();

	void sendData(ID3D11DeviceContext*);

	XMFLOAT3 updatePosition(float deltaTime);
	void setPosition(XMINT3 setPos);
	void setSpeed(XMINT3 startSpeed);
protected:
	void initBuffers(ID3D11Device* device);
	XMFLOAT3 position;
	XMINT3 startPos;
	XMINT3 startSpeed;
	XMINT3 speed;
};

