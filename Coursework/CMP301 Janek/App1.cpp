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

	// Create Mesh objects
	billboardMesh = new MultiPointMesh(renderer->getDevice(), renderer->getDeviceContext());
	icosahedron = new Icosahedron(renderer->getDevice(), renderer->getDeviceContext());

	//Load texture
	textureMgr->loadTexture("brick", L"res/brick1.dds");

	//Create shaders
	geometryShader = new GeometryShader(renderer->getDevice(), hwnd);
	tessShader = new TessellationShader(renderer->getDevice(), hwnd);

	//Set values
	tessFactor = 6;

	//Set lights
	lights[0] = new Light;
	lights[1] = new Light;

	// Setup light 1
	lights[0]->setPosition(-3.0f, 3.0f, -3.0f);
	lights[0]->setLookAt(0.0f, 0.0f, 0.0f);
	lights[0]->setAmbientColour(0.15f, 0.15f, 0.15f, 1.0f);
	lights[0]->setDiffuseColour(1.0f, 0.8f,0.8f, 1.0f);
	lights[0]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	// setup light 2
	lights[1]->setPosition(-6.0f, 3.0f, -5.0f);
	lights[1]->setLookAt(0.0f, 0.0f, 0.0f);
	lights[1]->setDiffuseColour(0.5f, 0.6f, 0.5f, 1.0f);
	lights[1]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	//TODO deletes 
	delete lights[0];
	delete lights[1];
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
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data, set shader parameters, render object with shader
	/*mesh->sendData(renderer->getDeviceContext());
	geometryShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("brick"), light, camera->getPosition());
	geometryShader->render(renderer->getDeviceContext(), mesh->getIndexCount());*/

	icosahedron->sendData(renderer->getDeviceContext());
	tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tessFactor);
	tessShader->render(renderer->getDeviceContext(), icosahedron->getIndexCount());

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

	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

