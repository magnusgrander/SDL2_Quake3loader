#include "Main.h"
#include "Camera.h"
#include "Quake3Bsp.h"


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

// In our CheckForMovement() function we add some code to add gravity and velocity
// to our camera.  We also get rid of the ability to move along the Y-Axis by the
// view vector.  That way we just go up only if we jump or go up some stairs/slopes.
// 
//

// Since we need the velocity vector for our camera, let's extern this here
extern glm::vec3 g_vVelocity;

// Changed the speed from 300 because of the frame interval change in CalculateFrameRate()
#define kSpeed	200.0f	

extern bool mforward, mbackward, mleft, mright, mshift, mspace;	

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


// Our global float that stores the elapsed time between the current and last frame
float g_FrameInterval = 0.0f;

// Extern the level object so we can call collision detection calls
extern CQuake3BSP g_Level;


///////////////////////////////// CALCULATE FRAME RATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function calculates the frame rate and time intervals between frames
/////
///////////////////////////////// CALCULATE FRAME RATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CalculateFrameRate()
{
  static float framesPerSecond    = 0.0f;     // This will store our fps
    static float lastTime           = 0.0f;     // This will hold the time from the last frame
    static char strFrameRate[50] = {0};         // We will store the string here for the window title

    static float frameTime = 0.0f;              // This stores the last frame's time

    // Get the current time in seconds
    float currentTime = SDL_GetTicks() * 0.001f;                

    // Here we store the elapsed time between the current and last frame,
    // then keep the current frame in our static variable for the next frame.
    g_FrameInterval = currentTime - frameTime;
    frameTime = currentTime;

    // Increase the frame counter
    ++framesPerSecond;

    // Now we want to subtract the current time by the last time that was stored
    // to see if the time elapsed has been over a second, which means we found our FPS.
    if( currentTime - lastTime > 1.0f )
    {
        // Here we set the lastTime to the currentTime
        lastTime = currentTime;
        
        // Copy the frames per second into a string to display in the window title bar
        sprintf(strFrameRate, "Current Frames Per Second: %d", int(framesPerSecond));

        // Set the window title bar to our string
        //SDL_WM_SetCaption(strFrameRate,"GameTutorials");
        SDL_SetWindowTitle(MainWindow, strFrameRate);

        // Reset the frames per second
        framesPerSecond = 0;
    }
}


/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a perpendicular vector from 2 given vectors by taking the cross product.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
												
glm::vec3 Cross(glm::vec3 vVector1, glm::vec3 vVector2)
{
	glm::vec3 vNormal;	

	// Calculate the cross product with the non communitive equation
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product
	return vNormal;										 
}


/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a vector
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(glm::vec3 vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}


/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

glm::vec3 Normalize(glm::vec3 vVector)
{
	// Get the magnitude of our normal
	float magnitude = Magnitude(vVector);				

	// Now that we have the magnitude, we can divide our vector by that magnitude.
	// That will make our vector a total length of 1.  
	vVector = vVector / magnitude;		
	
	// Finally, return our normalized vector
	return vVector;										
}


///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This is the class constructor
/////
///////////////////////////////// CCAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CCamera::CCamera()
{
	glm::vec3 vZero = glm::vec3(0.0, 0.0, 0.0);		// Init a vVector to 0 0 0 for our position
	glm::vec3 vView = glm::vec3(0.0, 1.0, 0.5);		// Init a starting view vVector (looking up and out the screen) 
	glm::vec3 vUp   = glm::vec3(0.0, 0.0, 1.0);		// Init a standard up vVector (Rarely ever changes)

	m_vPosition	= vZero;					// Init the position to zero
	m_vView		= vView;					// Init the view to a std starting view
	m_vUpVector	= vUp;						// Init the UpVector
}


///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function sets the camera's position and view and up vVector.
/////
///////////////////////////////// POSITION CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::PositionCamera(float positionX, float positionY, float positionZ,
				  		     float viewX,     float viewY,     float viewZ,
							 float upVectorX, float upVectorY, float upVectorZ)
{
	glm::vec3 vPosition	= glm::vec3(positionX, positionY, positionZ);
	glm::vec3 vView		= glm::vec3(viewX, viewY, viewZ);
	glm::vec3 vUpVector	= glm::vec3(upVectorX, upVectorY, upVectorZ);

	// The code above just makes it cleaner to set the variables.
	// Otherwise we would have to set each variable x y and z.

	m_vPosition = vPosition;					// Assign the position
	m_vView     = vView;						// Assign the view
	m_vUpVector = vUpVector;					// Assign the up vector
}


