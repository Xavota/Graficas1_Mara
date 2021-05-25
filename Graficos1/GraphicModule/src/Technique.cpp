#include "Technique.h"
#include <fstream>
#include <sstream>

namespace GraphicsModule
{
	void Technique::CompileShader(const char* vertexShaderPath, const char* pixelShaderPath)
	{
		std::ifstream vsFile(vertexShaderPath);
		std::stringstream vsString;
		for (string& s : m_defines)
		{
			vsString << "#define " << s << '\n';
		}

		while (!vsFile.eof())
		{
			string s;
			std::getline(vsFile, s);
			vsString << s << '\n';
		}

		std::cout << vsString.str() << endl;

		std::ifstream psFile(pixelShaderPath);
		std::stringstream psString;
		for (string& s : m_defines)
		{
			psString << "#define " << s << '\n';
		}

		while (!psFile.eof())
		{
			string s;
			std::getline(psFile, s);
			psString << s << '\n';
		}


		m_passes.push_back(Pass());
		m_passes[m_passes.size() - 1].Compile(vsString.str().c_str(), psString.str().c_str());
	}

	void Technique::AddDefine(std::string def)
	{
		m_defines.push_back(def);	
	}

	void Technique::Use()
	{
		for (Pass& p : m_passes)
		{
			p.Use();
		}
	}
#if defined(DX11)
	void Technique::SetBuffer(int slot, Buffer buff, void* data)
	{
		for (Pass& p : m_passes)
		{
			p.SetBuffer(slot, buff, data);
		}
	}
#elif defined(OGL)
	void Technique::Unuse()
	{
	}
	void Technique::SetBool(const string name, bool value)
	{
	}
	void Technique::SetInt(const string name, int value)
	{
	}
	void Technique::SetFloat(const string name, float value)
	{
	}
	void Technique::SetUint(const string name, unsigned int value)
	{
	}
	void Technique::SetBool2(const string name, bool value1, bool value2)
	{
	}
	void Technique::SetInt2(const string name, int value1, int value2)
	{
	}
	void Technique::SetFloat2(const string name, float value1, float value2)
	{
	}
	void Technique::SetUint2(const string name, unsigned int value1, unsigned int value2)
	{
	}
	void Technique::SetBool3(const string name, bool value1, bool value2, bool value3)
	{
	}
	void Technique::SetInt3(const string name, int value1, int value2, int value3)
	{
	}
	void Technique::SetFloat3(const string name, float value1, float value2, float value3)
	{
	}
	void Technique::SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
	{
	}
	void Technique::SetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
	{
	}
	void Technique::SetInt4(const string name, int value1, int value2, int value3, int value4)
	{
	}
	void Technique::SetFloat4(const string name, float value1, float value2, float value3, float value4)
	{
	}
	void Technique::SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
	{
	}
	void Technique::SetMat4(const string name, glm::mat4 value)
	{
	}
	void Technique::SetInputLayout(unsigned int VAO)
	{
	}
#endif
}