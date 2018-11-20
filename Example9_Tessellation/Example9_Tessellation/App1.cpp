// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	XMFLOAT3 vertices[4] =
	{
		XMFLOAT3(10 , -1, -20),// top left
		XMFLOAT3(10, -1, 20), //bottom left
		XMFLOAT3(-10, -1, 20), //bottom right
		XMFLOAT3(-10, -1, -20) //top right

	};

	// Create Mesh object and shader object
	tessQuad = new TessellationQuad(renderer->getDevice(), renderer->getDeviceContext(), vertices, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	textureMgr->loadTexture("brick", L"res/brick1.dds");
	shader = new TessellationShader(renderer->getDevice(), hwnd);
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data, set shader parameters, render object with shader
	tessQuad->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), tessInside, tessEdge, camera->getPosition());
	shader->render(renderer->getDeviceContext(), tessQuad->getIndexCount());

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	ImGui::SliderInt("Edge tessellation value ", &tessEdge, minTess, maxTess);
	ImGui::SliderInt("Inside tessellation value ", &tessInside, minTess, maxTess);


	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

