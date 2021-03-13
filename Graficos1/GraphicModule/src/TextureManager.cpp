#include "TextureManager.h"
#include "RenderManager.h"
#include <string>
#include <iostream>

map<string, GraphicsModule::ShaderResourceView> TextureManager::m_textures;
GraphicsModule::ShaderResourceView& TextureManager::GetTexture(string name)
{
	return m_textures[name];
}

void TextureManager::CreateTextureFromFile(LPCSTR pSrcFile, string name)
{
	if (m_textures.find(name) != m_textures.end())
	{
		std::cout << "Nombre de textura ya existente: " << name << std::endl;
		return;
	}
	m_textures.insert(make_pair(name, GraphicsModule::ShaderResourceView()));
	if (FAILED(GraphicsModule::GetManager()->CreateShaderResourceViewFromFile(pSrcFile, NULL, NULL, m_textures[name], NULL)))
	{
		std::cout << "Textura no vailda: " << pSrcFile << std::endl;
		return;
	}
}
