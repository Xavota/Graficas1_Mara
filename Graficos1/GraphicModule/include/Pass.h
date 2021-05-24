#pragma once
#include "Shader.h"
#include "OBJInstance.h"

#include <map>

namespace GraphicsModule
{
class Pass
{
public:
	Pass() = default;
	~Pass() = default;

	void DrawPass();

private:
	Shader m_shaders;
	std::map<int, void*> m_values;
	OBJInstance* m_model;
};
}
