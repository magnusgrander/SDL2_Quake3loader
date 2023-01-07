#include "Main.h"								// This includes our main header file
#include "Camera.h"								// Include our camera header file
#include "Quake3Bsp.h"							// Include our quake3 header file
#include "Frustum.h"							// Include our frustum culling class

// This is our velocity vector that is added to our camera position for gravity/jumping
glm::vec3 g_vVelocity = glm::vec3(0, 0, 0);

CCamera g_Camera;								// This is our global camera object

bool  g_bFullScreen = true;						// Set full screen as default

GLuint g_Texture[MAX_TEXTURES] = {0};				// This holds the texture info, referenced by an ID

// This is our object that loads and handles the .bsp file loading and rendering
CQuake3BSP g_Level;

// These tell us if we are rendering triangles and the textures.
bool g_RenderMode = true;
bool g_bTextures  = true;

// We want to be able to turn lightmaps on and off with the left mouse button
bool g_bLightmaps = true;

// This is our gamma level.  This is read from the config file.
float g_Gamma = 10;

// This is our global frustum class, which is used to cull BSP leafs
CFrustum g_Frustum;

// This will store how many faces are drawn and are seen by the camera
int g_VisibleFaces = 0;

bool mforward = false, mbackward = false, mleft = false, mright = false,  mspace=false, mshift = false;

///////////////////////////////// INIT WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function initializes the application
/////
///////////////////////////////// INIT WINDOW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Init()
{
	 InitializeOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);  // Init OpenGL with the global rect

    // To load our .bsp file, we created a config.ini file that stores
    // the name of the level that we are loading.  This way we don't need
    // to change code to test different levels.  We will also add more options
    // to our config file in later tutorials.

    // Here we open the config file and init some variables
    ifstream fin("Config.ini");
    string strLevel = "";
    string strTemp = "";

    // Check if the file was not found or could not be opened
    if(fin.fail())
    {
        // Display an error message and quit the program if file wasn't found
        cerr << "Could not find the Config.ini file!" << endl;
        Quit(0);
        return;
    }

    // Read in the name of the level that will be loaded and close the file
    fin >> strLevel >> strLevel;

    // Now we need to read in the gamma level for our lightmaps
    fin >> strTemp  >> g_Gamma;

    fin.close();

    // Here we load the level and get the result (true == success)
    bool bResult = g_Level.LoadBSP(strLevel.c_str());

    // Make sure the file was found and we received a success
    if(bResult == false)
    {
        // Quit the application
        Quit(0);
        return;
    }

    // Position the camera to the starting point since we have
    // not read in the entities yet, which gives the starting points.
    g_Camera.PositionCamera( 80, 88, 16,   80, 288, 17,    0, 1, 0);

    // Turn on depth testing and texture mapping
    glEnable(GL_DEPTH_TEST);    
    glEnable(GL_TEXTURE_2D);

    // Enable front face culling, since that's what Quake3 does
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
}


/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function clamps a specified frame rate for consistency
/////
/////////////////////////////// ANIMATE NEXT FRAME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

bool AnimateNextFrame(int desiredFrameRate)
{
    if( SDL_GetTicks() < 1000 / desiredFrameRate )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / desiredFrameRate ) - SDL_GetTicks() );
        }

	return false;
}

///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
	int i = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix

	// Tell OpenGL where to look from our camera info
	g_Camera.Look();

	// Calculate our frustum to check the world data again for PVS and Portal Rendering
	g_Frustum.CalculateFrustum();

	// Render the level to the screen
	g_Level.RenderLevel(g_Camera.Position());
    AnimateNextFrame(60);    
	// Swap the backbuffers to the foreground
	SDL_GL_SwapWindow(MainWindow);             //change !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}


