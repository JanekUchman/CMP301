#include "DisplacementShader.h"

DisplacementShader::DisplacementShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tessellation_vs.cso", L"tessellation_hs.cso", L"tessellation_ds.cso", L"tessellation_ps.cso");
}


DisplacementShader::~DisplacementShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	if (sampleStateDisplacement)
	{
		sampleStateDisplacement->Release();
		sampleStateDisplacement = 0;
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

	if (colourBuffer)
	{
		colourBuffer->Release();
		colourBuffer = 0;
	}

	if (tessellationBuffer)
	{
		tessellationBuffer->Release();
		tessellationBuffer = 0;
	}


	//Release base shader components
	BaseShader::~BaseShader();
}

void DisplacementShader::initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
	initShader(vsFilename, psFilename);


	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
	
}

void DisplacementShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);


	DXUtility::CreateBufferDesc(sizeof(MatrixBufferType), &matrixBuffer, renderer);
	DXUtility::CreateBufferDesc(sizeof(TessellationBufferType), &tessellationBuffer, renderer);
	DXUtility::CreateBufferDesc(sizeof(TimeBufferType), &timeBuffer, renderer);
	DXUtility::CreateBufferDesc(sizeof(ColourBufferType), &colourBuffer, renderer);
	DXUtility::CreateSamplerDesc(&sampleState, renderer);
	DXUtility::CreateSamplerDesc(&sampleStateDisplacement, renderer);
}




void DisplacementShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, 
	ID3D11ShaderResourceView* displacementMap,  float deltaTime, float lavaColours[3], bool lavaInvert, float displacement, int tessFactor)
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
	deviceContext->DSSetConstantBuffers(0, 1, &matrixBuffer);

	result = deviceContext->Map(tessellationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	TessellationBufferType* tessPtr = (TessellationBufferType*)mappedResource.pData;
	tessPtr->tessellationFactor = tessFactor;
	tessPtr->padding = { 0,0, 0 };
	deviceContext->Unmap(tessellationBuffer, 0);
	deviceContext->HSSetConstantBuffers(0, 1, &tessellationBuffer);

	// Send time data to vertex shader
	TimeBufferType* timePtr;
	deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (TimeBufferType*)mappedResource.pData;
	time += deltaTime;
	timePtr->time = time;
	timePtr->displacement = displacement;
	timePtr->padding = { 0,0 };
	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->DSSetConstantBuffers(1, 1, &timeBuffer);

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
	deviceContext->DSSetShaderResources(0, 1, &displacementMap);
	deviceContext->DSSetSamplers(0, 1, &sampleStateDisplacement);

	deviceContext->PSSetShaderResources(0, 1, &displacementMap);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
}
