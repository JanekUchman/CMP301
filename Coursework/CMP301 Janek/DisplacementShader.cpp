#include "DisplacementShader.h"

DisplacementShader::DisplacementShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"displacement_vs.cso", L"displacement_ps.cso");
}


DisplacementShader::~DisplacementShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

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

	
	if (timeBuffer)
	{
		timeBuffer->Release();
		timeBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void DisplacementShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	DXUtility::CreateBufferDesc(sizeof(MatrixBufferType), &matrixBuffer, renderer);
	DXUtility::CreateBufferDesc(sizeof(TimeBufferType), &timeBuffer, renderer);
	DXUtility::CreateBufferDesc(sizeof(ColourBufferType), &colourBuffer, renderer);
	DXUtility::CreateSamplerDesc(&sampleState, renderer);
	DXUtility::CreateSamplerDesc(&sampleStateDisplacement, renderer);
	
}




void DisplacementShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, 
	ID3D11ShaderResourceView* displacementMap,  float deltaTime, float lavaColours[3], bool lavaInvert)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// Send time data to vertex shader
	TimeBufferType* timePtr;
	deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (TimeBufferType*)mappedResource.pData;
	time += deltaTime;
	timePtr->time = time;
	timePtr->padding = { 0,0,0 };
	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &timeBuffer);

	// Send time data to vertex shader
	ColourBufferType* colourPtr;
	deviceContext->Map(colourBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	colourPtr = (ColourBufferType*)mappedResource.pData;
	colourPtr->red = lavaColours[0];
	colourPtr->green = lavaColours[1];
	colourPtr->blue = lavaColours[2];
	colourPtr->invert = lavaInvert ? 1 : 0;
	deviceContext->Unmap(colourBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &colourBuffer);


	// Set shader texture resource in the pixel shader.
	deviceContext->VSSetShaderResources(0, 1, &displacementMap);
	deviceContext->VSSetSamplers(0, 1, &sampleStateDisplacement);

	deviceContext->PSSetShaderResources(0, 1, &displacementMap);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}
