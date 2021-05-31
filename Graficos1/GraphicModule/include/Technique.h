#pragma once
#include <vector>
#include <map>
#include <string>
#include "Pass.h"
#include "Buffer.h"

namespace GraphicsModule
{
class Technique
{
public:
	Technique() = default;
	~Technique() = default;	

	void CompileShader(const char* vertexShaderPath, const char* pixelShaderPath);
	void CreatePass(string name, const char* vertexShaderPath, const char* pixelShaderPath);

	void AddDefine(std::string def);

	void Use(unsigned int indexCount);

#if defined(DX11)
	void SetBuffer(int slot, Buffer buff, void* data);


#elif defined(OGL)
	void Unuse();
	void SetBool(const string name, bool value);
	void SetInt(const string name, int value);
	void SetFloat(const string name, float value);
	void SetUint(const string name, unsigned int value);

	void SetBool2(const string name, bool value1, bool value2);
	void SetInt2(const string name, int value1, int value2);
	void SetFloat2(const string name, float value1, float value2);
	void SetUint2(const string name, unsigned int value1, unsigned int value2);

	void SetBool3(const string name, bool value1, bool value2, bool value3);
	void SetInt3(const string name, int value1, int value2, int value3);
	void SetFloat3(const string name, float value1, float value2, float value3);
	void SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3);

	void SetBool4(const string name, bool value1, bool value2, bool value3, bool value4);
	void SetInt4(const string name, int value1, int value2, int value3, int value4);
	void SetFloat4(const string name, float value1, float value2, float value3, float value4);
	void SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4);

	void SetMat4(const string name, glm::mat4 value);

	void SetInputLayout(unsigned int VAO);
#endif

#if defined(DX11)
	void AddTrackValue(string name, unsigned int id, unsigned int size);
	void AddPassTrackValue(string passName, string name, unsigned int id, unsigned int size);
#elif defined(OGL)
	void AddTrackValue(string name, string uniform, eDataType type);
	void AddPassTrackValue(string passName, string name, string uniform, eDataType type);
#endif

	void SetValue(string name, void* data);
private:
	std::vector<string> m_defines;

	std::map<string, Pass> m_passes;
	
	std::vector<Values> m_values;
};
}
