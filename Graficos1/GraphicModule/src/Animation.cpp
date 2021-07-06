#include "Animation.h"


namespace GraphicsModule
{
	void Animation::LoadAnimation(aiAnimation* anim, aiNode* root, SkeletalMesh* skMesh)
	{
		m_skMesh = skMesh;

		m_anim = new aiAnimation();
		memcpy(m_anim, anim, sizeof(aiAnimation));
		m_root = root;

		m_ticksPerSecond = m_anim->mTicksPerSecond != 0 ? m_anim->mTicksPerSecond : 25.0f;
		m_name = m_anim->mName.C_Str();

		m_duration = anim->mDuration;

		for (int i = 0; i < anim->mNumChannels; i++)
		{
			m_channels.push_back(AnimNode());

			m_channels[i].m_name = anim->mChannels[i]->mNodeName.C_Str();
			m_channels[i].m_positionKeysCount = anim->mChannels[i]->mNumPositionKeys;
			m_channels[i].m_rotationKeysCount = anim->mChannels[i]->mNumRotationKeys;
			m_channels[i].m_scalingKeysCount  = anim->mChannels[i]->mNumScalingKeys;

			for (int j = 0; j < m_channels[i].m_positionKeysCount; j++)
			{
				m_channels[i].m_positionKeys.push_back(VectorKeyFrame()); 
				m_channels[i].m_positionKeys[j].m_time = anim->mChannels[i]->mPositionKeys[j].mTime;
				aiVector3D val = anim->mChannels[i]->mPositionKeys[j].mValue;
				m_channels[i].m_positionKeys[j].m_value = Vector3{val.x, val.y, val.x};
			}

			for (int j = 0; j < m_channels[i].m_rotationKeysCount; j++)
			{
				m_channels[i].m_rotationKeys.push_back(QuatKeyFrame());
				m_channels[i].m_rotationKeys[j].m_time = anim->mChannels[i]->mRotationKeys[j].mTime;
				aiQuaternion val = anim->mChannels[i]->mRotationKeys[j].mValue;
				m_channels[i].m_rotationKeys[j].m_value = val;
			}

			for (int j = 0; j < m_channels[i].m_scalingKeysCount; j++)
			{
				m_channels[i].m_scalingKeys.push_back(VectorKeyFrame());
				m_channels[i].m_scalingKeys[j].m_time = anim->mChannels[i]->mScalingKeys[j].mTime;
				aiVector3D val = anim->mChannels[i]->mScalingKeys[j].mValue;
				m_channels[i].m_scalingKeys[j].m_value = Vector3{ val.x, val.y, val.x };
			}
		}

		m_rootNode = new Node();
		StoreNodes(root, m_rootNode);
	}
	void Animation::StoreNodes(aiNode* current, Node* storage)
	{
		storage->m_name = current->mName.C_Str();
		storage->m_transformation = MATRIX(&current->mTransformation.a1);

		storage->m_childrenCount = current->mNumChildren;

		for (int i = 0; i < storage->m_childrenCount; i++)
		{
			storage->m_children.push_back(new Node());
			StoreNodes(current->mChildren[i], storage->m_children[i]);
			storage->m_children[i]->m_parent = storage;
		}
	}
	void Animation::BoneTransform(float TimeInSeconds, int meshIndex)
	{
		MATRIX Identity;

		float TimeInTicks = TimeInSeconds * m_ticksPerSecond;
		float AnimationTime = fmod(TimeInTicks, m_duration);

		ReadNodeHeirarchy(AnimationTime, m_root, Identity, meshIndex);
	}
	void Animation::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const MATRIX& ParentTransform, int meshIndex)
	{
		std::string NodeName(pNode->mName.data);

		MATRIX NodeTransformation(&pNode->mTransformation.a1);

		const aiNodeAnim* pNodeAnim = FindNodeAnim(m_anim, NodeName);

		if (pNodeAnim) {
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			MATRIX ScalingM = MATRIX::Scale(Scaling.x, Scaling.y, Scaling.z);

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
			aiMatrix3x3 quat = RotationQ.GetMatrix();
			MATRIX RotationM = MATRIX(quat.a1, quat.a2, quat.a3, 0,
									  quat.b1, quat.b2, quat.b3, 0, 
									  quat.c1, quat.c2, quat.c3, 0, 
									  0,       0,       0,       1 );

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			MATRIX TranslationM = MATRIX::Translate(Translation.x, Translation.y, Translation.z);

			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM;
		}

		MATRIX GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_skMesh->m_boneMappings[meshIndex].find(NodeName) != m_skMesh->m_boneMappings[meshIndex].end()) {
			unsigned int BoneIndex = m_skMesh->m_boneMappings[meshIndex][NodeName];
			m_skMesh->m_bonesPerMesh[meshIndex][BoneIndex].m_finalTransformation = m_skMesh->m_globalInverseTransforms[meshIndex] * GlobalTransformation 
																				 * m_skMesh->m_bonesPerMesh[meshIndex][BoneIndex].m_offsetMatrix;
		}

		for (int i = 0; i < pNode->mNumChildren; i++) {
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation, meshIndex);
		}
	}
	aiNodeAnim* Animation::FindNodeAnim(aiAnimation* anim, std::string name)
	{
		for (int i = 0; i < anim->mNumChannels; i++)
		{
			if (anim->mChannels[i]->mNodeName.C_Str() == name)
			{
				return anim->mChannels[i];
			}
		}
		return nullptr;
	}
	void Animation::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumScalingKeys == 1) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		//assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime;
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& StartScaling = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& EndScaling = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		Out = EndScaling - StartScaling;
		Out = StartScaling + Out * Factor;
	}
	void Animation::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		//assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime;
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}
	void Animation::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumPositionKeys == 1) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		//assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime;
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& StartPosition = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& EndPosition = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		Out = EndPosition - StartPosition;
		Out = StartPosition + Out * Factor;
	}
	unsigned int Animation::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		//assert(pNodeAnim->mNumScalingKeys > 0);

		for (int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}

		//assert(0);
		return 0;
	}
	unsigned int Animation::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		//assert(pNodeAnim->mNumRotationKeys > 0);

		for (int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}

		//assert(0);
		return 0;
	}
	unsigned int Animation::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		//assert(pNodeAnim->mNumPositionKeys > 0);

		for (int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}

		//assert(0);
		return 0;
	}
}