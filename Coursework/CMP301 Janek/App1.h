// Application.h
#pragma once

// Includes
#include "DXF.h"	// include dxframework
#include "ParticleShader.h"
#include "SinglePointMesh.h"
#include "TessellationQuad.h"
#include "TessellationShader.h"
#include "IcosahedronMesh.h"
#include "DisplacementShader.h"
#include "TextureShader.h"
#include "ShadowShader.h"
#include "CubeMesh.h"
#include "DepthShader.h"
#include "TessellationSphere.h"
#include "OrthoMesh.h"
#include "MotionBlurShader.h"

#define MAX_PARTICLES 40

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();
	

protected:
	bool render();
	void gui();

private:
	const int directionalLight1 = 0;
	const int directionalLight2 = 1;
	const int plasmaLight = 2;
	//Variables
	int tessFactor;
	int amountOfParticles;
	int numberOfBlurSamples;
	float time;

	//plasma variables
	float plasmaFlowRate;
	float plasmaColours[3];
	float particleDirectionalSpeed;
	bool plasmaInvert;
	bool renderBox;
	//Shaders
	ParticleShader* particleShader;
	TessellationShader* tessShader;
	DisplacementShader* displacementShader;
	TextureShader* textureShader;
	ShadowShader* shadowShader;
	DepthShader* depthShader;
	MotionBlurShader* motionBlurShader;

	//Meshs
	SinglePointMesh* particleMesh[MAX_PARTICLES];
	IcosahedronMesh* icosahedronMesh;
	TessellationSphere* tessMesh;

	PlaneMesh* floorMesh;
	SphereMesh* sphereMesh;
	CubeMesh* cubeMesh;
	OrthoMesh* motionBlurMesh;
	//Render textures
	RenderTexture* shadowMapTexture[2];
	RenderTexture* sceneTexture;
	RenderTexture* motionBlurTexture;

	//Lights
	Light* lights[3];
	float lightPosition[3];
	float lightDirections[3];

	//functions
	void FirstShadowPass();
	void SecondShadowPass();
	void RenderScene();
	void MotionBlurShaderPass();
	void FinalPass();

};
