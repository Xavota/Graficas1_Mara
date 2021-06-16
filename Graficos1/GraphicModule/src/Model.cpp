#include "Model.h"
#include "RenderManager.h"

namespace GraphicsModule
{
void Model::AddMeshes(vector<Mesh> meshes)
{
	m_modelMeshes = meshes;
}

bool Model::LoadModel(const aiScene* scene, string fileName, unsigned int Flags, MATRIX mat, eDIMENSION dim)
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
	aiString completePathDiffuse;
	completePathDiffuse.Append(fileName.c_str());
	completePathDiffuse.Append("/../../Textures/M_");
	completePathDiffuse.Append(m_name.c_str());
	completePathDiffuse.Append("_Albedo");
	if (dim == eDIMENSION::TEXTURE2D)
		completePathDiffuse.Append(".jpg");
	else if (dim == eDIMENSION::TEX_CUBE)
		completePathDiffuse.Append(".dds");
	eSTATUS stat = TextureManager::CreateTextureFromFile(completePathDiffuse.C_Str(), m_name + "_Albedo", Flags, dim);
	//GetManager()->getShader().AddPassInputTexture("Lights", "diffuse");
	if (stat == eSTATUS::OK || stat == eSTATUS::REPITED)
	{
		setTexture(TextureManager::GetTexture(m_name + "_Albedo"));
	}
	else if (stat == eSTATUS::FAIL)
	{
		setTexture(TextureManager::GetTexture("Base Texture"));
	}

	aiString completePathNormal;
	completePathNormal.Append(fileName.c_str());
	completePathNormal.Append("/../../Textures/M_");
	completePathNormal.Append(m_name.c_str());
	completePathNormal.Append("_Normal.jpg");
	stat = TextureManager::CreateTextureFromFile(completePathNormal.C_Str(), m_name + "_Normal", Flags, dim);
	//GetManager()->getShader().AddPassInputTexture("Lights", "diffuse");
	if (stat == eSTATUS::OK || stat == eSTATUS::REPITED)
	{
		setTexture(TextureManager::GetTexture(m_name + "_Normal"));
	}
	else if (stat == eSTATUS::FAIL)
	{
		setTexture(TextureManager::GetTexture("Base Texture"));
	}

	aiString completePathSpecular;
	completePathSpecular.Append(fileName.c_str());
	completePathSpecular.Append("/../../Textures/M_");
	completePathSpecular.Append(m_name.c_str());
	completePathSpecular.Append("_Metallic.jpg");
	stat = TextureManager::CreateTextureFromFile(completePathSpecular.C_Str(), m_name + "_Specular", Flags, dim);
	//GetManager()->getShader().AddPassInputTexture("Lights", "diffuse");
	if (stat == eSTATUS::OK || stat == eSTATUS::REPITED)
	{
		setTexture(TextureManager::GetTexture(m_name + "_Specular"));
	}
	else if (stat == eSTATUS::FAIL)
	{
		setTexture(TextureManager::GetTexture("Base Texture"));
	}

