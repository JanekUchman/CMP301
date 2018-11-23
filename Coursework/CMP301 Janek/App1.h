// Application.h
#pragma once

// Includes
#include "DXF.h"	// include dxframework
#include "GeometryShader.h"
#include "MultiPointMesh.h"
#include "TessellationQuad.h"
#include "TessellationShader.h"
#include "Icosahedron.h"

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
	int tessFactor;
	GeometryShader* geometryShader;
	TessellationShader* tessShader;
	MultiPointMesh* billboardMesh;
	Icosahedron* icosahedron;
	QuadMesh* floor;

	Light* lights[2];

};
