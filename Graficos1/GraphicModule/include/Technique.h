#pragma once
#include <vector>
#include "Pass.h"
class Technique
{
public:
	Technique() = default;
	~Technique() = default;
private:
	std::vector<Pass> m_passes;
};