	aiString completePathAO;
	completePathAO.Append(fileName.c_str());
	completePathAO.Append("/../../Textures/M_");
	completePathAO.Append(m_name.c_str());
	completePathAO.Append("_AO.jpg");
	stat = TextureManager::CreateTextureFromFile(completePathAO.C_Str(), m_name + "_AO", Flags, dim);
	//GetManager()->getShader().AddPassInputTexture("Lights", "diffuse");
	if (stat == eSTATUS::OK || stat == eSTATUS::REPITED)
	{
		setTexture(TextureManager::GetTexture(m_name + "_AO"));
	}
	else if (stat == eSTATUS::FAIL)
	{
		setTexture(TextureManager::GetTexture("Base Texture"));
	}
	//TextureManager::CreateTextureFromFile("C:/Users/marad/OneDrive/Documents/GitHub/Graficas1_Mara/Graficos1/bin/Models/Textures/M_Pistola_Normal.jpg", "norm", MODEL_LOAD_FORMAT_BGRA);
	//setTexture(TextureManager::GetTexture("norm")); 
	//GetManager()->getShader().AddPassInputTexture("Lights", "normal");
	//TextureManager::CreateTextureFromFile("C:/Users/marad/OneDrive/Documents/GitHub/Graficas1_Mara/Graficos1/bin/Models/Textures/M_Pistola_Metallic.jpg", "spec", MODEL_LOAD_FORMAT_BGRA);
	//setTexture(TextureManager::GetTexture("spec"));
	//GetManager()->getShader().AddPassInputTexture("Lights", "specular");

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
				v.Pos = mat * Vector4{v.Pos.x, v.Pos.y, v.Pos.z, 1.0f};
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
					v.Tex.y = scene->mMeshes[i]->mTextureCoords[k][j].y;

				}
			}

			if (scene->mMeshes[i]->HasNormals())
			{
				v.Normales.x = scene->mMeshes[i]->mNormals[j].x;
				v.Normales.y = scene->mMeshes[i]->mNormals[j].y;
				v.Normales.z = scene->mMeshes[i]->mNormals[j].z;
				v.Normales = mat * Vector4{v.Normales.x, v.Normales.y, v.Normales.z, 0.0f};
			}
			else
			{
				v.Normales = Vector3{};
			}

			vertices.push_back(v);
		}

		for (int j = 0; j * 3 + 2 < scene->mMeshes[i]->mNumVertices; ++j)
		{
			aiVector3D deltaPos1 = scene->mMeshes[i]->mVertices[j * 3 + 1] - scene->mMeshes[i]->mVertices[j * 3];
			aiVector3D deltaPos2 = scene->mMeshes[i]->mVertices[j * 3 + 2] - scene->mMeshes[i]->mVertices[j * 3];
			aiVector3D deltaUV1 = scene->mMeshes[i]->mTextureCoords[0][j * 3 + 1] - scene->mMeshes[i]->mTextureCoords[0][j * 3];
			aiVector3D deltaUV2 = scene->mMeshes[i]->mTextureCoords[0][j * 3 + 2] - scene->mMeshes[i]->mTextureCoords[0][j * 3];
			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			aiVector3D tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			aiVector3D bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

			vertices[j * 3 + 0].Binormal.x = bitangent.x;
			vertices[j * 3 + 0].Binormal.y = bitangent.y;
			vertices[j * 3 + 0].Binormal.z = bitangent.z;

			vertices[j * 3 + 1].Binormal.x = bitangent.x;
			vertices[j * 3 + 1].Binormal.y = bitangent.y;
			vertices[j * 3 + 1].Binormal.z = bitangent.z;
											 
			vertices[j * 3 + 2].Binormal.x = bitangent.x;
			vertices[j * 3 + 2].Binormal.y = bitangent.y;
			vertices[j * 3 + 2].Binormal.z = bitangent.z;


			vertices[j * 3 + 0].Tangente.x = tangent.x;
			vertices[j * 3 + 0].Tangente.y = tangent.y;
			vertices[j * 3 + 0].Tangente.z = tangent.z;

			vertices[j * 3 + 1].Tangente.x = tangent.x;
			vertices[j * 3 + 1].Tangente.y = tangent.y;
			vertices[j * 3 + 1].Tangente.z = tangent.z;

			vertices[j * 3 + 2].Tangente.x = tangent.x;
			vertices[j * 3 + 2].Tangente.y = tangent.y;
			vertices[j * 3 + 2].Tangente.z = tangent.z;
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

#if !defined(OGL)
	if (Flags & MODEL_LOAD_FORMAT_TRIANGLES)
		m_topology = PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	if (Flags & MODEL_LOAD_FORMAT_WIREFRAME)
		m_topology = PRIMITIVE_TOPOLOGY_LINELIST;
	if (Flags & MODEL_LOAD_FORMAT_POINTS)
		m_topology = PRIMITIVE_TOPOLOGY_POINTLIST;
#else
	if (Flags & MODEL_LOAD_FORMAT_TRIANGLES)
		m_topology = GL_FILL;
	if (Flags & MODEL_LOAD_FORMAT_WIREFRAME)
		m_topology = GL_LINE;
	if (Flags & MODEL_LOAD_FORMAT_POINTS)
		m_topology = GL_POINT;
#endif

	return true;
}

void Model::Draw(RenderManager* renderManager, bool useTextures)
{
	for (int i = 0; i < m_modelMeshes.size(); ++i)
	{
		//Texture
		//renderManager->PSSetShaderResources(0, 1, m_textures[m_modelMeshes[i].getMaterialID()].getBuffer());
#if defined(DX11)
		/*Set primitive topology*/
		//renderManager->IASetPrimitiveTopology(m_topology);

		//renderManager->PSSetShaderResources(0, m_textures);

#elif defined(OGL)
		/*Set primitive topology*/
		/*glPolygonMode(GL_FRONT_AND_BACK, m_topology);

		renderManager->ShaderSetFloat("mat1.normalMap", 1);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_textures[1].getID());

		renderManager->ShaderSetFloat("mat1.diffuseMap", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textures[1].getID());

		renderManager->ShaderSetFloat("mat1.specularMap", 2);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, m_textures[2].getID());*/
#endif

		m_modelMeshes[i].Draw(renderManager);
	}
}
void Model::SetResources(RenderManager* renderManager, bool useTextures)
{
#if defined(DX11)
	renderManager->IASetPrimitiveTopology(m_topology);
#elif defined(OGL)
	glPolygonMode(GL_FRONT_AND_BACK, m_topology);
#endif

	if (useTextures)
	{
#if defined(DX11)
		renderManager->getShader("Deferred").SetPassInputTexture("GBuffer", "diffuse", m_textures[0]);
		renderManager->getShader("Deferred").SetPassInputTexture("GBuffer", "normal", m_textures[1]);
		renderManager->getShader("Deferred").SetPassInputTexture("GBuffer", "specular", m_textures[2]);

		renderManager->getShader("Deferred").SetPassInputTexture("SkyBox", "diffuse", m_textures[0]);


		renderManager->getShader("Forward").SetPassInputTexture("Lights", "diffuse", m_textures[0]);
		renderManager->getShader("Forward").SetPassInputTexture("Lights", "normal", m_textures[1]);
		renderManager->getShader("Forward").SetPassInputTexture("Lights", "specular", m_textures[2]);
		renderManager->getShader("Forward").SetPassInputTexture("Lights", "AO", m_textures[3]);

		renderManager->getShader("Forward").SetPassInputTexture("SkyBox", "diffuse", m_textures[0]);
#elif defined(OGL)
		renderManager->getShader("Deferred").SetPassInputTexture("GBuffer", "diffuse", m_textures[0]);
		renderManager->getShader("Deferred").SetPassInputTexture("GBuffer", "normal", m_textures[1]);
		renderManager->getShader("Deferred").SetPassInputTexture("GBuffer", "specular", m_textures[2]);
#endif
	}

	for (int i = 0; i < m_modelMeshes.size(); ++i)
	{
		m_modelMeshes[i].SetResources(renderManager);
	}
}
}