///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This allows us to look around using the mouse, like in most first person games.
/////
///////////////////////////////// SET VIEW BY MOUSE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::SetViewByMouse()
{
	int mousePos_x,mousePos_y;
    int middleX = 512; //SCREEN_WIDTH  >> 1;               // This is a binary shift to get half the width
    int middleY = 384; //SCREEN_HEIGHT >> 1;               // This is a binary shift to get half the height
    float angleY = 0.0f;                            // This is the direction for looking up or down
    float angleZ = 0.0f;                            // This will be the value we need to rotate around the Y axis (Left and Right)
    static float currentRotX = 0.0f;
        
    // Get the mouse's current X,Y position
    SDL_GetMouseState(&mousePos_x,&mousePos_y);
    
    // If our cursor is still in the middle, we never moved... so don't update the screen
    if( (mousePos_x == middleX) && (mousePos_y == middleY) ) return;

    // Set the mouse position to the middle of our window
    //SDL_WarpMouse(middleX, middleY);
    SDL_WarpMouseInWindow(MainWindow, middleX, middleY);

    // Get the direction the mouse moved in, but bring the number down to a reasonable amount
    angleY = (float)( (middleX - mousePos_x) ) / 500.0f;        
    angleZ = (float)( (middleY - mousePos_y) ) / 500.0f;        

    // Here we keep track of the current rotation (for up and down) so that
    // we can restrict the camera from doing a full 360 loop.
    currentRotX -= angleZ;  

    // If the current rotation (in radians) is greater than 1.0, we want to cap it.
    if(currentRotX > 1.0f)
        currentRotX = 1.0f;
    // Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
    else if(currentRotX < -1.8f)
        currentRotX = -1.8f;
    // Otherwise, we can rotate the view around our position
    else
    {
        // To find the axis we need to rotate around for up and down
        // movements, we need to get a perpendicular vector from the
        // camera's view vector and up vector.  This will be the axis.
        glm::vec3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
        vAxis = Normalize(vAxis);

        // Rotate around our perpendicular axis and along the y-axis
        RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
        RotateView(angleY, 0, 1, 0);
    }    
}


///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This rotates the view around the position using an axis-angle rotation
/////
///////////////////////////////// ROTATE VIEW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::RotateView(float angle, float x, float y, float z)
{
	glm::vec3 vNewView;

	// Get the view vector (The direction we are facing)
	glm::vec3 vView = m_vView - m_vPosition;		

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_vView = m_vPosition + vNewView;
}


///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This strafes the camera left and right depending on the speed (-/+)
/////
///////////////////////////////// STRAFE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::StrafeCamera(float speed)
{	
	// Add the strafe vector to our position
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// Add the strafe vector to our view
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}


///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This will move the camera forward or backward depending on the speed
/////
///////////////////////////////// MOVE CAMERA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::MoveCamera(float speed)
{
	// Get the current view vector (the direction we are looking)
	glm::vec3 vVector = m_vView - m_vPosition;
	vVector = Normalize(vVector);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// We don't add anything to this section, but we delete the code that
	// has the m_vPosition.y and m_vView.y effected.  We deleted those 2 lines.
	m_vPosition.x += vVector.x * speed;		// Add our acceleration to our position's X
	m_vPosition.z += vVector.z * speed;		// Add our acceleration to our position's Z
	m_vView.x += vVector.x * speed;			// Add our acceleration to our view's X
	m_vView.z += vVector.z * speed;			// Add our acceleration to our view's Z

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
}


