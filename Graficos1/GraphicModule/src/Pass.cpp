#include "Pass.h"
#include "RenderManager.h"

namespace GraphicsModule
{
	Pass::Pass(CULL_MODE cull)
	{
#if defined(DX11)
		RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(RASTERIZER_DESC));
		rsDesc.FillMode = FILL_SOLID;
		rsDesc.CullMode = cull;
		rsDesc.FrontCounterClockwise = true;

		if (FAILED(GetManager()->CreateRasterizerState(&rsDesc, m_rsState)))
		{
			cout << "mal" << endl;
		}
#elif defined(OGL)
		switch (cull)
		{
		case CULL_NONE:
			m_rsState.setCullingMode(GL_FRONT_AND_BACK);
			break;
		case CULL_FRONT:
			m_rsState.setCullingMode(GL_FRONT);
			break;
		case CULL_BACK:
			m_rsState.setCullingMode(GL_BACK);
			break;
		}
	//TODO
#endif
	}

	void Pass::Compile(const char* vertexShaderString, const char* pixelShaderString)
	{
		m_shaders.CompileFromString(vertexShaderString, pixelShaderString);
	}

	void Pass::Use()
	{
		m_shaders.Use();
		/*for (Values& v : m_values)
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
		}*/

#if defined(DX11)
		std::vector<RenderTargetView> rtvs;
		std::vector<bool> cleans;
		std::vector<float*> clearColors;
		for (OutputTexture& ot : m_outputTextures)
		{
			if (ot.m_renderTarget != nullptr)
			{
				rtvs.push_back(*ot.m_renderTarget);
				cleans.push_back(ot.m_cleanRenderTarget);
				clearColors.push_back(ot.m_clearColor);
			}
		}
		if (rtvs.size() > 0 && m_depthStencil.getStencilViewPtr() != NULL)
			GetManager()->ClearAndSetRenderTargets(rtvs.size(), rtvs.data(), m_depthStencil, clearColors, cleans);
#elif defined(OGL)

		// Set the render buffer


		if (m_outputTextures.size() > 0)
		{
			bool setRender = false;
			for (int i = 0; i < m_outputTextures.size(); i++)
			{
				if (m_outputTextures[i].m_renderTarget != nullptr)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, m_outputTextures[i].m_renderTarget->getID());
					glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencil.getID());
					setRender = true;
					break;
				}
			}

			if (setRender)
			{
				GLenum* DrawBuffers = new GLenum[m_outputTextures.size()];
				for (int i = 0; i < m_outputTextures.size(); i++)
				{
					if (m_outputTextures[i].m_renderTarget != nullptr)
					{
						if (m_outputTextures[i].m_cleanRenderTarget);
						{
							glClearColor(m_outputTextures[i].m_clearColor[0] * m_outputTextures[i].m_clearColor[3], m_outputTextures[i].m_clearColor[1] * m_outputTextures[i].m_clearColor[3], m_outputTextures[i].m_clearColor[2] * m_outputTextures[i].m_clearColor[3], m_outputTextures[i].m_clearColor[3]);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						}

						// Set "renderedTexture" as our colour attachement #0 + i

						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_outputTextures[i].m_renderTarget->getTextureAtatchedID(), 0);

						DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
					}
				}

				// Set the list of draw buffers.
				glDrawBuffers(m_outputTextures.size(), DrawBuffers);
				delete[] DrawBuffers;
			}
		}
