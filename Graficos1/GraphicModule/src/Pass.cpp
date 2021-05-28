#include "Pass.h"
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
}