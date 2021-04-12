#include "OBJInstance.h"
#include "RenderManager.h"

namespace GraphicsModule
{
OBJInstance::OBJInstance(Vector pos)
{
	m_pos = pos;
}

OBJInstance::OBJInstance()
{
	m_pos = { 0,0,0 };
}

OBJInstance::~OBJInstance()
{
}

#if defined(DX11)
XMMATRIX OBJInstance::getModelMatrix()
{
	XMMATRIX scale = XMMatrixScaling(m_size.x(), m_size.y(), m_size.z());
	XMMATRIX translation = XMMatrixTranslation(m_pos.x(), m_pos.y(), m_pos.z());
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(m_rot.x(), m_rot.y(), m_rot.z());

	XMMATRIX res = XMMatrixMultiply(scale, translation);
	res = XMMatrixMultiplyTranspose(rotation, res);

	return res;
}
#elif defined(OGL)
glm::mat4 OBJInstance::getModelMatrix()
{
	glm::mat4 res = glm::scale(glm::mat4(1.0f), glm::vec3(m_size.x(), m_size.y(), m_size.z()));	
	res = glm::translate(res, glm::vec3(m_pos.x(), m_pos.y(), m_pos.z()));
	res = glm::rotate(res, glm::radians(m_rot.x() * 180 / 3.1415f), glm::vec3(1, 0, 0));
	res = glm::rotate(res, glm::radians(m_rot.y() * 180 / 3.1415f), glm::vec3(0, 1, 0));
	res = glm::rotate(res, glm::radians(m_rot.z() * 180 / 3.1415f), glm::vec3(0, 0, 1));

	//glm::mat4 res = scale * translation;
	//res = rotation * res;

	return res;
}
#endif

void OBJInstance::LoadModel(const aiScene* scene, string fileName)
{
	m_OBJModel.LoadModel(scene, fileName);
}

void OBJInstance::setSize(Vector size)
{
	m_size = size;
}

Vector OBJInstance::getSize()
{
	return m_size;
}

void OBJInstance::setPosition(Vector pos)
{
	m_pos = pos;
}

Vector OBJInstance::getPosition()
{
	return m_pos;
}

void OBJInstance::setRotation(Vector rot)
{
	m_rot = rot;
}

Vector OBJInstance::getRotation()
{
	return m_rot;
}

void OBJInstance::Draw(RenderManager* renderManager)
{
#if defined(DX11)
	ModelMat cb;
	cb.model = getModelMatrix();
	renderManager->UpdateSubresource(renderManager->m_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
#elif defined(OGL)
	glm::mat4 mat = getModelMatrix();
	renderManager->UpdateModelMatrix(MATRIX(&mat[0][0]));
#endif           

	m_OBJModel.Draw(renderManager);
}
}