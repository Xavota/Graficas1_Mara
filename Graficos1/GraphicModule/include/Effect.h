#pragma once
#include "Technique.h"
#include <vector>

namespace GraphicsModule
{
class Effect
{
public:
	Effect() = default;
	~Effect() = default;
private:
	std::vector<Technique> m_technics;
};
}
