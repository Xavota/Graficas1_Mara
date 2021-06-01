#include "Technique.h"
#include <fstream>
#include <sstream>

#include "RenderManager.h"

namespace GraphicsModule
{
	void Technique::CompileShader(const char* vertexShaderPath, const char* pixelShaderPath)
	{
	}

	void Technique::CreatePass(string name, const char* vertexShaderPath, const char* pixelShaderPath)
	{
		std::ifstream vsFile(vertexShaderPath);
		std::stringstream vsString;

#if defined(DX11)
		vsString << "#define DX11" << '\n';
#elif defined(OGL)
		vsString << "#version 330 core" << '\n';
		vsString << "#define OGL" << '\n';
#endif

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

		//std::cout << vsString.str() << endl;

		std::ifstream psFile(pixelShaderPath);
		std::stringstream psString;

#if defined(DX11)
		psString << "#define DX11" << '\n';
#elif defined(OGL)
		psString << "#version 330 core" << '\n';
		psString << "#define OGL" << '\n';
#endif

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


		m_passes.insert(make_pair(name, Pass()));
		m_passes[name].Compile(vsString.str().c_str(), psString.str().c_str());
	}

	void Technique::AddDefine(std::string def)
	{
		m_defines.push_back(def);	
	}

	void Technique::Use(unsigned int indexCount)
	{
		bool f = true;
		for (std::map<string, Pass>::iterator p = m_passes.begin(); p != m_passes.end(); p++)
		{
			for (TextureExchange& te : m_textureExchanges)
			{
				if (te.m_outputPassName == p->first)
				{
					p->second.SetOutputTexture(te.m_outputTextureName, &te.m_outputTexture, te.m_depthStencil);
				}
				else
				{
					for (int i = 0; i < te.m_inputPassesNames.size(); i++)
					{
						if (te.m_inputPassesNames[i] == p->first)
						{
							p->second.SetInputTexture(te.m_inputTexturesNames[i], te.m_inputTextures[i]);
							break;
						}
					}
				}
			}

			p->second.Use();

			for (Values& v : m_values)
			{
#if defined(DX11)
				p->second.SetBuffer(v.m_id, v.m_buff, v.m_data);
#elif defined(OGL)
				switch (v.m_type)
				{
				case eDataType::BOOL:
					p->second.SetBool(v.m_uniform, *(bool*)v.m_data);
					break;
				case eDataType::INT:
					p->second.SetBool(v.m_uniform, *(int*)v.m_data);
					break;
				case eDataType::FLOAT:
					p->second.SetBool(v.m_uniform, *(float*)v.m_data);
					break;
				case eDataType::UINT:
					p->second.SetBool(v.m_uniform, *(unsigned int*)v.m_data);
					break;
				case eDataType::BOOL2:
				{
					bool* data = (bool*)v.m_data;
					p->second.SetBool2(v.m_uniform, data[0], data[1]);
					break;
				}
				case eDataType::INT2:
				{
					int* data = (int*)v.m_data;
					p->second.SetInt2(v.m_uniform, data[0], data[1]);
					break;
				}
				case eDataType::FLOAT2:
				{
					float* data = (float*)v.m_data;
					p->second.SetFloat2(v.m_uniform, data[0], data[1]);
					break;
				}
				case eDataType::UNIT2:
				{
					unsigned int* data = (unsigned int*)v.m_data;
					p->second.SetUint2(v.m_uniform, data[0], data[1]);
					break;
				}
				case eDataType::BOOL3:
				{
					bool* data = (bool*)v.m_data;
					p->second.SetBool3(v.m_uniform, data[0], data[1], data[2]);
					break;
				}
				case eDataType::INT3:
				{
					int* data = (int*)v.m_data;
					p->second.SetInt3(v.m_uniform, data[0], data[1], data[2]);
					break;
				}
				case eDataType::FLOAT3:
				{
					float* data = (float*)v.m_data;
					p->second.SetFloat3(v.m_uniform, data[0], data[1], data[2]);
					break;
				}
				case eDataType::UINT3:
				{
					unsigned int* data = (unsigned int*)v.m_data;
					p->second.SetUint3(v.m_uniform, data[0], data[1], data[2]);
					break;
				}
				case eDataType::BOOL4:
				{
					bool* data = (bool*)v.m_data;
					p->second.SetBool4(v.m_uniform, data[0], data[1], data[2], data[3]);
					break;
				}
				case eDataType::INT4:
				{
					int* data = (int*)v.m_data;
					p->second.SetInt4(v.m_uniform, data[0], data[1], data[2], data[3]);
					break;
				}
				case eDataType::FLOAT4:
				{
					float* data = (float*)v.m_data;
					p->second.SetFloat4(v.m_uniform, data[0], data[1], data[2], data[3]);
					break;
				}
				case eDataType::UINT4:
				{
					unsigned int* data = (unsigned int*)v.m_data;
					p->second.SetUint4(v.m_uniform, data[0], data[1], data[2], data[3]);
					break;
				}
				}
#endif
			}

			if (f)
			{	
				p->second.Draw(indexCount);
				f = false;
			}
			else
			{
				Mesh m;
				std::vector<Vertex> vertices =
				{
					Vertex{Vector3{-1,-1,0}, Vector2{ 0, 1}, Vector3{0,0,0}, Vector3{0,0,0}, Vector3{0,0,0}},
					Vertex{Vector3{-1, 1,0}, Vector2{ 0, 0}, Vector3{0,0,0}, Vector3{0,0,0}, Vector3{0,0,0}},
					Vertex{Vector3{ 1, 1,0}, Vector2{ 1, 0}, Vector3{0,0,0}, Vector3{0,0,0}, Vector3{0,0,0}},
					Vertex{Vector3{-1,-1,0}, Vector2{ 0, 1}, Vector3{0,0,0}, Vector3{0,0,0}, Vector3{0,0,0}},
					Vertex{Vector3{ 1, 1,0}, Vector2{ 1, 0}, Vector3{0,0,0}, Vector3{0,0,0}, Vector3{0,0,0}},
					Vertex{Vector3{ 1,-1,0}, Vector2{ 1, 1}, Vector3{0,0,0}, Vector3{0,0,0}, Vector3{0,0,0}},
				};

				m.setVertex(vertices);
				m.setIndices({0,1,2,3,4,5});

				UINT stride = sizeof(Vertex);
				UINT offset = 0;
				GetManager()->IASetVertexBuffers(0, 1, m.getVertexBuffer(), &stride, &offset);
				GetManager()->IASetIndexBuffer(m.getIndexBuffer(), FORMAT_R32_UINT, offset);

				p->second.Draw(m.getIndexCount());
			}
		}
	}
#if defined(DX11)
	void Technique::SetBuffer(int slot, Buffer buff, void* data)
	{
		for (map<string, Pass>::iterator p; p != m_passes.end(); p++)
		{
			p->second.SetBuffer(slot, buff, data);
		}
	}
#elif defined(OGL)
	void Technique::Unuse()
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.Unuse();
		}
	}
	void Technique::SetBool(const string name, bool value)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetBool(name, value);
		}
	}
	void Technique::SetInt(const string name, int value)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetInt(name, value);
		}
	}
	void Technique::SetFloat(const string name, float value)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetFloat(name, value);
		}
	}
	void Technique::SetUint(const string name, unsigned int value)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetUint(name, value);
		}
	}
	void Technique::SetBool2(const string name, bool value1, bool value2)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetBool2(name, value1, value2);
		}
	}
	void Technique::SetInt2(const string name, int value1, int value2)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetInt2(name, value1, value2);
		}
	}
	void Technique::SetFloat2(const string name, float value1, float value2)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetFloat2(name, value1, value2);
		}
	}
	void Technique::SetUint2(const string name, unsigned int value1, unsigned int value2)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetUint2(name, value1, value2);
		}
	}
	void Technique::SetBool3(const string name, bool value1, bool value2, bool value3)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetBool3(name, value1, value2, value3);
		}
	}
	void Technique::SetInt3(const string name, int value1, int value2, int value3)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetInt3(name, value1, value2, value3);
		}
	}
	void Technique::SetFloat3(const string name, float value1, float value2, float value3)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetFloat3(name, value1, value2, value3);
		}
	}
	void Technique::SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetUint3(name, value1, value2, value3);
		}
	}
	void Technique::SetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetBool4(name, value1, value2, value3, value4);
		}
	}
	void Technique::SetInt4(const string name, int value1, int value2, int value3, int value4)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetInt4(name, value1, value2, value3, value4);
		}
	}
	void Technique::SetFloat4(const string name, float value1, float value2, float value3, float value4)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetFloat4(name, value1, value2, value3, value4);
		}
	}
	void Technique::SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetUint4(name, value1, value2, value3, value4);
		}
	}
	void Technique::SetMat4(const string name, glm::mat4 value)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetMat4(name, value);
		}
	}
	void Technique::SetInputLayout(unsigned int VAO)
	{
		for (map<string, Pass>::iterator ps; ps != m_passes.end(); ps++)
		{
			ps->second.SetInputLayout(VAO);
		}
	}
