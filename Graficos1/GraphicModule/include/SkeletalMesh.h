#pragma once
#include "Descriptors.h"
#include "Bone.h"
#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GraphicsModule
{
	class OBJInstance;

	class SkeletalMesh
	{
	public:
		SkeletalMesh();
		~SkeletalMesh();
	
		std::vector<std::vector<Bone>> LoadSkeletalMesh(const aiScene* scene);

		std::vector<Bone> LoadSkeletalMesh(aiNode* root, std::vector<std::vector<Bone>> bones);
		bool LoadSkeletalByNode(aiNode* node, std::vector<std::vector<Bone>> bones);

		void BoneTransform(const aiNode* root, int meshIndex);
		void ReadNodeHeirarchy(const aiNode* pNode, const MATRIX& ParentTransform, int meshIndex);

		std::vector<MATRIX> GetBonesMatrices(int meshNum);

		OBJInstance* getSkeletonMesh() { return m_skeleton; }
	
	private:
		friend class Animation;

		std::vector<std::vector<Bone>> m_bonesPerMesh;
	
		vector<MATRIX> m_globalInverseTransforms;
		std::vector<std::map<std::string, int>> m_boneMappings;
		std::vector<int> m_numsBones;

		OBJInstance* m_skeleton = nullptr;
	};
}
