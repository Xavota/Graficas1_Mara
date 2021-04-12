#pragma once
#include "Vector.h"
#include "Model.h"
#include "Descriptors.h"
#include <vector>

#include <assimp/scene.h>

using std::vector;

#if defined(DX11)
#include <xnamath.h>
#elif defined(OGL)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace GraphicsModule
{
class OBJInstance
{
public:
	OBJInstance(Vector pos);
	OBJInstance();
	~OBJInstance();

#if defined(DX11)
	XMMATRIX getModelMatrix();
#elif defined(OGL)
	glm::mat4 getModelMatrix();
#endif

	void LoadModel(const aiScene* scene, string fileName);

	void setSize(Vector size);
	Vector getSize();

	void setPosition(Vector pos);
	Vector getPosition();

	void setRotation(Vector rot);
	Vector getRotation();

	Texture& getTexture(unsigned int index) { return m_OBJModel.getTexture(index); }
	void setTexture(Texture tex) { m_OBJModel.setTexture(tex); }

	size_t getTextureCount() { return m_OBJModel.getTextureCount(); }

	string getFilePath() { return m_OBJModel.getFilePath();	}
	string getName() { return m_OBJModel.getName(); }

	void setColor(Color col) { m_color = col; }

	void Draw(class RenderManager* renderManager);

public:
	Vector m_size{1,1,1};
	Vector m_pos{0,0,0};
	Vector m_rot{0,0,0};

	Model m_OBJModel;

	Color m_color;
};
}
