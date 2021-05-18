#pragma once
#include "Shader.h"

#include <map>

class Pass
{
public:
	Pass() = default;
	~Pass() = default;
private:
	Shader m_shaders;
	std::map<int, void*> m_values;
};

