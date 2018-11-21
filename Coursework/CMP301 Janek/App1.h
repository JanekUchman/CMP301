// Application.h
#pragma once

#define TESS_QUAD_ROWS 10
#define TESS_QUAD_COLS 10

// Includes
#include "DXF.h"	// include dxframework
#include "GeometryShader.h"
#include "MultiPointMesh.h"
#include "TessellationQuad.h"
#include "TessellationShader.h"

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
	MultiPointMesh* mesh;
	SphereMesh
	Light* light;
};
