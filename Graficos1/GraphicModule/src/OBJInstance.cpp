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

MATRIX OBJInstance::getModelMatrix()
{
#if defined(DX11)
	XMMATRIX scale = XMMatrixScaling(m_size.x(), m_size.y(), m_size.z());
	XMMATRIX translation = XMMatrixTranslation(m_pos.x(), m_pos.y(), m_pos.z());
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(m_rot.x(), m_rot.y(), m_rot.z());

	XMMATRIX res = XMMatrixMultiply(scale, translation);
	res = XMMatrixMultiplyTranspose(rotation, res);

	return MATRIX((float*)&res);
#elif defined(OGL)
	glm::mat4 res = glm::scale(glm::mat4(1.0f), glm::vec3(m_size.x(), m_size.y(), m_size.z()));	
	res = glm::translate(res, glm::vec3(m_pos.x(), m_pos.y(), m_pos.z()));
	res = glm::rotate(res, glm::radians(m_rot.x() * 180 / 3.1415f), glm::vec3(1, 0, 0));
	res = glm::rotate(res, glm::radians(m_rot.y() * 180 / 3.1415f), glm::vec3(0, 1, 0));
	res = glm::rotate(res, glm::radians(m_rot.z() * 180 / 3.1415f), glm::vec3(0, 0, 1));

	return MATRIX((float*)&res);
#endif
	return MATRIX();
}

MATRIX OBJInstance::getModelMatrix(Vector size, Vector pos, Vector rot)
{
#if defined(DX11)
	XMMATRIX scale = XMMatrixScaling(size.x(), size.y(), size.z());
	XMMATRIX translation = XMMatrixTranslation(pos.x(), pos.y(), pos.z());
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rot.x(), rot.y(), rot.z());

	XMMATRIX res = XMMatrixMultiply(scale, translation);
	res = XMMatrixMultiplyTranspose(rotation, res);

	return MATRIX((float*)&res);
#elif defined(OGL)
	glm::mat4 res = glm::scale(glm::mat4(1.0f), glm::vec3(size.x(), size.y(), size.z()));
	res = glm::translate(res, glm::vec3(pos.x(), pos.y(), pos.z()));
	res = glm::rotate(res, glm::radians(rot.x() * 180 / 3.1415f), glm::vec3(1, 0, 0));
	res = glm::rotate(res, glm::radians(rot.y() * 180 / 3.1415f), glm::vec3(0, 1, 0));
	res = glm::rotate(res, glm::radians(rot.z() * 180 / 3.1415f), glm::vec3(0, 0, 1));

	return MATRIX((float*)&res);
#endif

	return MATRIX();
}

bool OBJInstance::LoadModel(const aiScene* scene, string fileName, unsigned int Flags, MATRIX mat)
{
	return m_OBJModel.LoadModel(scene, fileName, Flags, mat);
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
	renderManager->UpdateModelMatrix(getModelMatrix());

	m_OBJModel.Draw(renderManager);
}
}