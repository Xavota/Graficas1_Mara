#pragma once
#include "Vector.h"
#include "Model.h"
#include "Descriptors.h"
#include "SkeletalMesh.h"
#include "Animation.h"
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

	OBJInstance(const OBJInstance& other);

	OBJInstance operator=(const OBJInstance& other);

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

	bool LoadSkeletalModel(aiNode* root, std::vector<std::vector<Bone>> bones);

	void setSize(Vector size);
	Vector getSize();

	void setPosition(Vector pos);
	Vector getPosition();

	void setRotation(Vector rot);
	Vector getRotation();

	Texture& getTexture(unsigned int meshIndex, unsigned int index) { return m_OBJModel.getTexture(meshIndex, index); }
	void setTexture(unsigned int meshIndex, Texture tex) { m_OBJModel.setTexture(meshIndex, tex); }

	size_t getTextureCount(unsigned int meshIndex) { return m_OBJModel.getTextureCount(meshIndex); }
	size_t getMeshCount()
	{
		return m_OBJModel.getMeshCount();
	}

	string getFilePath() { return m_OBJModel.getFilePath();	}
	string getName() { return m_OBJModel.getName(); }

	void setColor(Color col) { m_color = col; }

	SkeletalMesh* getSkeletalMesh() { return &m_skMesh; }

	void Update(float deltaTime);
	void Draw(class RenderManager* renderManager, bool useTextures);
	void SetResources(RenderManager* renderManager, bool useTextures);

	int getAnimationCount();
	int getCurrentAnimation();
	void setCurrentAnimation(int i);

	const char* getAnimationName(int animIndex);

public:
	Vector m_size{1,1,1};
	Vector m_pos{0,0,0};
	Vector m_rot{0,0,0};

	SkeletalMesh m_skMesh;
	Model m_OBJModel;
	std::vector<Animation> m_anims;

	Color m_color = Color{0,0,0,0};

	Assimp::Importer* m_importer;
	const aiScene* m_scene = nullptr;

	int m_currentAnimation = 0;
};
}