///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the window messages.
/////
///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void MainLoop(void)
{
    bool done = false;                                     // is our job done ? not yet !
    SDL_Event event;

    while(! done)                                          // as long as our job's not done
    {
        while( SDL_PollEvent(& event) )                    // look for events (like keystrokes, resizing etc.)
        {
            switch ( event.type )                          // what kind of event have we got ?
            {
                case SDL_QUIT :                                         // if user wishes to quit
                    done = true;                                        // this implies our job is done
                    break;

                case SDL_KEYDOWN :                      // if the user has pressed a key

                    /////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// * 
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_F1 :                      // If the key is F1 
                            ToggleFullScreen();             // Toggle between FullScreen and windowed mode
                            break;

                        case SDLK_ESCAPE :                  // if the key is ESCAPE
                            Quit(0);
            
                        // If we press the W or the UP arrow key we want to move the camera forward
                        case SDLK_UP :
                        case SDLK_w  :
                            mforward = true;
                            break;

                        // If we press the s or the DOWN arrow key we want to zoom the scope out
                        case SDLK_DOWN :
                        case SDLK_s    :
                            mbackward = true;
                            break;

                        // If we press the A or LEFT arrow key we want to strafe the camera left
                        case SDLK_LEFT :
                        case SDLK_a    :
                            mleft = true;
                            break;

                        // If we press the D or RIGHT arrow key we want to strafe the camera right
                        case SDLK_RIGHT :
                        case SDLK_d     :
                            mright = true;
                            break;
                        
                        case SDLK_SPACE :
                            mspace = true;
                            break;

                        case SDLK_LSHIFT :
                            mshift = true;
                            break;

                         case SDLK_RSHIFT :
                            mshift = true;
                            break;


                        case SDLK_F2 :                   // if we hit the SPACE Key
                            g_RenderMode = !g_RenderMode;           // Change the rendering mode
                            // Change the rendering mode to and from lines or triangles
                            if(g_RenderMode)                
                            {
                                // Render the triangles in fill mode        
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  
                            }
                            else 
                            {
                                // Render the triangles in wire frame mode
                                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
                            }
                            break;

                        default:
                            break;
                    }
                    break;               

                case SDL_KEYUP :
                    switch(event.key.keysym.sym)
                    {
                        // If we release the W or the UP arrow key we want to stop moving forward
                        case SDLK_UP :
                        case SDLK_w  :
                            mforward = false;
                            break;

                        // If we release the s or the DOWN arrow key we want to stop moving backward
                        case SDLK_DOWN :
                        case SDLK_s    :
                            mbackward = false;
                            break;

                        // If we release the A or LEFT arrow key we want to stop strafing left
                        case SDLK_LEFT :
                        case SDLK_a    :
                            mleft = false;
                            break;

                        // If we release the D or RIGHT arrow key we want to stop strafing right
                        case SDLK_RIGHT :
                        case SDLK_d     :
                            mright = false;
                            break;
                        
                        case SDLK_LSHIFT:
                            mshift = false;
                            break;

                        case SDLK_RSHIFT :
                            mshift = false;
                            break;


                        case SDLK_SPACE:
                            mspace = false;
                            break;
                    }
                    break;
                    
                case SDL_MOUSEBUTTONDOWN :                  // if a mouse button was pressed
                    if( event.button.button == SDL_BUTTON_RIGHT )       // and it's the Right mouse button
                    {
                        g_bTextures = !g_bTextures;                     // Turn texture mapping on/off

                        if(!g_bTextures)                                // If we don't want texture mapping
                        {
                            glActiveTextureARB(GL_TEXTURE0_ARB);
                            glDisable(GL_TEXTURE_2D);                   // Disable texture mapping
                        }
                    }
                    if( event.button.button == SDL_BUTTON_LEFT )
                    {
                        g_bLightmaps = !g_bLightmaps;

                        if( !g_bLightmaps)
                        {
                            glActiveTextureARB(GL_TEXTURE1_ARB);
                            glDisable(GL_TEXTURE_2D);
                        }
                    }
                    break;

                case SDL_MOUSEMOTION :                      // If the mouse is moved 
                    //g_Camera.Update();                      // Check the mouse camera movement
                    g_Camera.SetViewByMouse();                   
                    break;


                default:                                    // any other event
                    break;                                  // nothing to do
            } // switch
        } // while( SDL_ ...
        g_Camera.Update();                                  // update the position of the camera before drawing the frame
        RenderScene();                                      // draw our OpenGL scene
    } // while( ! done)
}






