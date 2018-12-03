#include "DepthShaderSphere.h"




DepthShaderSphere::DepthShaderSphere(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"displacement_vs.cso", L"depth_ps.cso");
}


DepthShaderSphere::~DepthShaderSphere()
{
	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void DepthShaderSphere::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	DXUtility::CreateBufferDesc(sizeof(MatrixBufferType), &matrixBuffer, renderer);
	DXUtility::CreateSamplerDesc(&sampleStateDisplacement, renderer);
	DXUtility::CreateBufferDesc(sizeof(TimeBufferType), &timeBuffer, renderer);

}


void DepthShaderSphere::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* displacementMap, float deltaTime, float displacement)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

	// Send time data to vertex shader
	TimeBufferType* timePtr;
	deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (TimeBufferType*)mappedResource.pData;
	time += deltaTime;
	timePtr->time = time;
	timePtr->displacement = displacement;
	timePtr->padding = { 0,0 };
	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &timeBuffer);

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->VSSetShaderResources(0, 1, &displacementMap);
	deviceContext->VSSetSamplers(0, 1, &sampleStateDisplacement);
}
