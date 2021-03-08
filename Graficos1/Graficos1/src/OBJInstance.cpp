#include "OBJInstance.h"

OBJInstance::OBJInstance(Mesh* mesh, Vector pos)
{
	m_mesh = mesh;
	m_pos = pos;
}

OBJInstance::OBJInstance()
{
	m_mesh = nullptr;
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
#endif

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

void OBJInstance::setMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

Mesh* OBJInstance::getMesh()
{
	return m_mesh;
}