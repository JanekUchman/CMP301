// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TessellationShader.h"
#include "TessellationQuad.h"

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
	TessellationQuad* tessQuad;
	TessellationShader* shader;
	int minTess = 1;
	int maxTess = 64;
	int tessEdge = 2;
	int tessInside = 3;

};

#endif