//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the input faster than in the WinProc()
/////
//////////////////////////// CHECK FOR MOVEMENT \\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::CheckForMovement()
{	
    float speed;
	// Once we have the frame interval, we find the current speed
    if( mshift ) {   
	    speed = (float)( (kSpeed*2) * g_FrameInterval);
    }
    else {
        speed = (float)(kSpeed * g_FrameInterval);
    }
	// Store the last position and view of the camera
	glm::vec3 vOldPosition = Position();
	glm::vec3 vOldView = View();

	// Use a flag to see if we movement backwards or not
	bool bMovedBack = false;


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// Here is where we subtract the gravity acceleration from our velocity vector.
	// We then add that velocity vector to our camera to effect our camera (or player)
	// This is also how we handle the jump velocity when we hit space bar.
	// Notice that we multiply the gravity by the frame interval (dt).  This makes
	// it so faster video cards don't do a 2 frame jump, while TNT2 cards do 20 frames :)
	// This is necessary to make every computer use the same movement and jump speed.
	g_vVelocity.y -= (float)(kGravity * g_FrameInterval);
	m_vPosition = m_vPosition + g_vVelocity;

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	 // Check if we hit the Up arrow or the 'w' key
    if( mforward )                                          // if we are supposed to move the camera forward
    {
        // Move our camera forward by a positive SPEED
        MoveCamera(speed);
    }
    // Check if we hit the Down arrow or the 's' key
    if( mbackward )                                         // if we are supposed to move the camera backward
    {
        // Move our camera backward by a negative SPEED
        MoveCamera(-speed);
    }
    // Check if we hit the Left arrow or the 'a' key
    if( mleft )                                             // if camera has to strafe towards its left
    {
        // Strafe the camera left
        StrafeCamera(-speed);
    }
    // Check if we hit the Right arrow or the 'd' key 
    if( mright )                                            // if camera has to strafe towards its right
    {
        // Strafe the camera right
        StrafeCamera(speed);
    }
    
    if( mspace )                                            // Jump
    {
        if(g_Level.IsOnGround())
		   g_vVelocity.y = kJumpAcceleration;		   
    }


	// Now that we moved, let's get the current position and test our movement
	// vector against the level data to see if there is a collision.
	glm::vec3 vCurrentPosition = Position();

	// Check for collision with AABB's and grab the new position
	glm::vec3 vNewPosition = g_Level.TraceBox(vOldPosition, vCurrentPosition,
		                                     glm::vec3(-20, -50, -20), glm::vec3(20, 50, 20));

	// Check if we collided and we moved backwards
	if(g_Level.Collided() && bMovedBack)
	{
		// If or x or y didn't move, then we are backed into a wall so restore the view vector
		if(vNewPosition.x == vOldPosition.x || vNewPosition.z == vOldPosition.z)
			m_vView = vOldView;		
	}

	// Set the new position that was returned from our trace function
	m_vPosition = vNewPosition;


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// After we check for collision, we only want to add the velocity vector to
	// our view vector when we are falling.  If we aren't on the ground then
	// we don't want to push the the camera view down to the ground.  It's okay
	// if the position goes down because the collision detection fixes that so
	// we don't go through the ground, however, it's not natural to push the view
	// down too.  Well, assuming is strong enough to push our face down to the ground :)
	if(!g_Level.IsOnGround())
		m_vView = m_vView + g_vVelocity;
	else
	{
		// If we ARE on the ground, we want to get rid of the jump acceleration
		// that we add when the user hits the space bar.  Below we check to see
		// if our velocity is below 0 then we are done with our jump and can just
		// float back to the ground by the gravity.  We do also add our gravity
		// acceleration to the velocity every frame, so this resets this to zero
		// for that as well.
		if(g_vVelocity.y < 0)
			g_vVelocity.y = 0;
	}

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
}


///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera's view and strafe vector
/////
///////////////////////////////// UPDATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Update() 
{
	// Initialize a variable for the cross product result
	glm::vec3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

	// Normalize the strafe vector
	m_vStrafe = Normalize(vCross);

	// Move the camera's view by the mouse
	SetViewByMouse();

	// This checks to see if the keyboard was pressed
	CheckForMovement();
	
	// Calculate our frame rate and set our frame interval for time based movement
	CalculateFrameRate();    
}

glm::mat4 CCamera::GetViewMatrix()
   {
        return glm::lookAt(this->m_vPosition, this->m_vPosition + this->m_vView, this->m_vUpVector);
   }
   
glm::mat4 CCamera::Getview()
   {
   	return glm::mat4(glm::mat3(GetViewMatrix()));      
   }	

///////////////////////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This updates the camera according to the 
/////
///////////////////////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CCamera::Look()
{
	// Give openGL our camera position, then camera view, then camera up vector
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vView.x,	 m_vView.y,     m_vView.z,	
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}


