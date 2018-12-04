// Application.h
#pragma once

// Includes
#include "DXF.h"	// include dxframework
#include "ParticleShader.h"
#include "SinglePointMesh.h"
#include "TessellationQuad.h"
#include "TessellationShader.h"
#include "IcosahedronMesh.h"
#include "DisplacementTessellationShader.h"
#include "TextureShader.h"
#include "ShadowShader.h"
#include "CubeMesh.h"
#include "DepthShader.h"
#include "OrthoMesh.h"
#include "DepthOfFieldShader.h"
#include "DepthShaderSphere.h"
#include "VerticalBlurShader.h"
#include "HorizontalBlurShader.h"

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
	//Light consts to help reading of light arrays
	const int directionalLight1 = 0;
	const int directionalLight2 = 1;
	const int plasmaLight = 2;
	//Variables
	int amountOfParticles;
	int tessFactor;
	float displacement;
	float time;

	//plasma variables
	float plasmaFlowRate;
	float plasmaColours[3];
	float particleDirectionalSpeed;
	bool plasmaInvert;
	bool renderBox;
	//Shaders
	ParticleShader* particleShader;
	//Plasma sphere shader, displaces and tessellates
	DisplacementTessellationShader* displacementTessShader;
	//For rendering the final scene
	TextureShader* textureShader;
	//For rendering anything affected by lighting/shadows
	ShadowShader* shadowShader;
	//For getting light/scene depths
	DepthShader* depthShader;
	DepthOfFieldShader* depthOfFieldShader;
	//Get a different depth shader for the sphere's shadow
	DepthShaderSphere* depthShaderSphere;
	HorizontalBlurShader* horizontalBlurShader;
	VerticalBlurShader* verticalBlurShader;

	//Meshs
	SinglePointMesh* particleMesh[MAX_PARTICLES];
	IcosahedronMesh* icosahedronMesh;
	PlaneMesh* floorMesh;
	SphereMesh* sphereMesh;
	CubeMesh* cubeMesh;
	OrthoMesh* depthOfFieldMesh;

	//Render textures
	RenderTexture* shadowMapTexture[2];
	RenderTexture* sceneTexture;
	RenderTexture* horizontalBlurTexture;
	RenderTexture* verticalBlurTexture;
	RenderTexture* depthOfFieldTexture;

	//Lights
	Light* lights[3];

	//functions
	void FirstShadowPass();
	void SecondShadowPass();
	void RenderScene();
	void DepthOfFieldShaderPass();
	void FinalPass();
	void HorizontalBlurPass();
	void VerticalBlurPass();

};
