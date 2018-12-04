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
		//What rough area do we want the particles to start appearing
		particleMesh[i]->setPosition(XMINT3(10, 10, 10));
		//What their max speed (y is always either negative or positive)
		particleMesh[i]->setSpeed(XMINT3(2, 2, 2));
	}

	icosahedronMesh = new IcosahedronMesh(renderer->getDevice(), renderer->getDeviceContext());
	floorMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	depthOfFieldMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);

	//Load texture
	textureMgr->loadTexture("plasmaHM", L"res/plasma_height_2.png");
	textureMgr->loadTexture("particle", L"res/light_particle_1.png");
	textureMgr->loadTexture("floor", L"res/floor_3.jpg");
	textureMgr->loadTexture("box", L"res/box_2.jpg");
	

	//Create shaders
	particleShader = new ParticleShader(renderer->getDevice(), hwnd);
	displacementTessShader = new DisplacementTessellationShader(renderer->getDevice(), hwnd);
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	depthOfFieldShader = new DepthOfFieldShader(renderer->getDevice(), hwnd);
	depthShaderSphere = new DepthShaderSphere(renderer->getDevice(), hwnd);
	horizontalBlurShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	verticalBlurShader = new VerticalBlurShader(renderer->getDevice(), hwnd);

	//Set values
	tessFactor = 6;
	plasmaFlowRate = 0.1f;
	plasmaColours[0] = 0.5f;
	plasmaColours[1] = 0.25f;
	plasmaColours[2] = 0.25f;
	particleDirectionalSpeed = -1;
	time = 0;
	displacement = 0.1f;
	plasmaInvert = true;
	renderBox = true;
	amountOfParticles = 20;
	tessFactor = 16;
	//Set lights
	lights[0] = new Light;
	lights[1] = new Light;
	lights[2] = new Light;

	//set up render textures
	int shadowmapWidth = 4096;
	int shadowmapHeight = 4096;
	int sceneWidth = 20;
	int sceneHeight = 20;

	shadowMapTexture[directionalLight1] = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);
	shadowMapTexture[directionalLight2] = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);
	sceneTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	depthOfFieldTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	verticalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	horizontalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// Setup directional light
	lights[directionalLight1]->setPosition(0, 5, 5.0f);
	lights[directionalLight1]->setDirection(0.8f, -1.0f, -1.3f);
	lights[directionalLight1]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[directionalLight1]->setDiffuseColour(0.5f, 0.5f, 0.5f, 1.0f);
	lights[directionalLight1]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	// Setup directional light
	lights[directionalLight2]->setPosition(0, 5, -5.0f);
	lights[directionalLight2]->setDirection(0.8f, -1.0f, 0.9f);
	lights[directionalLight2]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[directionalLight2]->setDiffuseColour(0.5f, 0.5f, 0.5f, 1.0f);
	lights[directionalLight2]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	// setup plasma light
	lights[plasmaLight]->setPosition(-80.0f, 2.0f, 0.0f);
	lights[plasmaLight]->setDirection(-0.7f, -1.0f, 1.5f);
	lights[plasmaLight]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[plasmaLight]->setDiffuseColour(plasmaColours[0], plasmaColours[1], plasmaColours[2], plasmaColours[3]);
	lights[plasmaLight]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	//Delete lights
	delete lights[0];
	delete lights[1];
	delete lights[2];
	//Delete shaders
	delete particleShader;
	delete displacementTessShader;
	delete textureShader;
	delete shadowShader;
	delete depthShader;
	delete depthOfFieldShader;
	delete depthShaderSphere;
	delete horizontalBlurShader;
	delete verticalBlurShader;
	//delete meshes
	for (int i = 0; i < MAX_PARTICLES; i ++)
		delete particleMesh[i];
	delete icosahedronMesh;
	delete floorMesh;
	delete sphereMesh;
	delete cubeMesh;
	delete depthOfFieldMesh;
	
	//delete textures
	delete shadowMapTexture[0];
	delete shadowMapTexture[1];
	delete sceneTexture;
	delete depthOfFieldTexture;
	delete horizontalBlurTexture;
	delete verticalBlurTexture;
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
	//Modify the light colour based on a sin wave, creating a breathing light
	time += timer->getTime();
	//Divide by a value to increase the time between pulses
	float sinValue = -abs(sin(time/2));
	//Increase/decrease the larger values of rgb more than the smaller ones by adding the multiple of the sin value
	lights[plasmaLight]->setDiffuseColour(plasmaColours[0]+(sinValue*plasmaColours[0]), plasmaColours[1]+ (sinValue*plasmaColours[1]), plasmaColours[2] + (sinValue*plasmaColours[2]), plasmaColours[3]);

	camera->update();

	//Get the directional lights shadows
	FirstShadowPass();
	SecondShadowPass();

	//Render the actual scene to a render texture
	RenderScene();

	//Get a blur of the scene
	HorizontalBlurPass();
	VerticalBlurPass();
	
	//Modify the scene based on the camera depth
	DepthOfFieldShaderPass();

	//Render the final product to an orthomesh
	FinalPass();

	return true;
}

