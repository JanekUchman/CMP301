// texture shader.cpp
#include "shadowshader.h"


ShadowShader::ShadowShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"shadow_vs.cso", L"shadow_ps.cso");
}


ShadowShader::~ShadowShader()
{
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}
	if (sampleStateShadow)
	{
		sampleStateShadow->Release();
		sampleStateShadow = 0;
	}
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	if (lightBuffer)
	{	
		lightBuffer->Release();
		lightBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void ShadowShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_SAMPLER_DESC shadowSamplerDesc;


	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);
	DXUtility::CreateBufferDesc(sizeof(MatrixBufferType), &matrixBuffer, renderer);
	DXUtility::CreateBufferDesc(sizeof(LightBuffers::MatrixBufferType), &shadowBuffer, renderer);
	DXUtility::CreateBufferDesc(sizeof(LightBuffers::LightBufferType), &lightBuffer, renderer);
	
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Sampler for shadow map sampling.
	shadowSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSamplerDesc.BorderColor[0] = 1.0f;
	shadowSamplerDesc.BorderColor[1] = 1.0f;
	shadowSamplerDesc.BorderColor[2] = 1.0f;
	shadowSamplerDesc.BorderColor[3] = 1.0f;
	renderer->CreateSamplerState(&shadowSamplerDesc, &sampleStateShadow);

}


void ShadowShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMap1, ID3D11ShaderResourceView* depthMap2, Light* lights[3])
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBuffers::MatrixBufferType* shadowPtr;
	LightBuffers::LightBufferType* lightPtr;
	
	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);
	
	//Get the lights view/proj matrices
	XMMATRIX tLightViewMatrix1 = XMMatrixTranspose(lights[0]->getViewMatrix());
	XMMATRIX tLightProjectionMatrix1 = XMMatrixTranspose(lights[0]->getOrthoMatrix());
	XMMATRIX tLightViewMatrix2 = (lights[1]->getViewMatrix());
	XMMATRIX tLightProjectionMatrix2 = XMMatrixTranspose(lights[1]->getOrthoMatrix());
	
	// Lock the constant buffer so it can be written to.
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	//Pass the view/proj matrices in in a seperate buffer
	//Not required, but helped to check debugging
	deviceContext->Map(shadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	shadowPtr = (LightBuffers::MatrixBufferType*)mappedResource.pData;
	shadowPtr->lightViewMatrix1 = tLightViewMatrix1;
	shadowPtr->lightProjectionMatrix1 = tLightProjectionMatrix1;
	shadowPtr->lightViewMatrix2 = tLightViewMatrix2;
	shadowPtr->lightProjectionMatrix2 = tLightProjectionMatrix2;
	deviceContext->Unmap(shadowBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &shadowBuffer);

	//Additional
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBuffers::LightBufferType*)mappedResource.pData;

	//Even though directional lights don't require position, and point don't require direction
	//Pass them in just so we're not passing a NULL
	lightPtr->ambient1 = lights[0]->getAmbientColour();
	lightPtr->diffuse1 = lights[0]->getDiffuseColour();
	lightPtr->lightDirection1 = XMFLOAT4(lights[0]->getDirection().x, lights[0]->getDirection().y, lights[0]->getDirection().z, 1);
	lightPtr->lightPosition1 = XMFLOAT4(lights[0]->getPosition().x, lights[0]->getPosition().y, lights[0]->getPosition().z, 1);

	lightPtr->ambient2 = lights[1]->getAmbientColour();
	lightPtr->diffuse2 = lights[1]->getDiffuseColour();
	lightPtr->lightDirection2 = XMFLOAT4(lights[1]->getDirection().x, lights[1]->getDirection().y, lights[1]->getDirection().z, 1);
	lightPtr->lightPosition2 = XMFLOAT4(lights[1]->getPosition().x, lights[1]->getPosition().y, lights[1]->getPosition().z, 1);

	lightPtr->ambient3 = lights[2]->getAmbientColour();
	lightPtr->diffuse3 = lights[2]->getDiffuseColour();
	lightPtr->lightDirection3 = XMFLOAT4(lights[2]->getDirection().x, lights[2]->getDirection().y, lights[2]->getDirection().z, 1);
	lightPtr->lightPosition3 = XMFLOAT4(lights[2]->getPosition().x, lights[2]->getPosition().y, lights[2]->getPosition().z, 1);
	
	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	// Set shader texture resource in the pixel shader.
	//Pass in both depth maps and the object's texture as they're all needed for the final colour
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &depthMap1);
	deviceContext->PSSetShaderResources(2, 1, &depthMap2);

	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateShadow);

}

