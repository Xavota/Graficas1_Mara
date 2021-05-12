#pragma once
#include "Technique.h"
#include <vector>
class Effect
{
public:
	Effect() = default;
	~Effect() = default;
private:
	std::vector<Technique> m_technics;
};

