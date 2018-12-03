// Horizontal blur shader
#include "MotionBlurShader.h"


MotionBlurShader::MotionBlurShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"motionBlur_vs.cso", L"motionBlur_ps.cso");
}


MotionBlurShader::~MotionBlurShader()
{
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
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
	if (screenSizeBuffer)
	{
		screenSizeBuffer->Release();
		screenSizeBuffer = 0;
	}
	if (viewProjBuffer)
	{
		viewProjBuffer->Release();
		viewProjBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void MotionBlurShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC screenSizeBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);


	DXUtility::CreateBufferDesc(sizeof(MatrixBufferType), &matrixBuffer, renderer);
	DXUtility::CreateBufferDesc(sizeof(ViewProjBufferType), &viewProjBuffer, renderer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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

	// Setup the description of the screen size.
	screenSizeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	screenSizeBufferDesc.ByteWidth = sizeof(BlurBufferType);
	screenSizeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	screenSizeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	screenSizeBufferDesc.MiscFlags = 0;
	screenSizeBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&screenSizeBufferDesc, NULL, &screenSizeBuffer);



}


void MotionBlurShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* sceneTexture, ID3D11ShaderResourceView* depthTexture, float samples, XMVECTOR cameraRotationVector)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	XMVECTOR dot = XMVector3Dot(cameraRotationVector, prevCameraVector);
	XMFLOAT4 v2F;    //the float where we copy the v2 vector members
	XMStoreFloat4(&v2F, dot);
	
	//samples *= acosf(v2F.x);
	
	//Additional
	BlurBufferType* blurPtr;
	deviceContext->Map(screenSizeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	blurPtr = (BlurBufferType*)mappedResource.pData;
	blurPtr->numSamples = samples;
	blurPtr->padding = XMFLOAT3(1.0f, 1.f, 1.f);
	deviceContext->Unmap(screenSizeBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &screenSizeBuffer);

	ViewProjBufferType* vpPtr;
	deviceContext->Map(viewProjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	vpPtr = (ViewProjBufferType*)mappedResource.pData;
	vpPtr->viewMatrix = viewMatrix;
	vpPtr->projectionMatrix = projectionMatrix;
	vpPtr->prevViewMatrix = prevProjectionMatrix;
	vpPtr->prevProjectionMatrix = prevProjectionMatrix;
	deviceContext->Unmap(viewProjBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &viewProjBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &sceneTexture);
	deviceContext->PSSetShaderResources(1, 1, &depthTexture);
	deviceContext->PSSetSamplers(0, 1, &sampleState);

	prevCameraVector = cameraRotationVector;

	prevProjectionMatrix = projectionMatrix;
	prevViewMatrix = viewMatrix;
}



