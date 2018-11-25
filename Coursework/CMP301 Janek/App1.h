// Application.h
#pragma once

// Includes
#include "DXF.h"	// include dxframework
#include "GeometryShader.h"
#include "MultiPointMesh.h"
#include "TessellationQuad.h"
#include "TessellationShader.h"
#include "IcosahedronMesh.h"
#include "DisplacementShader.h"

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
	//lava variables
	float lavaFlowRate;
	float lavaColours[3];
	bool lavaInvert;
	//Shaders
	GeometryShader* geometryShader;
	TessellationShader* tessShader;
	DisplacementShader* displacementShader;

	//Meshs
	MultiPointMesh* billboardMesh;
	IcosahedronMesh* icosahedronMesh;
	PlaneMesh* floorMesh;

	//Lights
	Light* lights[2];

};