#endif
	}

	//void Pass::Draw(unsigned int indexCount)/*
	void Pass::Draw()/**/
	{
#if defined(DX11)
		GetManager()->RSSetState(m_rsState);
#elif defined(OGL)
		glCullFace(m_rsState.getCullingMode());
#endif

		for (ObjectStruct obj : m_objects)
		{
			obj.m_obj->SetResources(GetManager(), obj.m_useTextures);

			// Sets the values stored for the shader
			for (Values& v : m_values)
			{
#if defined(DX11)
				MATRIX* A = (MATRIX*)v.m_data;
				m_shaders.SetBuffer(v.m_id, v.m_buff, v.m_data);
#elif defined(OGL)
				switch (v.m_type)
				{
				case eDataType::BOOL:
					m_shaders.SetBool(v.m_uniform, *(bool*)v.m_data);
					break;
				case eDataType::INT:
					m_shaders.SetInt(v.m_uniform, *(int*)v.m_data);
					break;
				case eDataType::FLOAT:
					m_shaders.SetFloat(v.m_uniform, *(float*)v.m_data);
					break;
				case eDataType::UINT:
					m_shaders.SetUint(v.m_uniform, *(unsigned int*)v.m_data);
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
				case eDataType::MAT:
				{
					glm::mat4* data = (glm::mat4*)v.m_data;
					m_shaders.SetMat4(v.m_uniform, *data);
				}
				}
#endif
			}

			// Sets the textures of the shader in order
			int slot = 0;
			for (InputTexture& it : m_inputTextures)
			{
#if defined(DX11)
				GetManager()->PSSetShaderResources(slot++, { it.m_texture });
#elif defined(OGL)
				GetManager()->ShaderSetInt(it.m_uniform, slot);

				// Uses GL_TEXTURE0 as base, and increment it with 'slot' to GL_TEXTURE1, GL_TEXTURE2, etc., because its values are consecutive.
				glActiveTexture(GL_TEXTURE0 + slot++);
				glBindTexture(GL_TEXTURE_2D, it.m_texture.getID());
#endif
			}

			// Draws the object	
			obj.m_obj->Draw(GetManager(), obj.m_useTextures);
		}
		/**/
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

	void Pass::AddInputTexture(string name)
	{
		m_inputTextures.push_back({ name, Texture() });
	}
#elif defined(OGL)
	void Pass::AddTrackValue(string name, string uniform, eDataType type)
	{
		m_values.push_back(Values(name, uniform, type));
	}

	void Pass::AddInputTexture(string name, string uniform)
	{
		m_inputTextures.push_back({ name, uniform, Texture() });
	}
#endif

	void Pass::SetOutputTexture(string name, RenderTargetView* tex, DepthStencilView dsv)
	{
		for (OutputTexture& ot : m_outputTextures)
		{
			if (ot.m_name == name)
			{
				ot.m_renderTarget = tex;
				ot.m_depthStencil = dsv;
				break;
			}
		}
		m_depthStencil = dsv;
	}

	void Pass::SetInputTexture(string name, Texture tex)
	{
		for (InputTexture& it : m_inputTextures)
		{
			if (it.m_name == name)
				it.m_texture = tex;
		}
	}

	void Pass::AddOutputTexture(string name, bool cleanRenderTarget, float clearColor[4])
	{
		m_outputTextures.push_back(OutputTexture( name, nullptr, cleanRenderTarget, clearColor, DepthStencilView() ));
	}

	void Pass::SetValue(string name, void* data)
	{
		for (Values& v : m_values)
		{
			if (v.m_name == name)
			{
#if defined(DX11)
				memcpy(v.m_data, data, v.m_size);
#elif defined(OGL)
				size_t size;
				switch (v.m_type)
				{
				case eDataType::BOOL:
					size = sizeof(bool);
					break;
				case eDataType::INT:
					size = sizeof(int);
					break;
				case eDataType::FLOAT:
					size = sizeof(float);
					break;
				case eDataType::UINT:
					size = sizeof(unsigned int);
					break;
				case eDataType::BOOL2:
					size = sizeof(bool) * 2;
					break;
				case eDataType::INT2:
					size = sizeof(int) * 2;
					break;
				case eDataType::FLOAT2:
					size = sizeof(float) * 2;
					break;
				case eDataType::UNIT2:
					size = sizeof(unsigned int) * 2;
					break;
				case eDataType::BOOL3:
					size = sizeof(bool) * 3;
					break;
				case eDataType::INT3:
					size = sizeof(int) * 3;
					break;
				case eDataType::FLOAT3:
					size = sizeof(float) * 3;
					break;
				case eDataType::UINT3:
					size = sizeof(unsigned int) * 3;
					break;
				case eDataType::BOOL4:
					size = sizeof(bool) * 4;
					break;
				case eDataType::INT4:
					size = sizeof(int) * 4;
					break;
				case eDataType::FLOAT4:
					size = sizeof(float) * 4;
					break;
				case eDataType::UINT4:
					size = sizeof(unsigned int) * 4;
					break;
				case eDataType::MAT:
					size = sizeof(float) * 4 * 4;
					break;
				}
				memcpy(v.m_data, data, size);
#endif
			}
		}
	}

	void GraphicsModule::Pass::AddObject(OBJInstance* obj, bool useTextures)
	{
		m_objects.push_back({obj, useTextures});
	}

	void GraphicsModule::Pass::ResetObjects(std::vector<OBJInstance*> models)
	{
		m_objects.clear();
		for (int i = 0; i < models.size(); i++)
		{
			m_objects.push_back({ models[i], true });
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
#if defined(DX11)
		m_buff.Release();
#endif
	}

	Values::Values(const Values& other)
	{
	
		m_name = other.m_name;
#if defined(DX11)
		m_data = (void*)new char[other.m_size];
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
		size_t size = 0u;
		switch (other.m_type)
		{
		case eDataType::BOOL:
			size = sizeof(bool);
			break;
		case eDataType::INT:
			size = sizeof(int);
			break;
		case eDataType::FLOAT:
			size = sizeof(float);
			break;
		case eDataType::UINT:
			size = sizeof(unsigned int);
			break;
		case eDataType::BOOL2:
			size = sizeof(bool) * 2;
			break;
		case eDataType::INT2:
			size = sizeof(int) * 2;
			break;
		case eDataType::FLOAT2:
			size = sizeof(float) * 2;
			break;
		case eDataType::UNIT2:
			size = sizeof(unsigned int) * 2;
			break;
		case eDataType::BOOL3:
			size = sizeof(bool) * 3;
			break;
		case eDataType::INT3:
			size = sizeof(int) * 3;
			break;
		case eDataType::FLOAT3:
			size = sizeof(float) * 3;
			break;
		case eDataType::UINT3:
			size = sizeof(unsigned int) * 3;
			break;
		case eDataType::BOOL4:
			size = sizeof(bool) * 4;
			break;
		case eDataType::INT4:
			size = sizeof(int) * 4;
			break;
		case eDataType::FLOAT4:
			size = sizeof(float) * 4;
			break;
		case eDataType::UINT4:
			size = sizeof(unsigned int) * 4;
			break;
		case eDataType::MAT:
			size = sizeof(float) * 4 * 4;
			break;
		}
		m_data = (void*)new char[size];
		m_uniform = other.m_uniform;
		m_type = other.m_type;
#endif
	}
}