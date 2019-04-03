#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "GeneralInclude.h"
/***********\
 * Defines *
\***********/
// Vector Indexing
#define I_THETA             0        // Spherical
#define I_PHI               1
#define I_RADIUS            2
#define I_X                 0        // Cartesian
#define I_Y                 1
#define I_Z                 2

// Shortened Indexing
#define PHI                 m_vPos[I_PHI]
#define THETA               m_vPos[I_THETA]
#define RADIUS              m_vPos[I_RADIUS]
#define X_LOOK              m_vPos[I_X]
#define Y_LOOK              m_vPos[I_Y]
#define Z_LOOK              m_vPos[I_Z]

// Default Camera Values
#define FOV_Y               60.f        // Degrees
#define Z_CLOSE             0.001f
#define Z_FAR               10000.f

using namespace glm;
/*************************************************************************\
 * Constructors                                                             *
\*************************************************************************/
// Default Constructor:
//        Requires an EntityID for the Entity that the component is a part of
//            and a ComponentID issued by the EntityManager.
Camera::Camera(int iWidth, int iHeight)
{
	updateWidthAndHeight(iWidth, iHeight);
	setFOV_Y(FOV_Y);
	setZRange(Z_CLOSE, Z_FAR);
	m_pRotation = glm::quat();
}

/*************************************************************************\
 * Getters                                                                *
\*************************************************************************/

// Generates toCamera Matrix and updates Uniform in ShaderManager.
mat4 Camera::getToCameraMat() const
{
	glm::vec3 vCamCPos = getCartesianPos();
	return lookAt(vCamCPos, m_LookPos, vec3(0,1,0));
}

// Generates toCamera Matrix and updates Uniform in ShaderManager.
mat4 Camera::getPerspectiveMat() const
{
	return perspective(m_fFOV_Y * DEGREES_TO_RADIANS, m_fAspectRatio, m_fZClose, m_fZFar);
}

// Returns the Current Camera Position in Cartesian Coordinates
vec3 Camera::getCartesianPos() const
{
	glm::mat3 RotationMatrix = glm::toMat3(m_pRotation);

	return RotationMatrix*offSetCamera + m_LookPos;
}
// Handle logic for changing window size.
void Camera::updateWidthAndHeight(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	m_fAspectRatio = (float)m_iWidth / (float)m_iHeight;
}
