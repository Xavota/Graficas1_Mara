#pragma once
#include "Descriptors.h"

#include <string>
#include <vector>

#include <assimp/scene.h>

namespace GraphicsModule
{
	struct VectorKeyFrame
	{
		double m_time;

		Vector3 m_value;
	};

	struct QuatKeyFrame
	{
		double m_time;

		aiQuaternion m_value;
	};

	struct AnimNode
	{
		std::string m_name;

		unsigned int m_positionKeysCount;
		unsigned int m_rotationKeysCount;
		unsigned int m_scalingKeysCount;

		std::vector<VectorKeyFrame> m_positionKeys;
		std::vector<QuatKeyFrame> m_rotationKeys;
		std::vector<VectorKeyFrame> m_scalingKeys;
	};
}