#endif

#if defined(DX11)
	void Technique::AddTrackValue(string name, unsigned int id, unsigned int size)
	{
		m_values.push_back(Values(name, id, size));
	}

	void Technique::AddPassTrackValue(string passName, string name, unsigned int id, unsigned int size)
	{
		m_passes[passName].AddTrackValue(name, id, size);
	}

	void Technique::AddPassInputTexture(string passName, string textureName)
	{
		m_passes[passName].AddInputTexture(textureName);
	}

	void Technique::SetPassInputTexture(string passName, string textureName, Texture tex)
	{
		m_passes[passName].SetInputTexture(textureName, tex);
	}

	void Technique::AddPassOutputTexture(string passName, string textureName)
	{
		m_passes[passName].AddOutputTexture(textureName);
	}

	void Technique::SetPassOutputTexture(string passName, string textureName, RenderTargetView* tex, DepthStencilView dsv)
	{
		m_passes[passName].SetOutputTexture(textureName, tex, dsv);
	}

	void Technique::UniteInputOutputTextures(string outputPassName, string outpuTextureName, string inputPassName, string inputTextureName)
	{
		for (TextureExchange& te : m_textureExchanges)
		{
			if (te.m_outputPassName == outputPassName && te.m_outputTextureName == outpuTextureName)
			{
				te.m_inputPassesNames.push_back(inputPassName);
				te.m_inputTexturesNames.push_back(inputTextureName);
				return;
			}
		}
		m_textureExchanges.push_back(TextureExchange( outputPassName, outpuTextureName, {inputPassName}, {inputTextureName}));
	}

