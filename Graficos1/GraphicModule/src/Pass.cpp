#include "Pass.h"
#include "RenderManager.h"

namespace GraphicsModule
{
	void Pass::DrawPass()
	{
		
	}
	void Pass::Compile(const char* vertexShaderString, const char* pixelShaderString)
	{
		m_shaders.CompileFromString(vertexShaderString, pixelShaderString);
	}
	void Pass::Use()
	{
		m_shaders.Use();
		for (Values& v : m_values)
		{
#if defined(DX11)
			m_shaders.SetBuffer(v.m_id, v.m_buff, v.m_data);
#elif defined(OGL)
			switch (v.m_type)
			{
			case eDataType::BOOL:
				m_shaders.SetBool(v.m_uniform, *(bool*)v.m_data);
				break;
			case eDataType::INT:
				m_shaders.SetBool(v.m_uniform, *(int*)v.m_data);
				break;
			case eDataType::FLOAT:
				m_shaders.SetBool(v.m_uniform, *(float*)v.m_data);
				break;
			case eDataType::UINT:
				m_shaders.SetBool(v.m_uniform, *(unsigned int*)v.m_data);
				break;
			case eDataType::BOOL2:
			{
				bool* data = (bool*)v.m_data;
				m_shaders.SetBool2(v.m_uniform, data[0], data[1]);
				break;
			}
			case eDataType::INT2:
			{
				int* data = (int*)v.m_data;
				m_shaders.SetInt2(v.m_uniform, data[0], data[1]);
				break;
			}
			case eDataType::FLOAT2:
			{
				float* data = (float*)v.m_data;
				m_shaders.SetFloat2(v.m_uniform, data[0], data[1]);
				break;
			}
			case eDataType::UNIT2:
			{
				unsigned int* data = (unsigned int*)v.m_data;
				m_shaders.SetUint2(v.m_uniform, data[0], data[1]);
				break;
			}
			case eDataType::BOOL3:
			{
				bool* data = (bool*)v.m_data;
				m_shaders.SetBool3(v.m_uniform, data[0], data[1], data[2]);
				break;
			}
			case eDataType::INT3:
			{
				int* data = (int*)v.m_data;
				m_shaders.SetInt3(v.m_uniform, data[0], data[1], data[2]);
				break;
			}
			case eDataType::FLOAT3:
			{
				float* data = (float*)v.m_data;
				m_shaders.SetFloat3(v.m_uniform, data[0], data[1], data[2]);
				break;
			}
			case eDataType::UINT3:
			{
				unsigned int* data = (unsigned int*)v.m_data;
				m_shaders.SetUint3(v.m_uniform, data[0], data[1], data[2]);
				break;
			}
			case eDataType::BOOL4:
			{
				bool* data = (bool*)v.m_data;
				m_shaders.SetBool4(v.m_uniform, data[0], data[1], data[2], data[3]);
				break;
			}
			case eDataType::INT4:
			{
				int* data = (int*)v.m_data;
				m_shaders.SetInt4(v.m_uniform, data[0], data[1], data[2], data[3]);
				break;
			}
			case eDataType::FLOAT4:
			{
				float* data = (float*)v.m_data;
				m_shaders.SetFloat4(v.m_uniform, data[0], data[1], data[2], data[3]);
				break;
			}
			case eDataType::UINT4:
			{
				unsigned int* data = (unsigned int*)v.m_data;
				m_shaders.SetUint4(v.m_uniform, data[0], data[1], data[2], data[3]);
				break;
			}
			}
#endif
		}
		/*std::vector<Texture> texs;
		for (map<string, Texture>::iterator tex = m_inputTextures.begin(); tex != m_inputTextures.end(); tex++)
		{
			texs.push_back(tex->second);
		}
		GetManager()->PSSetShaderResources()*/
	}
	void Pass::Draw(unsigned int indexCount)
	{
		GetManager()->DrawIndexed(indexCount, 0, 0);
	}
#if defined(DX11)
	void Pass::SetBuffer(int slot, Buffer buff, void* data)
	{
		m_shaders.SetBuffer(slot, buff, data);
	}
#elif defined(OGL)
	void Pass::Unuse()
	{
		m_shaders.Unuse();
	}
	void Pass::SetBool(const string name, bool value)
	{
		m_shaders.SetBool(name, value);
	}
	void Pass::SetInt(const string name, int value)
	{
		m_shaders.SetInt(name, value);
	}
	void Pass::SetFloat(const string name, float value)
	{
		m_shaders.SetFloat(name, value);
	}
	void Pass::SetUint(const string name, unsigned int value)
	{
		m_shaders.SetUint(name, value);
	}
	void Pass::SetBool2(const string name, bool value1, bool value2)
	{
		m_shaders.SetBool2(name, value1, value2);
	}
	void Pass::SetInt2(const string name, int value1, int value2)
	{
		m_shaders.SetInt2(name, value1, value2);
	}
	void Pass::SetFloat2(const string name, float value1, float value2)
	{
		m_shaders.SetFloat2(name, value1, value2);
	}
	void Pass::SetUint2(const string name, unsigned int value1, unsigned int value2)
	{
		m_shaders.SetUint2(name, value1, value2);
	}
	void Pass::SetBool3(const string name, bool value1, bool value2, bool value3)
	{
		m_shaders.SetBool3(name, value1, value2, value3);
	}
	void Pass::SetInt3(const string name, int value1, int value2, int value3)
	{
		m_shaders.SetInt3(name, value1, value2, value3);
	}
	void Pass::SetFloat3(const string name, float value1, float value2, float value3)
	{
		m_shaders.SetFloat3(name, value1, value2, value3);
	}
	void Pass::SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
	{
		m_shaders.SetUint3(name, value1, value2, value3);
	}
	void Pass::SetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
	{
		m_shaders.SetBool4(name, value1, value2, value3, value4);
	}
	void Pass::SetInt4(const string name, int value1, int value2, int value3, int value4)
	{
		m_shaders.SetInt4(name, value1, value2, value3, value4);
	}
	void Pass::SetFloat4(const string name, float value1, float value2, float value3, float value4)
	{
		m_shaders.SetFloat4(name, value1, value2, value3, value4);
	}
	void Pass::SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
	{
		m_shaders.SetUint4(name, value1, value2, value3, value4);
	}
	void Pass::SetMat4(const string name, glm::mat4 value)
	{
		m_shaders.SetMat4(name, value);
	}
	void Pass::SetInputLayout(unsigned int VAO)
	{
		m_shaders.SetInputLayout(VAO);
	}
#endif

#if defined(DX11)
	void Pass::AddTrackValue(string name, unsigned int id, unsigned int size)
	{
		m_values.push_back(Values(name, id, size));
	}
#elif defined(OGL)
	void Pass::AddTrackValue(string name, string uniform, eDataType type)
	{
		m_values.push_back(Values(name, uniform, type));
	}
#endif

