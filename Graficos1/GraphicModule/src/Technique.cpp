#include "Technique.h"
#include <fstream>
#include <sstream>

namespace GraphicsModule
{
	void Technique::CompileShader(const char* vertexShaderPath, const char* pixelShaderPath)
	{
		std::ifstream vsFile(vertexShaderPath);
		std::stringstream vsString;

/*#if defined(DX11)
		vsString << "#define DX11" << '\n';
#elif defined(OGL)
		vsString << "#version 330 core" << '\n';
		vsString << "#define OGL" << '\n';
#endif

		for (string& s : m_defines)
		{
			vsString << "#define " << s << '\n';
		}*/

		while (!vsFile.eof())
		{
			string s;
			std::getline(vsFile, s);
			vsString << s << '\n';
		}

		//std::cout << vsString.str() << endl;

		std::ifstream psFile(pixelShaderPath);
		std::stringstream psString;

/*#if defined(DX11)
		psString << "#define DX11" << '\n';
#elif defined(OGL)
		psString << "#version 330 core" << '\n';
		psString << "#define OGL" << '\n';
#endif

		for (string& s : m_defines)
		{
			psString << "#define " << s << '\n';
		}*/

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
		for (Pass& ps : m_passes)
		{
			ps.Unuse();
		}
	}
	void Technique::SetBool(const string name, bool value)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetBool(name, value);
		}
	}
	void Technique::SetInt(const string name, int value)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetInt(name, value);
		}
	}
	void Technique::SetFloat(const string name, float value)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetFloat(name, value);
		}
	}
	void Technique::SetUint(const string name, unsigned int value)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetUint(name, value);
		}
	}
	void Technique::SetBool2(const string name, bool value1, bool value2)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetBool2(name, value1, value2);
		}
	}
	void Technique::SetInt2(const string name, int value1, int value2)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetInt2(name, value1, value2);
		}
	}
	void Technique::SetFloat2(const string name, float value1, float value2)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetFloat2(name, value1, value2);
		}
	}
	void Technique::SetUint2(const string name, unsigned int value1, unsigned int value2)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetUint2(name, value1, value2);
		}
	}
	void Technique::SetBool3(const string name, bool value1, bool value2, bool value3)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetBool3(name, value1, value2, value3);
		}
	}
	void Technique::SetInt3(const string name, int value1, int value2, int value3)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetInt3(name, value1, value2, value3);
		}
	}
	void Technique::SetFloat3(const string name, float value1, float value2, float value3)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetFloat3(name, value1, value2, value3);
		}
	}
	void Technique::SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetUint3(name, value1, value2, value3);
		}
	}
	void Technique::SetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetBool4(name, value1, value2, value3, value4);
		}
	}
	void Technique::SetInt4(const string name, int value1, int value2, int value3, int value4)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetInt4(name, value1, value2, value3, value4);
		}
	}
	void Technique::SetFloat4(const string name, float value1, float value2, float value3, float value4)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetFloat4(name, value1, value2, value3, value4);
		}
	}
	void Technique::SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetUint4(name, value1, value2, value3, value4);
		}
	}
	void Technique::SetMat4(const string name, glm::mat4 value)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetMat4(name, value);
		}
	}
	void Technique::SetInputLayout(unsigned int VAO)
	{
		for (Pass& ps : m_passes)
		{
			ps.SetInputLayout(VAO);
		}
	}
#endif
}