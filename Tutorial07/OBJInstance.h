#pragma once
#include "Vector.h"
#include "Mesh.h"
class OBJInstance
{
public:
	OBJInstance(Mesh* mesh, Vector pos);
	OBJInstance();
	~OBJInstance();

	void setPosition(Vector pos);
	Vector getPosition();

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

private:
	Mesh* m_mesh;
	Vector m_pos;
};

