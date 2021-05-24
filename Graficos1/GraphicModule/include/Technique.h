#pragma once
#include <vector>
#include <string>
#include "Pass.h"

namespace GraphicsModule
{
class Technique
{
public:
	Technique() = default;
	~Technique() = default;	

	void AddDefines(std::vector<std::string> defs);

private:
	std::vector<Pass> m_passes;
	
};
}
