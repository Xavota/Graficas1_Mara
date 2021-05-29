#include "Effect.h"

namespace GraphicsModule
{
	void Effect::CompileShader(const char* vertexShaderPath, const char* pixelShaderPath)
	{
		
		for (int i = -1; i < (int)eNORMAL_TECHNIQUES::COUNT; i++)
		{
			for (int j = -1; j < (int)eSPECULAR_TECHNIQUES::COUNT; j++)
			{
				for (unsigned int k = 0; k <= TEXTURE_MAP_COUNT; k++)
				{
					m_techniques.push_back({ eNORMAL_TECHNIQUES(i), eSPECULAR_TECHNIQUES(j), k, Technique() });
					
					switch ((eNORMAL_TECHNIQUES)i)
					{
					case eNORMAL_TECHNIQUES::PIXEL_SHADER:
						m_techniques[m_techniques.size() - 1].tech.AddDefine("PIXEL_LIGHT");
						break;
					case eNORMAL_TECHNIQUES::VERTEX_SHADER:
						m_techniques[m_techniques.size() - 1].tech.AddDefine("VERTEX_LIGHT");
						break;

					}
					

					switch ((eSPECULAR_TECHNIQUES)j)
					{
					case eSPECULAR_TECHNIQUES::PHONG:
						m_techniques[m_techniques.size() - 1].tech.AddDefine("PHONG");
						break;
					case eSPECULAR_TECHNIQUES::BLINN_PHONG:
						m_techniques[m_techniques.size() - 1].tech.AddDefine("BLINN_PHONG");
						break;
					}

					if (k & TEXTURE_MAP_DIFFUSE)
					{
						m_techniques[m_techniques.size() - 1].tech.AddDefine("DIFFUSE_MAP");
					}
					if (k & TEXTURE_MAP_NORMAL)
					{
						m_techniques[m_techniques.size() - 1].tech.AddDefine("NORMAL_MAP");
					}
					if (k & TEXTURE_MAP_SPECULAR)
					{
						m_techniques[m_techniques.size() - 1].tech.AddDefine("SPECULAR_MAP");
					}

					m_techniques[m_techniques.size() - 1].tech.CompileShader(vertexShaderPath, pixelShaderPath);
				}
			}
		}
		
	}

	void Effect::SetShaderFlags(eNORMAL_TECHNIQUES nor, eSPECULAR_TECHNIQUES spec, unsigned int texFlags)
	{
		for (Techs& t : m_techniques)
		{
			if (t.normalTech == nor && t.specularTech == spec && t.texFlagsTech == texFlags)
			{
				m_currentTechnique = &t.tech;
			}
		}
	}

	void Effect::Use()
	{
		m_currentTechnique->Use();
	}
#if defined(DX11)
	void Effect::SetBuffer(int slot, Buffer buff, void* data)
	{
		for (Techs& t : m_techniques)
		{
			t.tech.SetBuffer(slot, buff, data);
		}
	}

#elif defined(OGL)
	void Effect::Unuse()
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.Unuse();
		}
	}
	void Effect::SetBool(const string name, bool value)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetBool(name, value);
		}
	}
	void Effect::SetInt(const string name, int value)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetInt(name, value);
		}
	}
	void Effect::SetFloat(const string name, float value)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetFloat(name, value);
		}
	}
	void Effect::SetUint(const string name, unsigned int value)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetUint(name, value);
		}
	}
	void Effect::SetBool2(const string name, bool value1, bool value2)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetBool2(name, value1, value2);
		}
	}
	void Effect::SetInt2(const string name, int value1, int value2)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetInt2(name, value1, value2);
		}
	}
	void Effect::SetFloat2(const string name, float value1, float value2)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetFloat2(name, value1, value2);
		}
	}
	void Effect::SetUint2(const string name, unsigned int value1, unsigned int value2)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetUint2(name, value1, value2);
		}
	}
	void Effect::SetBool3(const string name, bool value1, bool value2, bool value3)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetBool3(name, value1, value2, value3);
		}
	}
	void Effect::SetInt3(const string name, int value1, int value2, int value3)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetInt3(name, value1, value2, value3);
		}
	}
	void Effect::SetFloat3(const string name, float value1, float value2, float value3)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetFloat3(name, value1, value2, value3);
		}
	}
	void Effect::SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetUint3(name, value1, value2, value3);
		}
	}
	void Effect::SetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetBool4(name, value1, value2, value3, value4);
		}
	}
	void Effect::SetInt4(const string name, int value1, int value2, int value3, int value4)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetInt4(name, value1, value2, value3, value4);
		}
	}
	void Effect::SetFloat4(const string name, float value1, float value2, float value3, float value4)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetFloat4(name, value1, value2, value3, value4);
		}
	}
	void Effect::SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetUint4(name, value1, value2, value3, value4);
		}
	}
	void Effect::SetMat4(const string name, glm::mat4 value)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetMat4(name, value);
		}
	}
	void Effect::SetInputLayout(unsigned int VAO)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetInputLayout(VAO);
		}
	}
#endif
}