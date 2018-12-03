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
	tessMesh = new TessellationSphere(renderer->getDevice(), renderer->getDeviceContext(), 20);
	floorMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	sphereMesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	cubeMesh = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	motionBlurMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);

	//Load texture
	textureMgr->loadTexture("brick", L"res/brick1.dds");
	textureMgr->loadTexture("plasmaHM", L"res/plasma_height_2.png");
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
	motionBlurShader = new MotionBlurShader(renderer->getDevice(), hwnd);
	depthShaderSphere = new DepthShaderSphere(renderer->getDevice(), hwnd);

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
	numberOfBlurSamples = 1;
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
	depthSceneTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	motionBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);


	// Setup directional light
	lights[directionalLight1]->setPosition(0, 10, -5.0f);
	lights[directionalLight1]->setDirection(0.7f, -1.0f, 0.7f);
	lights[directionalLight1]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[directionalLight1]->setDiffuseColour(0.2f, 0.2f, 0.2f, 1.0f);
	lights[directionalLight1]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	// Setup directional light
	lights[directionalLight2]->setPosition(0, 20, -10.0f);
	lights[directionalLight2]->setDirection(-0.7f, -1.0f, 1.5f);
	lights[directionalLight2]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[directionalLight2]->setDiffuseColour(0.2f, 0.2f, 0.2f, 1.0f);
	lights[directionalLight2]->generateOrthoMatrix(sceneWidth, sceneHeight, 0.1f, 100.f);

	// setup plasma light
	lights[plasmaLight]->setPosition(-2.0f, 2.0f, -2.0f);
	lights[plasmaLight]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	lights[plasmaLight]->setDiffuseColour(plasmaColours[0], plasmaColours[1], plasmaColours[2], plasmaColours[3]);


	lightPosition[0] = lights[plasmaLight]->getPosition().x;
	lightPosition[1] = lights[plasmaLight]->getPosition().y;
	lightPosition[2] = lights[plasmaLight]->getPosition().z;

	lightDirections[0] = lights[directionalLight2]->getDirection().x;
	lightDirections[1] = lights[directionalLight2]->getDirection().y;
	lightDirections[2] = lights[directionalLight2]->getDirection().z;

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
	delete tessShader;
	delete displacementShader;
	delete textureShader;
	delete shadowShader;
	delete depthShader;
	delete motionBlurShader;
	delete depthShaderSphere;
	//delete meshes
	for (int i = 0; i < MAX_PARTICLES; i ++)
		delete particleMesh[i];
	delete icosahedronMesh;
	delete tessMesh;
	delete floorMesh;
	delete sphereMesh;
	delete cubeMesh;
	delete motionBlurMesh;
	//delete textures
	delete shadowMapTexture[0];
	delete shadowMapTexture[1];
	delete sceneTexture;
	delete motionBlurTexture;
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
	time += timer->getTime();
	float sinValue = -abs(sin(time));
	lights[plasmaLight]->setPosition(lightPosition[0], lightPosition[1], lightPosition[2]);
	lights[plasmaLight]->setDiffuseColour(plasmaColours[0]+(sinValue*plasmaColours[0]), plasmaColours[1]+ (sinValue*plasmaColours[1]), plasmaColours[2] + (sinValue*plasmaColours[2]), plasmaColours[3]);
	//lights[directionalLight2]->setDirection(lightDirections[0], lightDirections[1], lightDirections[2]);

	camera->update();

	FirstShadowPass();

	SecondShadowPass();

	SceneDepthPass();

	RenderScene();

	MotionBlurShaderPass();

	FinalPass();

	return true;
}

void App1::FirstShadowPass()
{
	shadowMapTexture[directionalLight1]->setRenderTarget(renderer->getDeviceContext());
	shadowMapTexture[directionalLight1]->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

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
	shadowMapTexture[directionalLight2]->setRenderTarget(renderer->getDeviceContext());
	shadowMapTexture[directionalLight2]->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

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
void App1::SceneDepthPass()
{
	depthSceneTexture->setRenderTarget(renderer->getDeviceContext());
	depthSceneTexture->clearRenderTarget(renderer->getDeviceContext(), 0.02f, 0.02f, 0.02f, 1.0f);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	ID3D11ShaderResourceView* depthMaps[2] = { shadowMapTexture[0]->getShaderResourceView(), shadowMapTexture[1]->getShaderResourceView() };

	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-50, -20, -50), XMMatrixScaling(0.1f, 0.1f, 0.1f));
	floorMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	depthShader->render(renderer->getDeviceContext(), floorMesh->getIndexCount());

	/*worldMatrix = XMMatrixTranslation(-2.5, 0, 0);
	icosahedronMesh->sendData(renderer->getDeviceContext());
	tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tessFactor, camera->getPosition());
	tessShader->render(renderer->getDeviceContext(), icosahedronMesh->getIndexCount());*/

	worldMatrix = renderer->getWorldMatrix();
	icosahedronMesh->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	depthShader->render(renderer->getDeviceContext(), icosahedronMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	// Send geometry data, set shader parameters, render object with shader

	if (renderBox)
	{
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(2, -3, 6), XMMatrixScaling(0.5f, 0.5f, 0.5f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, 150, 0));
		cubeMesh->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	}
	renderer->setBackBufferRenderTarget();
}

