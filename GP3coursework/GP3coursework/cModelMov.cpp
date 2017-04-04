#include "cModelMov.h"

cModelMov::cModelMov() : cModel()
{

}

void cModelMov::attachInputMgr(cInputMgr* inputMgr)
{
	m_InputMgr = inputMgr;
}

void cModelMov::update(float elapsedTime)
{
	if (m_InputMgr->isKeyDown(0x44))
	{
		rotationAngle += 5.0f;
	}
	if (m_InputMgr->isKeyDown(0x41))
	{
		rotationAngle -= 5.0f;
	}
	if (m_InputMgr->isKeyDown(0x57))
	{
		transZ += 5.0f;
	}
	if (m_InputMgr->isKeyDown(0x53))
	{
		transZ -= 5.0f;
	}

	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = -(float)glm::sin(glm::radians(m_mdlRotation));
	mdlVelocityAdd.y = 0.0f;
	mdlVelocityAdd.z = -(float)glm::cos(glm::radians(m_mdlRotation));

	m_mdlRotation -= rotationAngle;

	mdlVelocityAdd *= transZ;
	m_mdlDirection += mdlVelocityAdd;

	m_mdlPosition += m_mdlDirection * m_mdlSpeed *elapsedTime;
	m_mdlDirection *= 0.95f;

	rotationAngle = 0;
	transZ = 0;
}

cModelMov::~cModelMov()
{

}