#pragma once
#include "Descriptors.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace GraphicsModule
{
	struct VertexWeight
	{
		unsigned int m_vertexID = 0;
		float m_weight = 0;
	};

	struct Bone
	{
		Bone() = default;
		Bone(string name, vector<VertexWeight> vertexWeights, MATRIX offsetMatrix);
		~Bone();

		void AddBoneData(VertexWeight vw);
	
		string m_name;
		vector<VertexWeight> m_vertexWeights;
		MATRIX m_offsetMatrix;
	};

	struct UnaEstructuraQueSoloTraeLaMatrizParaQueElRivazSeCalleYMeAyude
	{
		MATRIX m_laMatriz;
	};
}
