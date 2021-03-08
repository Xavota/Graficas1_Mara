#include "TextureManager.h"
#include "RenderManager.h"

map<string, GraphicsModule::ShaderResourceView> TextureManager::m_textures;
GraphicsModule::ShaderResourceView& TextureManager::GetTexture(string name)
{
	return m_textures[name];
}

void TextureManager::CreateTextureFromFile(LPCSTR pSrcFile, string name)
{
	m_textures.insert(make_pair(name, GraphicsModule::ShaderResourceView()));
	GraphicsModule::GetManager()->CreateShaderResourceViewFromFile(pSrcFile, NULL, NULL, m_textures[name], NULL);
}
