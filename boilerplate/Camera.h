#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
class Camera{
public:
	Camera(int iWidth, int iHeight);

	// Updating Functions
	void updateWidthAndHeight(int iWidth, int iHeight);

	// Set up Camera Matrices
	glm::mat4 getToCameraMat() const;
	glm::mat4 getPerspectiveMat() const;


	void setFOV_Y(float fFOVY) { m_fFOV_Y = fFOVY; }
	void setZRange(float fClose, float fFar)
	{
		m_fZClose = fClose;
		m_fZFar = fFar;
	}
	void setQuatRotation(float w) {
		m_pRotation.w = w;
	}
	float getQuatRotation() {
		return m_pRotation.w;
	}
	glm::vec3 getLookPos() {
		return m_LookPos;
	}
	void setLookPos(glm::vec3 m_LookPos) {
		this->m_LookPos = m_LookPos;
	}
private:
	glm::vec3 m_LookPos;
	glm::quat m_pRotation;
	const glm::vec3 offSetCamera = glm::vec3(0,3,-3);
	int m_iHeight, m_iWidth;
	float m_fAspectRatio;
	float m_fFOV_Y;
	float m_fZClose, m_fZFar;

	glm::vec3 getCartesianPos() const;
};