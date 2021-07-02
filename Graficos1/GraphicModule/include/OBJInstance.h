#pragma once
#include "Vector.h"
#include "Model.h"
#include "Descriptors.h"
#include "SkeletalMesh.h"
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

/*#if defined(DX11)
	XMMATRIX getModelMatrix();
#elif defined(OGL)
	glm::mat4 getModelMatrix();
#endif*/

	MATRIX getModelMatrix();

/*#if defined(DX11)
	static XMMATRIX getModelMatrix(Vector size, Vector pos, Vector rot);
#elif defined(OGL)
	static glm::mat4 getModelMatrix(Vector size, Vector pos, Vector rot);
#endif*/

	static MATRIX getModelMatrix(Vector size, Vector pos, Vector rot);

	//bool LoadModel(const aiScene* scene, string fileName, unsigned int Flags, MATRIX mat, eDIMENSION dim);
	bool LoadModel(string fileName, unsigned int Flags, MATRIX mat, eDIMENSION dim);

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

	void Draw(class RenderManager* renderManager, bool useTextures);
	void SetResources(RenderManager* renderManager, bool useTextures);

public:
	Vector m_size{1,1,1};
	Vector m_pos{0,0,0};
	Vector m_rot{0,0,0};

	SkeletalMesh m_skMesh;
	Model m_OBJModel;

	Color m_color;

	const aiScene* m_scene;
};
}
