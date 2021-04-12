#include "Texture.h"
#include "RenderManager.h"
#include "FreeImage.h"

namespace GraphicsModule
{
bool Texture::CreateTextureFromFile(LPCSTR pSrcFile)
{
#if defined(DX11)
	if (FAILED(GetManager()->CreateShaderResourceViewFromFile(pSrcFile, NULL, NULL, m_texture, NULL)))
	{
		std::cout << "Textura no vailda: " << pSrcFile << std::endl;
		return false;
	}
#elif defined(OGL)
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP* dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(pSrcFile, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(pSrcFile);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, pSrcFile);
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	m_ID = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
		0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
#endif           
	return true;
}

#if defined(DX11)
bool Texture::CreateTextureFromBuffer(Texture2D buffer)
{
	SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1; // same as orig texture
	HRESULT hr = GetManager()->CreateShaderResourceView(buffer, &srvDesc, m_texture);
	if (FAILED(hr))
		return false;
	return true;
}
#endif
}