#include <cmath>
#include "glee/GLee.h"
#include "geom.h"
#include "frustum.h"

Plane Frustum::extractPlane(float a, float b, float c, float d)
{
	float t = sqrt( a * a + b * b + c * c );
	Plane p;
	p.a = a / t;
	p.b = b / t;
	p.c = c / t;
	p.d = d / t;
	return p;
}

bool Frustum::sphereInFrustum(float x, float y, float z, float radius)
{
	for (int p = 0; p < 6; p++)
    {
		if (m_planes[p].a * x + 
            m_planes[p].b * y + 
            m_planes[p].c * z + 
            m_planes[p].d <= -radius)
        {
			return false;
        }
    }
	return true;
}

bool Frustum::PointInFrustum( float x, float y, float z )
{
	for (int p = 0; p < 6; p++)
    {
		if (m_planes[p].a * x + 
            m_planes[p].b * y + 
            m_planes[p].c * z + 
            m_planes[p].d < 0)
        {
			return false;
        }
    }
	return true;
}

bool Frustum::PointOnPlane(Vector3 point, int planeIndex)
{
	if (m_planes[planeIndex].a * point.x + 
		m_planes[planeIndex].b * point.y + 
		m_planes[planeIndex].c * point.z + 
		m_planes[planeIndex].d < 0)
	{
		return false;
	}

	return true;
}

bool Frustum::BoxInFrustum(float minX, float minZ, float maxX, float maxZ)
{
	//If all 8 points are not on the correct side
	//of the plane, then the entitys is not in the Frustum
	int total = 0;
	Vector3 points[8];

	points[0] = Vector3(minX, -100, minZ);
	points[1] = Vector3(minX, -100, maxZ);
	points[2] = Vector3(maxX, -100, maxZ);
	points[3] = Vector3(maxX, -100, minZ);

	points[4] = Vector3(minX, 100, minZ);
	points[5] = Vector3(minX, 100, maxZ);
	points[6] = Vector3(maxX, 100, maxZ);
	points[7] = Vector3(maxX, 100, minZ);

	for(int j = 0; j < 6; j++)
	{
		for(int i = 0; i < 8; i++)
		{
			Vector3 temp = points[i];
			
			if(!PointOnPlane(temp, j))
			{
				total++;
			}
		}

		if(total >= 8)//if all 8 points are not on a plane then it is not in the frustum
		{
			return false;
		}

		total = 0;
	}

	return true;
}

void Frustum::updateFrustum()
{
    GLfloat projection[16];
    GLfloat modelview[16];
	GLfloat mvp[16];

	/* Get the current PROJECTION and MODELVIEW matrices from OpenGL */
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	glPushMatrix();
    //Load the stored projection matrix
    glLoadMatrixf(projection);
	//multiply the stored MODELVIEW matrix with the projection matrix
	glMultMatrixf(modelview);
	//we read the result of the multiplication
	glGetFloatv(GL_MODELVIEW_MATRIX, mvp);
	//and restore the former MODELVIEW_MATRIX
	glPopMatrix();


	/* Extract the RIGHT plane */
	m_planes[PLANE_RIGHT] = extractPlane(mvp[ 3] - mvp[ 0],
	                                     mvp[ 7] - mvp[ 4],
	                                     mvp[11] - mvp[ 8],
	                                     mvp[15] - mvp[12]);

	/* Extract the LEFT plane */
	m_planes[PLANE_LEFT] = extractPlane(mvp[ 3] + mvp[ 0],
	                                    mvp[ 7] + mvp[ 4],
	                                    mvp[11] + mvp[ 8],
	                                    mvp[15] + mvp[12]);

	/* Extract the TOP plane */
	m_planes[PLANE_TOP] = extractPlane(mvp[ 3] - mvp[ 1],
	                                   mvp[ 7] - mvp[ 5],
	                                   mvp[11] - mvp[ 9],
	                                   mvp[15] - mvp[13]);

	/* Extract the BOTTOM plane */
	m_planes[PLANE_BOTTOM] = extractPlane(mvp[ 3] + mvp[ 1],
	                                      mvp[ 7] + mvp[ 5],
	                                      mvp[11] + mvp[ 9],
	                                      mvp[15] + mvp[13]);

	/* Extract the FAR plane */
	m_planes[PLANE_FAR] = extractPlane(mvp[ 3] - mvp[ 2],
	                                   mvp[ 7] - mvp[ 6],
	                                   mvp[11] - mvp[10],
	                                   mvp[15] - mvp[14]);

	/* Extract the NEAR plane */
	m_planes[PLANE_NEAR] = extractPlane(mvp[ 3] + mvp[ 2],
	                                   mvp[ 7] + mvp[ 6],
	                                   mvp[11] + mvp[10],
	                                   mvp[15] + mvp[14]);

}