	void Pass::AddInputTexture(string name)
	{
		m_inputTextures.insert(make_pair(name, Texture()));
		// TODO: Terminar de setear el sistema de intercambio de texturas entre pases
	}

	void Pass::SetInputTexture(string name, Texture tex)
	{
		m_inputTextures[name] = tex;
	}

	void Pass::AddOutputTexture(string name)
	{
		m_outputTextures.insert(make_pair(name, RenderTargetView()));
	}

	void Pass::SetOutputTexture(string name, RenderTargetView tex)
	{
		m_outputTextures[name] = tex;
	}

	void Pass::SetValue(string name, void* data)
	{
		for (Values& v : m_values)
		{
			if (v.m_name == name)
			{
				memcpy(v.m_data, data, v.m_size);
			}
		}
	}





#if defined(DX11)
	Values::Values(string name, unsigned int id, unsigned int size)
	{
		m_name = name;

		m_id = id;
		m_size = size;
	}
#elif defined(OGL)
	Values::Values(string name, string uniform, eDataType type)
	{
		m_name = name;
	
		m_uniform = uniform;
		m_type = type;
	}
#endif

	Values::~Values()
	{
		if (nullptr != m_data)
		{
			delete m_data;
			m_data = nullptr;
		}
		m_buff.Release();
	}

	Values::Values(const Values& other)
	{
	
		m_name = other.m_name;
		m_data = (void*)new char[other.m_size];
#if defined(DX11)
		m_id = other.m_id;
		m_size = other.m_size;
	
		BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = USAGE_DEFAULT;
		bd.ByteWidth = other.m_size;
		bd.BindFlags = BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		GetManager()->CreateBuffer(&bd, NULL, m_buff);
#elif defined(OGL)
		m_uniform = other.m_uniform;
		m_type = other.m_type;
	}
#endif
	}
}