#include "Init.h"

///////////////////////////////// CREATE TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This creates a texture in OpenGL that we can texture map
/////
///////////////////////////////// CREATE TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool CreateTexture(GLuint &texture, char* strFileName)
{   
    GLuint tex;
    TextureLoader tl;
    std::string FileN(strFileName);

    tex = tl.loadTexture(FileN + ".jpg");

    if(tl.m_Error==-1) {
        tex = tl.loadTexture(FileN + ".tga");
    }

    if(tl.m_Error==-1) {
        tex = tl.loadTexture(FileN + ".bmp");
    }

    if(tl.m_Error==-1) {
        tex = tl.loadTexture(FileN + ".png");
    }

    if(tl.m_Error==-1) {
        printf("Error Loading: %s \n", FileN.c_str());
    }
    
    texture = tex;    
        
    return true;
}

/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function TOGGLES between FULLSCREEN and WINDOWED mode
/////
/////////////////////////////////// TOGGLE FULL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void ToggleFullScreen(void)
{
  // remove
}

///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  This function CREATES our WINDOW for drawing the GL stuff
/////
///////////////////////////////   CREATE MY WINDOW   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateMyWindow(const char * strWindowName, int width, int height, int VideoFlags)
{
    MainWindow = SDL_CreateWindow(strWindowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);

	// Check that everything worked out okay
	if (!MainWindow)
	{
		std::cout << "Unable to create window\n";		
		return;
	}

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(MainWindow);  
}

/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////  Sets the pixel format for openGL and video flags for SDL
/////
/////////////////////////////   SETUP PIXEL FORMAT   \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void SetupPixelFormat(void)
{    
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );      // tell SDL that the GL drawing is going to be double buffered
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   SCREEN_DEPTH);         // size of depth buffer 
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);       // we aren't going to use the stencil buffer
    SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);     // this and the next three lines set the bits allocated per pixel -
    SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);   // - for the accumulation buffer to 0
    SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);
}

//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This function resizes the viewport for OpenGL.
/////
//////////////////////////// RESIZE OPENGL SCREEN \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
void SizeOpenGLScreen(int width, int height)            // Initialize The GL Window
{
    if (height==0)                                      // Prevent A Divide By Zero error
    {
        height=1;                                       // Make the Height Equal One
    }

    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                   // Reset The Projection Matrix
    gluPerspective(70.0f,(GLfloat)width/(GLfloat)height, 10.0f , 4000.0f);
    glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix
    glLoadIdentity();                                   // Reset The Modelview Matrix
}

/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////         This function handles all the initialization for openGL
/////
/////////////////////////////////   INITIALIZE OPENGL  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void InitializeOpenGL(int width, int height)
{
    glEnable( GL_DEPTH_TEST );                             // Enables Depth Test i.e enables use of depth buffer for drawing
    glEnable( GL_TEXTURE_2D );                             // Enable Texture Mapping 
    SizeOpenGLScreen(width, height);                       // resize the OpenGL Viewport to the given height and width
}

////////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////     create the window and calling the initialization functions
//////
////////////////////////////////      MAIN      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

int main(void)
{
    // NEEDED TO CHANGE....................................

    // print user instructions
    cout << " Hit the F1 key to Toggle between Fullscreen and windowed mode" << endl;
    cout << " Hit ESC to quit" << endl;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )                      // try to initialize SDL video module
    {
        cerr << "Failed initializing SDL Video : " << SDL_GetError() << endl;      // report error if it fails
        return 1;                                             // we use this instead of Quit because SDL isn't yet initialized
    }

    // Set up the format for the pixels of the OpenGL drawing surface
    SetupPixelFormat();
    // Create our window, we pass caption for the window, the width, height and video flags required
    CreateMyWindow("www.GameTutorials.com - Quake 3 BSP Loader3", SCREEN_WIDTH, SCREEN_HEIGHT, VideoFlags);

    // This makes our buffer swap syncronized with the monitor's vertical refresh
	GLenum rev;
    
    rev = glewInit();
    glewExperimental = GL_TRUE;  

      if (GLEW_OK != rev){
          printf("Error: %s", glewGetErrorString(rev));
          exit(1);
      } else {
          printf("GLEW Init: Success!");
      }      


    // Initializes our OpenGL drawing surface
    Init();
    SDL_GL_SetSwapInterval(1);  
        
    // Run our message loop
    MainLoop();

    // quit main returning success
    return 0;
}

////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//////
//////      This will shutdown SDL and quit the program
//////
////////////////////////////////    QUIT    \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Quit(int ret_val)
{
    SDL_Quit();                                            // shuts down SDL stuff
    exit(ret_val);                                         // quit the program
}