void App1::RenderScene()
{


	sceneTexture->setRenderTarget(renderer->getDeviceContext());
	sceneTexture->clearRenderTarget(renderer->getDeviceContext(), 0.02f, 0.02f, 0.02f, 1.0f);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	ID3D11ShaderResourceView* depthMaps[2] = { shadowMapTexture[0]->getShaderResourceView(), shadowMapTexture[1]->getShaderResourceView() };

	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-50, -20, -50), XMMatrixScaling(0.1f, 0.1f, 0.1f));
	floorMesh->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("floor"), depthMaps, lights);
	shadowShader->render(renderer->getDeviceContext(), floorMesh->getIndexCount());

	/*worldMatrix = XMMatrixTranslation(-2.5, 0, 0);
	icosahedronMesh->sendData(renderer->getDeviceContext());
	tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tessFactor, camera->getPosition());
	tessShader->render(renderer->getDeviceContext(), icosahedronMesh->getIndexCount());*/

	worldMatrix = renderer->getWorldMatrix();
	icosahedronMesh->sendData(renderer->getDeviceContext());
	displacementShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("plasmaHM"), timer->getTime()*plasmaFlowRate, plasmaColours, plasmaInvert, displacement, tessFactor);
	displacementShader->render(renderer->getDeviceContext(), icosahedronMesh->getIndexCount());

	worldMatrix = renderer->getWorldMatrix();
	// Send geometry data, set shader parameters, render object with shader
	
	if (renderBox)
	{
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(2, -3, 6), XMMatrixScaling(0.5f, 0.5f, 0.5f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationRollPitchYaw(0, 150, 0));
		cubeMesh->sendData(renderer->getDeviceContext());
		shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("box"), depthMaps, lights);
		shadowShader->render(renderer->getDeviceContext(), cubeMesh->getIndexCount());
	}

	renderer->setAlphaBlending(true);

	for (int i = 0; i < amountOfParticles; i++)
	{
		XMFLOAT3 particlePositon = particleMesh[i]->updatePosition(timer->getTime(), particleDirectionalSpeed);
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(particlePositon.x, particlePositon.y, particlePositon.z), XMMatrixScaling(0.15f, 0.15f, 0.15f));
		particleMesh[i]->sendData(renderer->getDeviceContext());
		particleShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("particle"), lights[0], camera->getPosition(), plasmaColours, plasmaInvert);
		particleShader->render(renderer->getDeviceContext(), particleMesh[i]->getIndexCount());
	}
	renderer->setAlphaBlending(false);


	/*TessellationSphere->sendData(renderer->getDeviceContext());
	tessShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tessFactor, camera->getPosition());
	tessShader->render(renderer->getDeviceContext(), TessellationSphere->getIndexCount());*/

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::MotionBlurShaderPass()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeY = (float)motionBlurTexture->getTextureHeight();
	motionBlurTexture->setRenderTarget(renderer->getDeviceContext());
	motionBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.02f, 0.02f, 0.02f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = motionBlurTexture->getOrthoMatrix();

	// Render for Vertical Blur
	renderer->setZBuffer(false);
	motionBlurMesh->sendData(renderer->getDeviceContext());
	motionBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, sceneTexture->getShaderResourceView(), depthSceneTexture->getShaderResourceView(),
		numberOfBlurSamples, camera->getRotation());
	motionBlurShader->render(renderer->getDeviceContext(), motionBlurMesh->getIndexCount());
	renderer->setZBuffer(true);
}

void App1::FinalPass()
{
	renderer->setBackBufferRenderTarget();
	renderer->beginScene(0.02f, 0.02f, 0.02f, 1.0f);

	// RENDER THE RENDER TEXTURE SCENE
	// Requires 2D rendering and an ortho mesh.
	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	motionBlurMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, motionBlurTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), motionBlurMesh->getIndexCount());
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
	ImGui::SliderFloat("particle directional speed", &particleDirectionalSpeed, -1, 1);
	ImGui::SliderFloat("plasma flow rate", &plasmaFlowRate, -1, 1);
	ImGui::Checkbox("Invert plasma colours: ", &plasmaInvert);
	ImGui::Checkbox("Render box: ", &renderBox);
	ImGui::SliderInt("Amount of particles", &amountOfParticles, 0, MAX_PARTICLES);
	ImGui::SliderInt("Blur samples", &numberOfBlurSamples, 1, 3);
	ImGui::ColorPicker3("plasma colour", plasmaColours);
	ImGui::SliderFloat3("Plasma light position", lightPosition, -20, 20);
	ImGui::SliderFloat3("Directional light direction", lightDirections, -2, 2);
	ImGui::SliderFloat("Sphere displacement", &displacement, -1, 1);
	ImGui::SliderInt("Sphere tessellation", &tessFactor, 0, 64);


	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

