#include "SkeletalMesh.h"
#include <iostream>
#include <string>

namespace GraphicsModule
{
	SkeletalMesh::SkeletalMesh()
	{
	}
	
	SkeletalMesh::~SkeletalMesh()
	{
	}

	void ReadNodes(aiNode* node, int offset)
	{
		if (node != nullptr)
		{
			for (int i = 0; i < offset; i++)
			{
				std::cout << "|  ";
			}
			std::cout << node->mName.C_Str() << std::endl;
		}
		else
			return;

		for (int i = 0; i < node->mNumChildren; i++)
		{
			ReadNodes(node->mChildren[i], offset + 1);
		}
	}

	void ReadAnimNodes(aiAnimation* anim)
	{
		for (int i = 0; i < anim->mNumChannels; i++)
		{
			std::cout << anim->mChannels[i]->mNodeName.C_Str() << std::endl;
		}
	}
	
	std::vector<std::vector<Bone>> SkeletalMesh::LoadSkeletalMesh(const aiScene* scene)
	{

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//m_bones = new Bone[100];

			if (scene->mMeshes[i]->HasBones())
			{
				m_numsBones.push_back(0);
				
				aiMatrix4x4 globalTransform = scene->mRootNode->mTransformation.Inverse();
				m_globalInverseTransforms.push_back(MATRIX(&globalTransform.a1));

				m_bonesPerMesh.push_back(std::vector<Bone>(100));
				m_boneMappings.push_back(std::map<std::string, int>());
				//m_estructurasConSoloMatricesQueSonBásicamenteSoloMatrices.push_back(std::vector<UnaEstructuraQueSoloTraeLaMatrizParaQueElRivazSeCalleYMeAyude>());

				for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
				{
					unsigned int boneIndex = 0;
					std::string name = scene->mMeshes[i]->mBones[j]->mName.C_Str();

					if (m_boneMappings[i].find(name) == m_boneMappings[i].end())
					{
						boneIndex = m_numsBones[i];
						m_numsBones[i]++;
					}
					else
					{
						boneIndex = m_boneMappings[i][name];
					}

					m_boneMappings[i][name] = boneIndex;

					m_bonesPerMesh[i][boneIndex].m_offsetMatrix = MATRIX(&scene->mMeshes[i]->mBones[boneIndex]->mOffsetMatrix.a1);

					for (int k = 0; k < scene->mMeshes[i]->mBones[j]->mNumWeights; k++)
					{
						unsigned int vertexID = scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId;
						float weight = scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;

						m_bonesPerMesh[i][boneIndex ].AddBoneData({ vertexID, weight});

						//vertexWeights.push_back(VertexWeight{ scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId, scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight });
					}


					//std::vector<VertexWeight> vertexWeights;
					//m_bonesPerMesh[i].push_back(Bone(name, vertexWeights, offsetMatrix));

					//m_estructurasConSoloMatricesQueSonBásicamenteSoloMatrices[i].push_back({offsetMatrix});
				}
			}

				/*m_bonesPerMesh.push_back(std::vector<Bone>());
				m_boneMappings.push_back(std::map<std::string, int>());
				m_numsBones.push_back(0);
				for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
					int BoneIndex = 0;
					string BoneName(scene->mMeshes[i]->mBones[j]->mName.data);

					if (m_boneMappings[i].find(BoneName) == m_boneMappings[i].end()) {
						BoneIndex = m_numsBones[i];
						m_numsBones[i]++;
						Bone bi;
						m_bonesPerMesh[i].push_back(bi);
					}
					else {
						BoneIndex = m_boneMappings[i][BoneName];
					}

					m_boneMappings[i][BoneName] = BoneIndex;
					m_bonesPerMesh[i][BoneIndex].m_offsetMatrix = MATRIX(&scene->mMeshes[i]->mBones[j]->mOffsetMatrix.a1);

					for (int k = 0; k < scene->mMeshes[i]->mBones[j]->mNumWeights; k++) {
						int VertexID = m_Entries[MeshIndex].BaseVertex + scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId;
						float Weight = scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
						m_bonesAddBoneData(BoneIndex, Weight);
					}
				}*/
		}

		

		if (scene->mRootNode != nullptr)
		{
			std::cout << "* Nodes: " << std::endl << std::endl;
			ReadNodes(scene->mRootNode, 0);
			std::cout << std::endl << std::endl;
		}
		if (scene->HasAnimations())
		{
			std::cout << "* AnimNodes: " << std::endl << std::endl;
			ReadAnimNodes(scene->mAnimations[0]);
			std::cout << std::endl << std::endl;
		}

		return m_bonesPerMesh;
	}

	std::vector<MATRIX> SkeletalMesh::GetBonesMatrices(int meshNum)
	{
		std::vector<MATRIX> bonesMatrices;
		if (m_bonesPerMesh.size() > meshNum)
		{
			for (int i = 0; i < m_bonesPerMesh[meshNum].size(); i++)
			{
				bonesMatrices.push_back(m_bonesPerMesh[meshNum][i].m_offsetMatrix);
			}
		}

		return bonesMatrices;
	}
}