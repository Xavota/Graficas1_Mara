#include "Bone.h"


namespace GraphicsModule
{
	Bone::Bone(string name, vector<VertexWeight> vertexWeights, MATRIX offsetMatrix)
	{
		m_name = name;
		m_vertexWeights = vertexWeights;
		m_offsetMatrix = offsetMatrix;
	}
	Bone::~Bone()
	{
	}
	void Bone::AddBoneData(VertexWeight vw)
	{
		m_vertexWeights.push_back(vw);
	}
}