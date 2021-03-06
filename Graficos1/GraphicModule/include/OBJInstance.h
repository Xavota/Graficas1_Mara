#pragma once
#include "Vector.h"
#include "Mesh.h"

#if defined(DX11)
#include <xnamath.h>
#endif

namespace GraphicsModule
{
class OBJInstance
{
public:
	OBJInstance(Mesh* mesh, Vector pos);
	OBJInstance();
	~OBJInstance();

#if defined(DX11)
	XMMATRIX getModelMatrix();
#endif

	void setSize(Vector size);
	Vector getSize();

	void setPosition(Vector pos);
	Vector getPosition();

	void setRotation(Vector rot);
	Vector getRotation();

	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	ShaderResourceView& getTexture() { return m_texture; }
	void setTexture(ShaderResourceView tex) { m_texture = tex; }

private:
	Mesh* m_mesh;

	Vector m_size{1,1,1};
	Vector m_pos{0,0,0};
	Vector m_rot{0,0,0};

	ShaderResourceView m_texture;
};
}
