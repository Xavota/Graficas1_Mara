#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "RenderManager.h"

namespace GraphicsModule
{

Shader::~Shader()
{
}

void Shader::CompileFromFile(const char* vertexShaderPath, const char* pixelShaderPath)
{
#if defined(DX11)
	HRESULT hr = S_OK;
	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL;
	hr = GraphicsModule::GetManager()->CompileShaderFromFile(vertexShaderPath, "main", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return;
	}

	// Create the vertex shader
	hr = GraphicsModule::GetManager()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, m_vertex);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return;
	}




	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = GraphicsModule::GetManager()->CompileShaderFromFile(pixelShaderPath, "main", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return;
	}

	// Create the pixel shader
	hr = GraphicsModule::GetManager()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, m_pixel);
	pPSBlob->Release();
	if (FAILED(hr))
		return;




	GraphicsModule::GetManager()->CreateInputLayoutDescFromVertexShaderSignature(pVSBlob, m_inputLayout);

	pVSBlob->Release();
	if (FAILED(hr))
		return;

#elif defined(OGL)
	ifstream vShaderFile;
	ifstream pShaderFile;
	stringstream ssVertex;
	stringstream ssPixel;

	vShaderFile.open(vertexShaderPath);
	pShaderFile.open(pixelShaderPath);

	ssVertex << vShaderFile.rdbuf();
	ssPixel << pShaderFile.rdbuf();

	vShaderFile.close();
	pShaderFile.close();

	string sv = ssVertex.str();
	string sp = ssPixel.str();

	const char* vShaderCode = sv.c_str();
	const char* pShaderCode = sp.c_str();

	unsigned int vertex, pixel;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	pixel = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pixel, 1, &pShaderCode, NULL);
	glCompileShader(pixel);
	// print compile errors if any
	glGetShaderiv(pixel, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(pixel, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PIXEL::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, pixel);
	glLinkProgram(m_ID);
	// print linking errors if any
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(pixel);
#endif
}

void Shader::CompileFromString(const char* vertexShaderString, const char* pixelShaderString)
{
#if defined(DX11)
	HRESULT hr = S_OK;
	// Compile the vertex shader
	ID3DBlob* pVSBlob = NULL; 
	ID3DBlob* pVSErrorBlob = NULL;

	hr = D3DCompile(
		vertexShaderString,
		strlen(vertexShaderString),
		nullptr,
		nullptr,
		nullptr,
		"main", "vs_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&pVSBlob,
		&pVSErrorBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);

		if (pVSErrorBlob != NULL)
			OutputDebugStringA((char*)pVSErrorBlob->GetBufferPointer());
		if (pVSErrorBlob) pVSErrorBlob->Release();
		return;
	}

	// Create the vertex shader
	hr = GraphicsModule::GetManager()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, m_vertex);

	if (pVSErrorBlob) pVSErrorBlob->Release();

	if (FAILED(hr))
	{
		return;
	}



	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	ID3DBlob* pPSErrorBlob = NULL;
	hr = D3DCompile(
		pixelShaderString,
		strlen(pixelShaderString),
		nullptr,
		nullptr,
		nullptr,
		"main", "ps_4_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0,
		&pPSBlob,
		&pPSErrorBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		
		if (pPSErrorBlob != NULL)
			OutputDebugStringA((char*)pPSErrorBlob->GetBufferPointer());
		if (pPSErrorBlob) pPSErrorBlob->Release();
		return;
	}

	// Create the pixel shader
	hr = GraphicsModule::GetManager()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, m_pixel);

	if (pPSErrorBlob) pPSErrorBlob->Release();

	if (FAILED(hr))
		return;




	GraphicsModule::GetManager()->CreateInputLayoutDescFromVertexShaderSignature(pVSBlob, m_inputLayout);

	if (pVSBlob) pVSBlob->Release();
	if (pPSBlob) pPSBlob->Release();

	if (FAILED(hr))
		return;

#elif defined(OGL)
	/*ifstream vShaderFile;
	ifstream pShaderFile;
	stringstream ssVertex;
	stringstream ssPixel;

	vShaderFile.open(vertexShaderPath);
	pShaderFile.open(pixelShaderPath);

	ssVertex << vShaderFile.rdbuf();
	ssPixel << pShaderFile.rdbuf();

	vShaderFile.close();
	pShaderFile.close();

	string sv = ssVertex.str();
	string sp = ssPixel.str();

	const char* vShaderCode = sv.c_str();
	const char* pShaderCode = sp.c_str();*/

	unsigned int vertex, pixel;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderString, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	pixel = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pixel, 1, &pixelShaderString, NULL);
	glCompileShader(pixel);
	// print compile errors if any
	glGetShaderiv(pixel, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(pixel, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PIXEL::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, pixel);
	glLinkProgram(m_ID);
	// print linking errors if any
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(pixel);
#endif
}

