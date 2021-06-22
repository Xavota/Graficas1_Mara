#include "TextureManager.h"
#include "RenderManager.h"
#include <string>
#include <iostream>

namespace GraphicsModule
{
map<string, Texture> TextureManager::m_textures;
Texture& TextureManager::GetTexture(string name)
{
	if (m_textures.find(name) != m_textures.end())
	{
		return m_textures[name];
	}
	std::cout << "Textura no existente: " << name << std::endl;
	std::cout << "Procure cargar una textura con este nombre usando" << endl;
	cout << "\tTextureManager::CreateTextureFromFile(LPCSTR pSrcFile, string name)" << std::endl;
	cout << "\tTextureManager::CreateTextureFromBuffer(Texture2D buffer, string name)" << std::endl;
	Texture t;
	return t;
}

eSTATUS TextureManager::CreateTextureFromFile(std::vector<string> pSrcFile, string name, unsigned int Flags, eDIMENSION dim)
{
	if (m_textures.find(name) != m_textures.end())
	{
		std::cout << "Nombre de textura ya existente: " << name << std::endl;
		return eSTATUS::REPITED;
	}
	m_textures.insert(make_pair(name, Texture()));
	if (!m_textures[name].CreateTextureFromFile(pSrcFile, Flags, dim))
	{
		m_textures.erase(name);
		return eSTATUS::FAIL;
	}
	return eSTATUS::OK;
}

#if defined(DX11)
eSTATUS TextureManager::CreateTextureFromBuffer(Texture2D buffer, string name)
{
	if (m_textures.find(name) != m_textures.end())
	{
		std::cout << "Nombre de textura ya existente: " << name << std::endl;
		return eSTATUS::REPITED;
	}
	m_textures.insert(make_pair(name, Texture()));
	if (!m_textures[name].CreateTextureFromBuffer(buffer))
	{
		m_textures.erase(name);
		return eSTATUS::FAIL;
	}
	return eSTATUS::OK;
}
#endif
}