void App1::FirstShadowPass()
{
	//Change the back buffer to a render texture
	shadowMapTexture[directionalLight1]->setRenderTarget(renderer->getDeviceContext());
	shadowMapTexture[directionalLight1]->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//Get the scene from the light's POV
	lights[directionalLight1]->generateViewMatrix();
	XMMATRIX viewMatrix = lights[directionalLight1]->getViewMatrix();
	XMMATRIX projectionMatrix = lights[directionalLight1]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-50, -20, -50), XMMatrixScaling(0.1f, 0.1f, 0.1f));
	floorMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	depthShader->render(renderer->getDeviceContext(), floorMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	sphereMesh->sendData(renderer->getDeviceContext());
	depthShaderSphere->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("plasmaHM"), timer->getTime()*plasmaFlowRate, displacement);
	depthShaderSphere->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	if (renderBox)
	{
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(2, -3, 6), XMMatrixScaling(0.5f, 0.5f, 0.5f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, 150, 0));
		cubeMesh->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	}
	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::SecondShadowPass()
{
	//Change the back buffer to a render texture
	shadowMapTexture[directionalLight2]->setRenderTarget(renderer->getDeviceContext());
	shadowMapTexture[directionalLight2]->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//Get the scene from the light's POV
	lights[directionalLight2]->generateViewMatrix();
	XMMATRIX viewMatrix = lights[directionalLight2]->getViewMatrix();
	XMMATRIX projectionMatrix = lights[directionalLight2]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-50, -20, -50), XMMatrixScaling(0.1f, 0.1f, 0.1f));
	floorMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	depthShader->render(renderer->getDeviceContext(), floorMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	sphereMesh->sendData(renderer->getDeviceContext());
	depthShaderSphere->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("plasmaHM"), timer->getTime()*plasmaFlowRate, displacement);
	depthShaderSphere->render(renderer->getDeviceContext(), sphereMesh->getIndexCount());

	if (renderBox)
	{
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(2, -3, 6), XMMatrixScaling(0.5f, 0.5f, 0.5f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, 150, 0));
		cubeMesh->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	}
	// Set back buffer as render target and reset view port.
	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();

}