void Shader::Use()
{
#if defined(DX11)
	GraphicsModule::GetManager()->IASetInputLayout(m_inputLayout);

	GraphicsModule::GetManager()->VSSetShader(m_vertex, NULL, 0);
	GraphicsModule::GetManager()->PSSetShader(m_pixel, NULL, 0);
#elif defined(OGL)
	//glBindVertexArray(m_inputLayout);
	glUseProgram(m_ID);
#endif
}

#if defined(DX11)
void Shader::SetBuffer(int slot, Buffer buff, void* data)
{
	GraphicsModule::GetManager()->UpdateSubresource(buff, 0, NULL, data, 0, 0);
	GraphicsModule::GetManager()->VSSetConstantBuffers(slot, 1, buff);
	GraphicsModule::GetManager()->PSSetConstantBuffers(slot, 1, buff);
}

#elif defined(OGL)

void Shader::Unuse()
{
	glUseProgram(0);
}

void Shader::SetBool(const string name, bool value)
{
	//Use();
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
	//Unuse();
}

void Shader::SetInt(const string name, int value)
{
	//Use();
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	//Unuse();
}

void Shader::SetFloat(const string name, float value)
{
	//Use();
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	//Unuse();
}

void Shader::SetUint(const string name, unsigned int value)
{
	//Use();
	glUniform1ui(glGetUniformLocation(m_ID, name.c_str()), value);
	//Unuse();
}

void Shader::SetBool2(const string name, bool value1, bool value2)
{
	//Use();
	glUniform2i(glGetUniformLocation(m_ID, name.c_str()), (int)value1, (int)value2);
	//Unuse();
}

void Shader::SetInt2(const string name, int value1, int value2)
{
	//Use();
	glUniform2i(glGetUniformLocation(m_ID, name.c_str()), value1, value2);
	//Unuse();
}

void Shader::SetFloat2(const string name, float value1, float value2)
{
	//Use();
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value1, value2);
	//Unuse();
}

void Shader::SetUint2(const string name, unsigned int value1, unsigned int value2)
{
	//Use();
	glUniform2ui(glGetUniformLocation(m_ID, name.c_str()), value1, value2);
	//Unuse();
}

void Shader::SetBool3(const string name, bool value1, bool value2, bool value3)
{
	//Use();
	glUniform3i(glGetUniformLocation(m_ID, name.c_str()), (int)value1, (int)value2, (int)value3);
	//Unuse();
}

void Shader::SetInt3(const string name, int value1, int value2, int value3)
{
	//Use();
	glUniform3i(glGetUniformLocation(m_ID, name.c_str()), value1, value2, value3);
	//Unuse();
}

void Shader::SetFloat3(const string name, float value1, float value2, float value3)
{
	//Use();
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value1, value2, value3);
	//Unuse();
}

void Shader::SetUint3(const string name, unsigned int value1, unsigned int value2, unsigned int value3)
{
	//Use();
	glUniform3ui(glGetUniformLocation(m_ID, name.c_str()), value1, value2, value3);
	//Unuse();
}

void Shader::SetBool4(const string name, bool value1, bool value2, bool value3, bool value4)
{
	//Use();
	glUniform4i(glGetUniformLocation(m_ID, name.c_str()), (int)value1, (int)value2, (int)value3, (int)value4);
	//Unuse();
}

void Shader::SetInt4(const string name, int value1, int value2, int value3, int value4)
{
	//Use();
	glUniform4i(glGetUniformLocation(m_ID, name.c_str()), value1, value2, value3, value4);
	//Unuse();
}

void Shader::SetFloat4(const string name, float value1, float value2, float value3, float value4)
{
	//Use();
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value1, value2, value3, value4);
	//Unuse();
}

void Shader::SetUint4(const string name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4)
{
	//Use();
	glUniform4ui(glGetUniformLocation(m_ID, name.c_str()), value1, value2, value3, value4);
	//Unuse();
}

void Shader::SetMat4(const string name, glm::mat4 value)
{
	//Use();
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	//Unuse();
}
void Shader::SetInputLayout(unsigned int VAO)
{
	m_inputLayout = VAO;
}
#endif
}