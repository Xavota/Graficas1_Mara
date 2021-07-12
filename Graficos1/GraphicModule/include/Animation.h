#pragma once
#include <string>
#include <vector>
#include "Descriptors.h"

#include "SkeletalMesh.h"
#include "AnimNode.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GraphicsModule
{

	struct Node
	{
		std::string m_name;

		MATRIX m_transformation;

		Node* m_parent = nullptr;

		unsigned int m_childrenCount;

		std::vector<Node*> m_children;
	};

	class Animation
	{
	public:
		Animation() = default;
		~Animation() = default;

		void LoadAnimation(aiAnimation* anim, aiNode* root, SkeletalMesh* skMesh);
		void StoreNodes(aiNode* current, Node* storage);

		void BoneTransform(float TimeInSeconds, int meshIndex, SkeletalMesh* skMesh);
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const MATRIX& ParentTransform, int meshIndex);

		aiNodeAnim* FindNodeAnim(aiAnimation* anim, std::string name);

		void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

		unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);/**/


		/*void ReadNodeHeirarchy(float AnimationTime, const Node* pNode, const MATRIX& ParentTransform, int meshIndex);

		AnimNode* FindNodeAnim(std::string name);

		void CalcInterpolatedScaling(Vector3& Out, float AnimationTime, const AnimNode* pNodeAnim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const AnimNode* pNodeAnim);
		void CalcInterpolatedPosition(Vector3& Out, float AnimationTime, const AnimNode* pNodeAnim);

		unsigned int FindScaling(float AnimationTime, const AnimNode* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const AnimNode* pNodeAnim);
		unsigned int FindPosition(float AnimationTime, const AnimNode* pNodeAnim);/**/

		const char* getName();

	private:
		SkeletalMesh* m_skMesh = nullptr;

		float m_totalTime = 0.0f;
		float m_ticksPerSecond = 0.0f;
		float m_duration = 0.0f;
		std::string m_name;

		aiAnimation* m_anim = nullptr;
		aiNode* m_root = nullptr;

		std::vector<AnimNode> m_channels;
		Node* m_rootNode = nullptr;
	};
}
