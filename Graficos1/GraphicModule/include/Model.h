#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Bone.h"
#include "SkeletalMesh.h"
#include <vector>

#include <iostream>

#include <assimp/scene.h>

using std::vector;
using std::cout;
using std::endl;
using std::string;

namespace GraphicsModule
{
class Model
{
public:
	Model() = default;
	~Model() = default;

	void AddMeshes(vector<Mesh> meshes);
	bool LoadModel(const class aiScene* scene, string fileName, unsigned int Flags, MATRIX mat, eDIMENSION dim, std::vector<std::vector<Bone>> bones);
	void Draw(class RenderManager* renderManager, bool useTextures, SkeletalMesh* sk);
	void SetResources(RenderManager* renderManager, bool useTextures);

	Texture& getTexture(unsigned int meshIndex, unsigned int textureIndex) { return m_textures[meshIndex][textureIndex]; }
	void setTexture(unsigned int meshIndex, Texture tex) { m_textures[meshIndex].push_back(tex); }

	size_t getTextureCount(unsigned int meshIndex) 
	{
		return m_textures[meshIndex].size();
	}
	size_t getMeshCount()
	{
		return m_modelMeshes.size();
	}

	string getFilePath() { return m_filePath; }
	string getName() { return m_name; }

public:
	//ShaderResourceView m_texture;
	vector<vector<Texture>> m_textures;
	vector<Mesh> m_modelMeshes;

	string m_filePath;
	string m_name;
#if !defined(OGL)
	PRIMITIVE_TOPOLOGY m_topology;
#else
	unsigned int m_topology;
#endif

	bool yaBones = false;
};
}
