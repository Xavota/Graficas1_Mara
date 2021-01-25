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

void OBJInstance::setPosition(Vector pos)
{
	m_pos = pos;
}

Vector OBJInstance::getPosition()
{
	return m_pos;
}

void OBJInstance::setMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

Mesh* OBJInstance::getMesh()
{
	return m_mesh;
}
