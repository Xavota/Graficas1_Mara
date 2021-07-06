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
	class SkeletalMesh
	{
	public:
		SkeletalMesh();
		~SkeletalMesh();
	
		std::vector<std::vector<Bone>> LoadSkeletalMesh(const aiScene* scene);

		std::vector<MATRIX> GetBonesMatrices(int meshNum);
	
	private:
		friend class Animation;

		std::vector<std::vector<Bone>> m_bonesPerMesh;
	
		vector<Bone*> m_bones;
		vector<MATRIX> m_globalInverseTransforms;
		std::vector<std::map<std::string, int>> m_boneMappings;
		std::vector<int> m_numsBones;
	};
}