#elif defined(OGL)
	void Technique::AddTrackValue(string name, string uniform, eDataType type)
	{
		m_values.push_back(Values(name, uniform, type));
	}
	void Technique::AddPassTrackValue(string passName, string name, string uniform, eDataType type)
	{
		m_passes[passName].AddTrackValue(name, uniform, type);
	}
#endif
	void Technique::SetValue(string name, void* data)
	{
		for (Values& v : m_values)
		{
			if (v.m_name == name)
			{
				memcpy(v.m_data, data, v.m_size);
			}
		}
	}



	Technique::TextureExchange::TextureExchange(string outputPassName, string outputTextureName, std::vector<string> inputPassesNames, std::vector<string> inputTexturesNames)
	{
		m_outputPassName = outputPassName;
		m_outputTextureName = outputTextureName;
		m_inputPassesNames = inputPassesNames;
		m_inputTexturesNames = inputTexturesNames;


		Texture2D depth;
		TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = 1264;
		descDepth.Height = 681;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = USAGE_DEFAULT;
		descDepth.BindFlags = BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		if (FAILED(GetManager()->CreateTexture2D(&descDepth, NULL, depth)))
			return;

		DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		if (FAILED(GetManager()->CreateDepthStencilView(depth, &descDSV, m_depthStencil)))
			return;

		depth.Release();

		Texture2D Tex;
		TEXTURE2D_DESC descTextRT;
		ZeroMemory(&descTextRT, sizeof(descTextRT));
		descTextRT.Width = 1264;
		descTextRT.Height = 681;
		descTextRT.MipLevels = 1;
		descTextRT.ArraySize = 1;
		descTextRT.Format = FORMAT_R8G8B8A8_UNORM;
		descTextRT.SampleDesc.Count = 1;
		descTextRT.SampleDesc.Quality = 0;
		descTextRT.Usage = USAGE_DEFAULT;
		descTextRT.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
		descTextRT.CPUAccessFlags = 0;
		descTextRT.MiscFlags = 0;
		if (FAILED(GetManager()->CreateTexture2D(&descTextRT, NULL, Tex)))
			return;

		if (FAILED(GetManager()->CreateRenderTargetView(Tex, NULL, m_outputTexture)))
			return;


		for (int i = 0; i < m_inputTexturesNames.size(); i++)
		{
			m_inputTextures.push_back(Texture());
			if (!m_inputTextures[i].CreateTextureFromBuffer(Tex))
			{
				Tex.Release();
				return;
			}
		}
	}
	//1982
}