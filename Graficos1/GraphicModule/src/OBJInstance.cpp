#include "OBJInstance.h"
#include "RenderManager.h"

namespace GraphicsModule
{
	OBJInstance::OBJInstance(Vector pos)
	{
		m_pos = pos;
	}
	
	OBJInstance::OBJInstance()
	{
		m_pos = { 0,0,0 };
	}

	OBJInstance::OBJInstance(const OBJInstance& other)
	{
		m_size = other.m_size;
		m_pos = other.m_pos;
		m_rot = other.m_rot;

		m_skMesh = other.m_skMesh;
		m_OBJModel = other.m_OBJModel;
		m_anims = other.m_anims;

		m_color = other.m_color;

		m_importer = other.m_importer;
		m_scene = other.m_scene;
	}

	OBJInstance OBJInstance::operator=(const OBJInstance& other)
	{
		m_size = other.m_size;
		m_pos = other.m_pos;
		m_rot = other.m_rot;

		m_skMesh = other.m_skMesh;
		m_OBJModel = other.m_OBJModel;
		m_anims = other.m_anims;

		m_color = other.m_color;

		//m_importer = other.m_importer;
		m_scene = other.m_scene;
		return *this;
	}
	
	OBJInstance::~OBJInstance()
	{
	}
	
	MATRIX OBJInstance::getModelMatrix()
	{
#if defined(DX11)
		XMMATRIX scale = XMMatrixScaling(m_size.x(), m_size.y(), m_size.z());
		XMMATRIX translation = XMMatrixTranslation(m_pos.x(), m_pos.y(), m_pos.z());
		XMMATRIX rotation = XMMatrixRotationRollPitchYaw(m_rot.x(), m_rot.y(), m_rot.z());
	
		XMMATRIX res = XMMatrixMultiply(scale, translation);
		res = XMMatrixMultiplyTranspose(rotation, res);
	
		return MATRIX((float*)&res);
#elif defined(OGL)
		glm::mat4 res = glm::scale(glm::mat4(1.0f), glm::vec3(m_size.x(), m_size.y(), m_size.z()));	
		res = glm::translate(res, glm::vec3(m_pos.x(), m_pos.y(), m_pos.z()));
		res = glm::rotate(res, glm::radians(m_rot.x() * 180 / 3.1415f), glm::vec3(1, 0, 0));
		res = glm::rotate(res, glm::radians(m_rot.y() * 180 / 3.1415f), glm::vec3(0, 1, 0));
		res = glm::rotate(res, glm::radians(m_rot.z() * 180 / 3.1415f), glm::vec3(0, 0, 1));
		
		return MATRIX((float*)&res);
#endif
		return MATRIX();
	}
	
	MATRIX OBJInstance::getModelMatrix(Vector size, Vector pos, Vector rot)
	{
#if defined(DX11)
		XMMATRIX scale = XMMatrixScaling(size.x(), size.y(), size.z());
		XMMATRIX translation = XMMatrixTranslation(pos.x(), pos.y(), pos.z());
		XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rot.x(), rot.y(), rot.z());
	
		XMMATRIX res = XMMatrixMultiply(scale, translation);
		res = XMMatrixMultiplyTranspose(rotation, res);
	
		return MATRIX((float*)&res);
#elif defined(OGL)
		glm::mat4 res = glm::scale(glm::mat4(1.0f), glm::vec3(size.x(), size.y(), size.z()));
		res = glm::translate(res, glm::vec3(pos.x(), pos.y(), pos.z()));
		res = glm::rotate(res, glm::radians(rot.x() * 180 / 3.1415f), glm::vec3(1, 0, 0));
		res = glm::rotate(res, glm::radians(rot.y() * 180 / 3.1415f), glm::vec3(0, 1, 0));
		res = glm::rotate(res, glm::radians(rot.z() * 180 / 3.1415f), glm::vec3(0, 0, 1));
	
		return MATRIX((float*)&res);
#endif
	
		return MATRIX();
	}
	
	/*bool OBJInstance::LoadModel(const aiScene* scene, string fileName, unsigned int Flags, MATRIX mat, eDIMENSION dim)
	{
		return m_OBJModel.LoadModel(scene, fileName, Flags, mat, dim);
	}*/

	bool OBJInstance::LoadModel(string fileName, unsigned int Flags, MATRIX mat, eDIMENSION dim)
	{
		m_importer = new Assimp::Importer();
		m_scene = m_importer->ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
		if (!m_scene)
		{
			cout << m_importer->GetErrorString() << endl;
			return false;
		}

		std::vector<std::vector<Bone>> bones;

		bones = m_skMesh.LoadSkeletalMesh(m_scene);

		if (m_scene->HasAnimations())
		{
			for (int i = 0; i < m_scene->mNumAnimations; i++)
			{
				m_anims.push_back(Animation());
				m_anims[i].LoadAnimation(m_scene->mAnimations[i], m_scene->mRootNode, &m_skMesh);
			}
		}

		return m_OBJModel.LoadModel(m_scene, fileName, Flags, mat, dim, bones);
	}
	
	void OBJInstance::setSize(Vector size)
	{
		m_size = size;
	}
	
	Vector OBJInstance::getSize()
	{
		return m_size;
	}
	
	void OBJInstance::setPosition(Vector pos)
	{
		m_pos = pos;
	}
	
	Vector OBJInstance::getPosition()
	{
		return m_pos;
	}
	
	void OBJInstance::setRotation(Vector rot)
	{
		m_rot = rot;
	}
	
	Vector OBJInstance::getRotation()
	{
		return m_rot;
	}
	
	void OBJInstance::Update(float deltaTime)
	{
		if (m_anims.size() > 0)
		{
			for (int i = 0; i < m_OBJModel.m_modelMeshes.size(); i++)
			{
				m_anims[0].BoneTransform(deltaTime, i, &m_skMesh);
			}
		}
	}

	void OBJInstance::Draw(RenderManager* renderManager, bool useTextures)
	{
		//renderManager->UpdateModelMatrix(getModelMatrix());
	
		m_OBJModel.Draw(renderManager, useTextures, &m_skMesh);
	}
	void OBJInstance::SetResources(RenderManager* renderManager, bool useTextures)
	{
		renderManager->UpdateModelMatrix(getModelMatrix());

		m_OBJModel.SetResources(renderManager, useTextures);
 	}
}