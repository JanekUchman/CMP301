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
	//Variables
	int tessFactor;
	int amountOfParticles;
	int numberOfBlurSamples;
	//lava variables
	float lavaFlowRate;
	float lavaColours[3];
	bool lavaInvert;
	bool renderBox;
	//Shaders
	ParticleShader* particleShader;
	TessellationShader* tessShader;
	DisplacementShader* displacementShader;
	TextureShader* textureShader;
	ShadowShader* shadowShader;
	DepthShader* depthShader;

	//Meshs
	SinglePointMesh* particleMesh[MAX_PARTICLES];
	IcosahedronMesh* icosahedronMesh;
	TessellationSphere* tessMesh;

	PlaneMesh* floorMesh;
	SphereMesh* sphereMesh;
	CubeMesh* cubeMesh;

	//Render textures
	RenderTexture* shadowMap;
	RenderTexture* motionBlur;

	//Lights
	Light* lights[2];

	//functions
	void DepthPass();
	void ShadowPass();
	void FinalPass();

};
