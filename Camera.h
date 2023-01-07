#ifndef _CAMERA_H
#define _CAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// This is our camera class
class CCamera {

public:

	// Our camera constructor
	CCamera();	

	// These are are data access functions for our camera's private data
	glm::vec3 Position() {	return m_vPosition;		}
	glm::vec3 View()		{	return m_vView;			}
	glm::vec3 UpVector() {	return m_vUpVector;		}
	glm::vec3 Strafe()	{	return m_vStrafe;		}

	// These function set the position, view and up vector of the camera respectively
	void SetPosition(glm::vec3 vPosition)	{	m_vPosition = vPosition;	}
	void SetView(glm::vec3 vView)			{	m_vView = vView;			}
	void SetUpVector(glm::vec3 vUpVector)	{	m_vUpVector = vUpVector;	}
	
	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	// This rotates the camera's view around the position depending on the values passed in.
	void RotateView(float angle, float X, float Y, float Z);

	// This moves the camera's view by the mouse movements (First person view)
	void SetViewByMouse(); 

	// This rotates the camera around a point (I.E. your character).
	void RotateAroundPoint(glm::vec3 vCenter, float X, float Y, float Z);

	// This strafes the camera left or right depending on the speed (+/-) 
	void StrafeCamera(float speed);

	// This will move the camera forward or backward depending on the speed
	void MoveCamera(float speed);

	// This checks for keyboard movement
	void CheckForMovement();

	// This updates the camera's view and other data (Should be called each frame)
	void Update();

    	glm::mat4 GetViewMatrix();
    	
    	glm::mat4 Getview();    	

	// This uses gluLookAt() to tell OpenGL where to look
	void Look();
		
	// The camera's position
	glm::vec3 m_vPosition;					

	// The camera's view
	glm::vec3 m_vView;						

	// The camera's up vector
	glm::vec3 m_vUpVector;		
	
	// The camera's strafe vector
	glm::vec3 m_vStrafe;	
};

#endif