void App1::RenderScene()
{
	//Save the scene to a render texture for post-processing
	sceneTexture->setRenderTarget(renderer->getDeviceContext());
	sceneTexture->clearRenderTarget(renderer->getDeviceContext(), 0.02f, 0.02f, 0.02f, 1.0f);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//The floor is just being lit/shaded 
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-50, -20, -50), XMMatrixScaling(0.1f, 0.1f, 0.1f));
	floorMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("floor"), shadowMapTexture[0]->getShaderResourceView(), shadowMapTexture[1]->getShaderResourceView(), lights);
	shadowShader->render(renderer->getDeviceContext(), floorMesh->getIndexCount());

	//The sphere needs to be displaced and tessellated
	worldMatrix = renderer->getWorldMatrix();
	icosahedronMesh->sendData(renderer->getDeviceContext());
	displacementTessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("plasmaHM"), timer->getTime()*plasmaFlowRate, plasmaColours, plasmaInvert, displacement, tessFactor);
	displacementTessShader->render(renderer->getDeviceContext(), icosahedronMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	// Send geometry data, set shader parameters, render object with shader
	
	if (renderBox)
	{
		//Lit/shaded
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(2, -3, 6), XMMatrixScaling(0.5f, 0.5f, 0.5f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, 150, 0));
		cubeMesh->sendData(renderer->getDeviceContext());
		shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("box"), shadowMapTexture[0]->getShaderResourceView(), shadowMapTexture[1]->getShaderResourceView(), lights);
		shadowShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	}

	//Hide the black and high alpha values of our particle
	renderer->setAlphaBlending(true);

	//Loop through for the amount of particles set in ImGui
	for (int i = 0; i < amountOfParticles; i++)
	{
		XMFLOAT3 particlePositon = particleMesh[i]->updatePosition(timer->getTime(), particleDirectionalSpeed);
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(particlePositon.x, particlePositon.y, particlePositon.z), XMMatrixScaling(0.15f, 0.15f, 0.15f));
		particleMesh[i]->sendData(renderer->getDeviceContext());
		particleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("particle"), lights[0], camera->getPosition(), plasmaColours, plasmaInvert);
		particleShader->render(renderer->getDeviceContext(), particleMesh[i]->getIndexCount());
	}
	renderer->setAlphaBlending(false);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::HorizontalBlurPass()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeX = (float)horizontalBlurTexture->getTextureWidth();
	horizontalBlurTexture->setRenderTarget(renderer->getDeviceContext());
	horizontalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.02f, 0.02f, 0.02f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions
	orthoMatrix = horizontalBlurTexture->getOrthoMatrix();

	// Render for Vertical Blur
	renderer->setZBuffer(false);
	depthOfFieldMesh->sendData(renderer->getDeviceContext());
	horizontalBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, sceneTexture->getShaderResourceView(), screenSizeX);
	horizontalBlurShader->render(renderer->getDeviceContext(), depthOfFieldMesh->getIndexCount());
	renderer->setZBuffer(true);
}

void App1::VerticalBlurPass()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeY = (float)verticalBlurTexture->getTextureHeight();
	verticalBlurTexture->setRenderTarget(renderer->getDeviceContext());
	verticalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.02f, 0.02f, 0.02f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions
	orthoMatrix = verticalBlurTexture->getOrthoMatrix();

	// Render for Vertical Blur
	renderer->setZBuffer(false);
	depthOfFieldMesh->sendData(renderer->getDeviceContext());
	//Use the render texture from the previous horizontal blur pass
	verticalBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, horizontalBlurTexture->getShaderResourceView(), screenSizeY);
	verticalBlurShader->render(renderer->getDeviceContext(), depthOfFieldMesh->getIndexCount());
	renderer->setZBuffer(true);
}

void App1::DepthOfFieldShaderPass()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	
	depthOfFieldTexture->setRenderTarget(renderer->getDeviceContext());
	depthOfFieldTexture->clearRenderTarget(renderer->getDeviceContext(), 0.02f, 0.02f, 0.02f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions
	orthoMatrix = depthOfFieldTexture->getOrthoMatrix();

	// Render for Vertical Blur
	renderer->setZBuffer(false);
	depthOfFieldMesh->sendData(renderer->getDeviceContext());
	//Pass in all the relevant render textures so we can blend between them
	depthOfFieldShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, sceneTexture->getShaderResourceView(), verticalBlurTexture->getShaderResourceView());
	depthOfFieldShader->render(renderer->getDeviceContext(), depthOfFieldMesh->getIndexCount());
	renderer->setZBuffer(true);
}

void App1::FinalPass()
{
	renderer->setBackBufferRenderTarget();
	renderer->beginScene(0.02f, 0.02f, 0.02f, 1.0f);

	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	// ortho matrix for 2D rendering
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  

	// Default camera position for orthographic rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();

	//display the blurred and blended image
	depthOfFieldMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, depthOfFieldTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), depthOfFieldMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
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
	//Plasma settings
	ImGui::SliderFloat("plasma flow rate", &plasmaFlowRate, -1, 1);
	ImGui::Checkbox("Invert plasma colours: ", &plasmaInvert);
	ImGui::ColorPicker3("plasma colour", plasmaColours);
	ImGui::SliderFloat("Sphere displacement", &displacement, -1, 1);
	ImGui::SliderInt("Sphere tessellation", &tessFactor, 1, 64);

	//Model settings
	ImGui::Checkbox("Render box: ", &renderBox);
	//Particle settings
	ImGui::SliderFloat("particle directional speed", &particleDirectionalSpeed, -1, 1);
	ImGui::SliderInt("Amount of particles", &amountOfParticles, 0, MAX_PARTICLES);
	
	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

