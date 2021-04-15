#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "TextureManager.h"
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
	bool LoadModel(const class aiScene* scene, string fileName, unsigned int Flags, MATRIX mat);
	void Draw(class RenderManager* renderManager);

	Texture& getTexture(unsigned int index) { return m_textures[index]; }
	void setTexture(Texture tex) { m_textures.push_back(tex); }

	size_t getTextureCount() { return m_textures.size(); }

	string getFilePath() { return m_filePath; }
	string getName() { return m_name; }

public:
	//ShaderResourceView m_texture;
	vector<Texture> m_textures;
	vector<Mesh> m_modelMeshes;

	string m_filePath;
	string m_name;
#if !defined(OGL)
	PRIMITIVE_TOPOLOGY m_topology;
#else
	unsigned int m_topology;
#endif
};
}
