#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "Main.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// This will allow us to create an object to keep track of our frustum
class CFrustum {

public:

	// Call this every time the camera moves to update the frustum
	void CalculateFrustum();

	// This takes a 3D point and returns TRUE if it's inside of the frustum
	bool PointInFrustum(float x, float y, float z);

	// This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
	bool SphereInFrustum(float x, float y, float z, float radius);

	// This takes the center and half the length of the cube.
	bool CubeInFrustum(float x, float y, float z, float size);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// This checks if a box is in the frustum
	bool BoxInFrustum(float x, float y, float z, float sizeX, float sizeY, float sizeZ);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


private:

	// This holds the A B C and D values for each side of our frustum.
	float m_Frustum[6][4];
};


// This is our debug lines class to view the octree visually
class CDebug 
{

public:

	// This adds a line to our list of debug lines
	void AddDebugLine(glm::vec3 vPoint1, glm::vec3 vPoint2);

	// This adds a rectangle with a given center, width, height and depth to our list
	void AddDebugBox(glm::vec3 vCenter, float width, float height, float depth);

	// This renders all of the lines
	void RenderDebugLines();		

	// This clears all of the debug lines
	void Clear();					

private:

	// This is the vector list of all of our lines
	vector<glm::vec3> m_vLines;		
};


#endif

