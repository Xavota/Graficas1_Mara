#include "Effect.h"

namespace GraphicsModule
{
	/*void Effect::CompileShader(const char* vertexShaderPath, const char* pixelShaderPath)
	{	
		
	}*/

	void Effect::GenerateEffects()
	{
		/*for (int i = -1; i < (int)eNORMAL_TECHNIQUES::COUNT; i++)
		{
			if (eNORMAL_TECHNIQUES(i) == eNORMAL_TECHNIQUES::PIXEL_SHADER)
			{
				for (int j = -1; j < (int)eSPECULAR_TECHNIQUES::COUNT; j++)
				{
					for (unsigned int k = 0; k <= TEXTURE_MAP_COUNT; k++)
					{
						for (int l = -1; l < (int)eTONE_CORRECTION_TECHNIQUES::COUNT; l++)
						{
							m_techniques.push_back({ eNORMAL_TECHNIQUES(i), eSPECULAR_TECHNIQUES(j), k, eTONE_CORRECTION_TECHNIQUES(l), Technique() });

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


							switch ((eTONE_CORRECTION_TECHNIQUES)l)
							{
							case eTONE_CORRECTION_TECHNIQUES::BASIC:
								m_techniques[m_techniques.size() - 1].tech.AddDefine("BASIC");
								break;
							case eTONE_CORRECTION_TECHNIQUES::REINHARD:
								m_techniques[m_techniques.size() - 1].tech.AddDefine("REINHARD");
								break;
							case eTONE_CORRECTION_TECHNIQUES::BURGESS_DAWSON:
								m_techniques[m_techniques.size() - 1].tech.AddDefine("BURGESS_DAWSON");
								break;
							case eTONE_CORRECTION_TECHNIQUES::UNCHARTED2:
								m_techniques[m_techniques.size() - 1].tech.AddDefine("UNCHARTED2");
								break;
							}
						}
					}
				}
			}

		}/**/

		m_techniques.push_back({ eNORMAL_TECHNIQUES::NONE, eSPECULAR_TECHNIQUES::NONE, 0, eTONE_CORRECTION_TECHNIQUES::NONE, Technique() });

		m_currentTechnique = &m_techniques[0].tech;
	}

	void Effect::CreatePass(string name, const char* vertexShaderPath, const char* pixelShaderPath, CULL_MODE cull)
	{
		if (m_techniques.size() == 0)
			GenerateEffects();
		
		for (Techs& t : m_techniques)
		{
			t.tech.CreatePass(name, vertexShaderPath, pixelShaderPath, cull);
		}
		
	}	

	void Effect::SetShaderFlags(eNORMAL_TECHNIQUES nor, eSPECULAR_TECHNIQUES spec, unsigned int texFlags, eTONE_CORRECTION_TECHNIQUES toneMap/**/)
	{
		for (Techs& t : m_techniques)
		{
			if (t.normalTech == nor && t.specularTech == spec && t.texFlagsTech == texFlags && t.m_toneMap == toneMap/**/)
			{
				m_currentTechnique = &t.tech;
				return;
			}
		}
	}

	void Effect::Use()
	{
	}
	//void Effect::Draw(unsigned int indexCount)
	void Effect::Draw()
	{
		//m_currentTechnique->Use(indexCount);
		m_currentTechnique->Use();
	}
#if defined(DX11)
	/*void Effect::SetBuffer(int slot, Buffer buff, void* data)
	{
		for (Techs& t : m_techniques)
		{
			t.tech.SetBuffer(slot, buff, data);
		}
	}*/

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

#if defined(DX11)
	void Effect::AddEffectTrackValue(string name, unsigned int id, unsigned int size)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.AddTrackValue(name, id, size);
		}
	}
	void Effect::AddPassTrackValue(string passName, string name, unsigned int id, unsigned int size)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.AddPassTrackValue(passName, name, id, size);
		}
	}
	void Effect::AddPassInputTexture(string passName, string textureName)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.AddPassInputTexture(passName, textureName);
		}
	}
#elif defined(OGL)
	void Effect::AddEffectTrackValue(string name, string uniform, eDataType type)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.AddTrackValue(name, uniform, type);
		}
	}
	void Effect::AddPassTrackValue(string passName, string name, string uniform, eDataType type)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.AddPassTrackValue(passName, name, uniform, type);
		}
	}
	void Effect::AddPassInputTexture(string passName, string textureName, string uniform)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.AddPassInputTexture(passName, textureName, uniform);
		}
	}
#endif
	void Effect::SetEffectValue(string name, void* data)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetValue(name, data);
		}
	}
	void Effect::SetPassValue(string passName, string name, void* data)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetPassValue(passName, name, data);
		}
	}
	void Effect::AddObjectToPass(string passName, OBJInstance* obj, bool useTextures)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.AddObjectToPass(passName, obj, useTextures);
		}
	}
	void Effect::UniteInputOutputTextures(string outputPassName, string outpuTextureName, string inputPassName, string inputTextureName)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.UniteInputOutputTextures(outputPassName, outpuTextureName, inputPassName, inputTextureName);
		}
	}
	void Effect::UniteOutputOutputTextures(string outputPassName, string outpuTextureName, string newOutputPassName, string newOutputTextureName)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.UniteOutputOutputTextures(outputPassName, outpuTextureName, newOutputPassName, newOutputTextureName);
		}
	}
	void Effect::SetPassInputTexture(string passName, string textureName, Texture tex)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetPassInputTexture(passName, textureName, tex);
		}
	}
	void Effect::AddPassOutputTexture(string passName, string textureName, bool cleanRenderTarget, float clearColor[4])
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.AddPassOutputTexture(passName, textureName, cleanRenderTarget, clearColor);
		}
	}
	void Effect::SetPassOutputTexture(string passName, string textureName, RenderTargetView* tex, DepthStencilView dsv)
	{
		for (Techs& ps : m_techniques)
		{
			ps.tech.SetPassOutputTexture(passName, textureName, tex, dsv);
		}
	}
}