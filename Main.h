#ifndef _MAIN_H
#define _MAIN_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <windows.h>					// Include the much need windows.h
//#include <gl\gl.h>						// Header File For The OpenGL32 Library
//#include <gl\glu.h>						// Header File For The GLu32 Library

#include <GL/glew.h>					// Header File For The GLaux Library
#include <iostream>						// Include our c++ standard header
#include <fstream>						// Include this to use ifstream
#include <string>						// Include this to use the string class
#include <vector>						// Include this to use STL vectors
#include <math.h>                       /* fabs */
#include <stdio.h>
#include <string.h>
//#include <mmsystem.h>					// Include this to use timeGetTime()
#include <SDL2/SDL.h>
#include "TextureLoader.h"

using namespace std;					// Start off with the standard namespace

#define SCREEN_WIDTH  1024				// We want our screen width 640 pixels
#define SCREEN_HEIGHT 768				// We want our screen height 480 pixels
#define SCREEN_DEPTH 24					// We want 16 bits per pixel

#define MAX_TEXTURES 1000				// The maximum amount of textures to load

extern GLuint g_Texture[MAX_TEXTURES];	// Our global texture array

// This is our basic 2D point class.  This will be used to store the UV coordinates.
class CVector2 
{
public:

		// A default constructor
	CVector2() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector2(float X, float Y) 
	{ 
		x = X; y = Y;
	}

	// Here we overload the + operator so we can add vectors together 
	CVector2 operator+(CVector2 vVector)
	{
		// Return the added vectors result.
		return CVector2(vVector.x + x, vVector.y + y);
	}

	// Here we overload the - operator so we can subtract vectors 
	CVector2 operator-(CVector2 vVector)
	{
		// Return the subtracted vectors result
		return CVector2(x - vVector.x, y - vVector.y);
	}
	
	// Here we overload the * operator so we can multiply by scalars
	CVector2 operator*(float num)
	{
		// Return the scaled vector
		return CVector2(x * num, y * num);
	}

	// Here we overload the / operator so we can divide by a scalar
	CVector2 operator/(float num)
	{
		// Return the scale vector
		return CVector2(x / num, y / num);
	}

	float x, y;
};


// Extern our global variables so other source files can use them
extern float g_FrameInterval;
extern int VideoFlags;                                     // defines how the drawing surface should be
extern SDL_Window *MainWindow;                            // our main window for drawing
extern SDL_GLContext mainContext;

// This controls our main program loop
void MainLoop();

// This creates a texture and stores it in the texture array with it's ID.
bool CreateTexture(GLuint &texture, char *strFileName);

// This changes the screen to full screen mode
void ToggleFullScreen();

// This is our own function that makes creating a window modular and easy
void CreateMyWindow(char *strWindowName, int width, int height, int Videoflags);

// This allows us to configure our window for OpenGL and backbuffered
void SetupPixelFormat(void);

// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
void InitializeOpenGL(int width, int height);

// This initializes the whole program
void Init();

// This draws everything to the screen
void RenderScene();

// This Shuts down SDL and quits the program
void Quit(int ret_val);

#endif 

