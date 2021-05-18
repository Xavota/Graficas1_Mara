#pragma once
#include <string>

#if defined(DX11)
#include "Buffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#elif defined(OGL)
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

using namespace std;

namespace GraphicsModule
{
class Shader
{
public:
	Shader() = default;
	Shader(const char* vertexShaderPath, const char* pixelShaderPath);
	~Shader();

	void Init(const char* vertexShaderPath, const char* pixelShaderPath);

#if defined(DX11)
	void SetBuffer(int slot, Buffer data);
#elif defined(OGL)
	void Use();
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
private:
#if defined(DX11)
	VertexShader m_vertex;
	PixelShader m_pixel;
#elif defined(OGL)
	unsigned int m_ID = 0;
#endif
};
}