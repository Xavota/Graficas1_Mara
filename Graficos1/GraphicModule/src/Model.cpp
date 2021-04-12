#include "Model.h"
#include "RenderManager.h"

namespace GraphicsModule
{
void Model::AddMeshes(vector<Mesh> meshes)
{
	m_modelMeshes = meshes;
}

void Model::LoadModel(const aiScene* scene, string fileName)
{
	m_filePath = fileName;
	bool point = false;
	for (int i = fileName.size() - 1; i >= 0; --i)
	{
		if (!point)
		{
			if (fileName[i] == '.')
			{
				point = true;
			}
		}
		else
		{
			if (fileName[i] != '/' && fileName[i] != '\\')
				m_name = fileName[i] + m_name;
			else
				break;
		}
	}

	aiString path;

	/*for (int i = 0; i < scene->mNumMaterials; i++)
	{
		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiGetMaterialTexture(scene->mMaterials[i], aiTextureType_DIFFUSE, 0, &path);

			aiString completePath;
			completePath.Append(fileName.c_str());
			completePath.Append("/../../Textures/");
			completePath.Append(path.C_Str());

			aiString completeName;
			completeName.Append("Texture ");
			completeName.Append(completePath.C_Str());
			cout << "\tPath: " << completePath.C_Str() << endl;
			if (GraphicsModule::TextureManager::CreateTextureFromFile(completePath.C_Str(), completeName.C_Str()))
				setTexture(GraphicsModule::TextureManager::GetTexture(completeName.C_Str()));
			else
			{
				setTexture(GraphicsModule::TextureManager::GetTexture("Base Texture"));
			}
		}
		else
		{
			setTexture(GraphicsModule::TextureManager::GetTexture("Base Texture"));
		}
	}*/
	aiString completePath;
	completePath.Append(fileName.c_str());
	completePath.Append("/../../Textures/M_");
	completePath.Append(m_name.c_str());
	completePath.Append("_Albedo.jpg");
	if (TextureManager::CreateTextureFromFile(completePath.C_Str(), m_name))
	{
		setTexture(TextureManager::GetTexture(m_name));
	}
	else
	{
		setTexture(TextureManager::GetTexture("Base Texture"));
	}

	HRESULT hr;
	
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		vector<Vertex> vertices;

		m_modelMeshes.push_back(Mesh());

		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j)
		{
			Vertex v;
			if (scene->mMeshes[i]->HasPositions())
			{
				v.Pos.x = scene->mMeshes[i]->mVertices[j].x;
				v.Pos.y = scene->mMeshes[i]->mVertices[j].y;
				v.Pos.z = scene->mMeshes[i]->mVertices[j].z;
			}
			else
			{
				v.Pos = Vector3{};
			}

			for (int k = 0; k < scene->mMeshes[i]->mMaterialIndex; ++k)
			{
				if (scene->mMeshes[i]->HasTextureCoords(k))
				{
					v.Tex.x = scene->mMeshes[i]->mTextureCoords[k][j].x;
#if defined(DX11)
					v.Tex.y = 1 - scene->mMeshes[i]->mTextureCoords[k][j].y;
#elif defined(OGL)
					v.Tex.y = scene->mMeshes[i]->mTextureCoords[k][j].y;
#endif
				}
			}

			if (scene->mMeshes[i]->HasNormals())
			{
				v.Normales.x = scene->mMeshes[i]->mNormals[j].x;
				v.Normales.y = scene->mMeshes[i]->mNormals[j].y;
				v.Normales.z = scene->mMeshes[i]->mNormals[j].z;
			}
			else
			{
				v.Normales = Vector3{};
			}

			vertices.push_back(v);
		}

		hr = m_modelMeshes[m_modelMeshes.size() - 1].setVertex(vertices);

		vector<unsigned int> indices;

		for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
		{
			for (int k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; ++k)
			{
				indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);
			}
		}

		hr = m_modelMeshes[m_modelMeshes.size() - 1].setIndices(indices);

		indices.clear();
		vertices.clear();

		m_modelMeshes[m_modelMeshes.size() - 1].setMaterialID(scene->mMeshes[i]->mMaterialIndex);
	}
}

void Model::Draw(RenderManager* renderManager)
{
	for (int i = 0; i < m_modelMeshes.size(); ++i)
	{
		//Texture
		//renderManager->PSSetShaderResources(0, 1, m_textures[m_modelMeshes[i].getMaterialID()].getBuffer());
#if defined(DX11)
		renderManager->PSSetShaderResources(0, 1, m_textures[0].getBuffer());
#elif defined(OGL)
		glBindTexture(GL_TEXTURE_2D, m_textures[0].getID());
#endif

		m_modelMeshes[i].Draw(renderManager);
	}
}
}