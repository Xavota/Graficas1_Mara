#include "Effect.h"

namespace GraphicsModule
{
	void Effect::CompileShader(const char* vertexShaderPath, const char* pixelShaderPath)
	{
		
		for (int i = -1; i < (int)eNORMAL_TECHNIQUES::COUNT; i++)
		{
			for (int j = -1; j < (int)eSPECULAR_TECHNIQUES::COUNT; j++)
			{
				if (eNORMAL_TECHNIQUES(i) == eNORMAL_TECHNIQUES::VERTEX_SHADER)
				{
					m_techniques.push_back({ eNORMAL_TECHNIQUES(i), eSPECULAR_TECHNIQUES(j), 0, Technique() });
					m_techniques[m_techniques.size() - 1].tech.AddDefine("VERTEX_LIGHT");

					switch ((eSPECULAR_TECHNIQUES)j)
					{
					case eSPECULAR_TECHNIQUES::PHONG:
						m_techniques[m_techniques.size() - 1].tech.AddDefine("PHONG");
						break;
					case eSPECULAR_TECHNIQUES::BLINN_PHONG:
						m_techniques[m_techniques.size() - 1].tech.AddDefine("BLINN_PHONG");
						break;
					}

					m_techniques[m_techniques.size() - 1].tech.CompileShader(vertexShaderPath,pixelShaderPath);
				}
				else if (eNORMAL_TECHNIQUES(i) == eNORMAL_TECHNIQUES::PIXEL_SHADER)
				{
					for (unsigned int k = 0; k < TEXTURE_MAP_COUNT; k++)
					{
						m_techniques.push_back({ eNORMAL_TECHNIQUES(i), eSPECULAR_TECHNIQUES(j), k, Technique() });
						m_techniques[m_techniques.size() - 1].tech.AddDefine("PIXEL_LIGHT");

						switch ((eSPECULAR_TECHNIQUES)j)
						{
						case eSPECULAR_TECHNIQUES::PHONG:
							m_techniques[m_techniques.size() - 1].tech.AddDefine("PHONG");
							break;
						case eSPECULAR_TECHNIQUES::BLINN_PHONG:
							m_techniques[m_techniques.size() - 1].tech.AddDefine("BLINN_PHONG");
							break;
						}

						if (k & TEXTURE_MAP_NORMAL)
						{
							m_techniques[m_techniques.size() - 1].tech.AddDefine("TEXTURE_MAP_NORMAL");
						}
						if (k & TEXTURE_MAP_SPECULAR)
						{
							m_techniques[m_techniques.size() - 1].tech.AddDefine("TEXTURE_MAP_SPECULAR");
						}

						m_techniques[m_techniques.size() - 1].tech.CompileShader(vertexShaderPath, pixelShaderPath);
					}
				}
				else
				{
					m_techniques.push_back({ eNORMAL_TECHNIQUES(i), eSPECULAR_TECHNIQUES(j), 0, Technique() });

					switch ((eSPECULAR_TECHNIQUES)j)
					{
					case eSPECULAR_TECHNIQUES::PHONG:
						m_techniques[m_techniques.size() - 1].tech.AddDefine("PHONG");
						break;
					case eSPECULAR_TECHNIQUES::BLINN_PHONG:
						m_techniques[m_techniques.size() - 1].tech.AddDefine("BLINN_PHONG");
						break;
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
	}
	void Effect::SetBool(const string name, bool value)
	{
	}
	void Effect::SetInt(const string name, int value)
	{
	}
	void Effect::SetFloat(const string name, float value)
	{
	}
	void Effect::SetUint(const string name, unsigned int value)
	{
	}
	void Effect::SetBool2(const string name, bool value1, bool value2)
	{
	}
	void Effect::SetInt2(const string name, int value1, int value2)
	{
	}
	void Effect::SetFloat2(const string name, float value1, float value2)
	{
	}
	void Effect::SetUint2(const string name, unsigned int value1, unsigned int value2)
	{
	}
	void Effect::SetBool3(const string name, bool value1, bool value2, bool value3)
	{
	}
	void Effect::SetInt3(const string name, int value1, int value2, int value3)
	{
	}
	void Effect::SetFloat3(const string name, float value1, float value2, float value3)
	{
	}
	void Effect::SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
	{
	}
	void Effect::SetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
	{
	}
	void Effect::SetInt4(const string name, int value1, int value2, int value3, int value4)
	{
	}
	void Effect::SetFloat4(const string name, float value1, float value2, float value3, float value4)
	{
	}
	void Effect::SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
	{
	}
	void Effect::SetMat4(const string name, glm::mat4 value)
	{
	}
	void Effect::SetInputLayout(unsigned int VAO)
	{
	}
#endif
}