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
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particleMesh[i] = new SinglePointMesh(renderer->getDevice(), renderer->getDeviceContext());
		particleMesh[i]->setPosition(XMINT3(10, 10, 10));
		particleMesh[i]->setSpeed(XMINT3(2, 2, 2));
	}
	icosahedronMesh = new IcosahedronMesh(renderer->getDevice(), renderer->getDeviceContext());
	floorMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	//Load texture
	textureMgr->loadTexture("brick", L"res/brick1.dds");
	textureMgr->loadTexture("lavaHM", L"res/lava_height_2.png");
	textureMgr->loadTexture("particle", L"res/light_particle_1.png");
	textureMgr->loadTexture("floor", L"res/floor_3.jpg");
	textureMgr->loadTexture("box", L"res/box_1.jpg");
	

	//Create shaders
	particleShader = new ParticleShader(renderer->getDevice(), hwnd);
	tessShader = new TessellationShader(renderer->getDevice(), hwnd);
	displacementShader = new DisplacementShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	//Set values
	tessFactor = 6;
	lavaFlowRate = 0.1f;
	lavaColours[0] = 0.5f;
	lavaColours[1] = 0.25f;
	lavaColours[2] = 0.25f;
	lavaInvert = true;
	renderBox = true;
	amountOfParticles = 20;
	numberOfMBlurSamples = 3;
	//Set lights
	lights[0] = new Light;
	lights[1] = new Light;

	//set up render textures
	int shadowmapWidth = 4096;
	int shadowmapHeight = 4096;
	int sceneWidth = 100;
	int sceneHeight = 100;

	shadowMap = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);

	// Setup light 1
	lights[0]->setPosition(-3.0f, 3.0f, -3.0f);
	lights[0]->setLookAt(0.0f, 0.0f, 0.0f);
	lights[0]->setAmbientColour(0.3f, 0.15f, 0.15f, 1.0f);
	lights[0]->setDiffuseColour(1.0f, 0.8f,0.8f, 1.0f);
	lights[0]->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	lights[0]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);
	// setup red light
	lights[1]->setPosition(-6.0f, 3.0f, -5.0f);
	lights[1]->setLookAt(0.0f, 0.0f, 0.0f);
	lights[1]->setDiffuseColour(1.0f, 0.3f, 0.3f, 1.0f);
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

	camera->update();

	DepthPass();

	ShadowPass();

	// Clear the scene. (default blue colour)
	//renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	renderer->beginScene(0.025f, 0.02f, 0.02f, 1.0f);
	// Generate the view matrix based on the camera's position.

	FinalPass();

	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::DepthPass()
{
	shadowMap->setRenderTarget(renderer->getDeviceContext());
	shadowMap->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	lights[0]->generateViewMatrix();
	XMMATRIX viewMatrix = lights[0]->getViewMatrix();
	XMMATRIX projectionMatrix = lights[0]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-50, -20, -50), XMMatrixScaling(0.1f, 0.1f, 0.1f));
	floorMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	depthShader->render(renderer->getDeviceContext(), floorMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	sphereMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());


	if (renderBox)
	{
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-3, -3, -6), XMMatrixScaling(0.5f, 0.5f, 0.5f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, 150, 0));
		cubeMesh->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	}
	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}
void App1::ShadowPass()
{
	//
	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	//XMMATRIX worldMatrix = renderer->getWorldMatrix();
	//XMMATRIX viewMatrix = camera->getViewMatrix();
	//XMMATRIX projectionMatrix = renderer->getProjectionMatrix();


	///*icosahedronMesh->sendData(renderer->getDeviceContext());
	//tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tessFactor, camera->getPosition());
	//tessShader->render(renderer->getDeviceContext(), icosahedronMesh->getIndexCount());*/
	//worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-50, -20, -50), XMMatrixScaling(0.1f, 0.1f, 0.1f));
	//floorMesh->sendData(renderer->getDeviceContext());
	//shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("floor"), shadowMap->getShaderResourceView(), lights[0]);
	//shadowShader->render(renderer->getDeviceContext(), floorMesh->getIndexCount());

	//worldMatrix = renderer->getWorldMatrix();
	//sphereMesh->sendData(renderer->getDeviceContext());
	//shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("lavaHM"), timer->getTime()*lavaFlowRate, lavaColours, lavaInvert);
	//shadowShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	//// Send geometry data, set shader parameters, render object with shader
	//for (int i = 0; i < amountOfParticles; i++)
	//{
	//	XMFLOAT3 particlePositon = particleMesh[i]->updatePosition(timer->getTime());
	//	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(particlePositon.x, particlePositon.y, particlePositon.z), XMMatrixScaling(0.15f, 0.15f, 0.15f));
	//	particleMesh[i]->sendData(renderer->getDeviceContext());
	//	particleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("particle"), lights[0], camera->getPosition());
	//	particleShader->render(renderer->getDeviceContext(), particleMesh[i]->getIndexCount());
	//}

	//if (renderBox)
	//{
	//	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-3, -3, -6), XMMatrixScaling(0.5f, 0.5f, 0.5f));
	//	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, 150, 0));
	//	cubeMesh->sendData(renderer->getDeviceContext());
	//	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("box"));
	//	textureShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	//}

	///*sphereMesh->sendData(renderer->getDeviceContext());
	//tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tessFactor, camera->getPosition());
	//tessShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());*/


}
void App1::FinalPass()
{
	

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();


	/*icosahedronMesh->sendData(renderer->getDeviceContext());
	tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tessFactor, camera->getPosition());
	tessShader->render(renderer->getDeviceContext(), icosahedronMesh->getIndexCount());*/
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-50, -20, -50), XMMatrixScaling(0.1f, 0.1f, 0.1f));
	floorMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("floor"), shadowMap->getShaderResourceView(), lights[0]);
	shadowShader->render(renderer->getDeviceContext(), floorMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	sphereMesh->sendData(renderer->getDeviceContext());
	displacementShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("lavaHM"), timer->getTime()*lavaFlowRate, lavaColours, lavaInvert);
	displacementShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	// Send geometry data, set shader parameters, render object with shader
	for (int i = 0; i < amountOfParticles; i++)
	{
		XMFLOAT3 particlePositon = particleMesh[i]->updatePosition(timer->getTime());
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(particlePositon.x, particlePositon.y, particlePositon.z), XMMatrixScaling(0.15f, 0.15f, 0.15f));
		particleMesh[i]->sendData(renderer->getDeviceContext());
		particleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("particle"), lights[0], camera->getPosition(), lavaColours, lavaInvert);
		particleShader->render(renderer->getDeviceContext(), particleMesh[i]->getIndexCount());
	}

	if (renderBox)
	{
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-3, -3, -6), XMMatrixScaling(0.5f, 0.5f, 0.5f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, 150, 0));
		cubeMesh->sendData(renderer->getDeviceContext());
		textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("box"));
		textureShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	}

	/*sphereMesh->sendData(renderer->getDeviceContext());
	tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tessFactor, camera->getPosition());
	tessShader->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());*/

	
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
	//Variables
	ImGui::SliderFloat("Lava flow rate: ", &lavaFlowRate, -1, 1);
	ImGui::ColorPicker3("Lava colour", lavaColours);
	ImGui::Checkbox("Invert lava colours: ", &lavaInvert);
	ImGui::Checkbox("Render box: ", &renderBox);
	ImGui::SliderInt("Amount of particles: ", &amountOfParticles, 0, MAX_PARTICLES);
	ImGui::SliderInt("Blur samples: ", &numberOfMBlurSamples, 0, 10);

	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

