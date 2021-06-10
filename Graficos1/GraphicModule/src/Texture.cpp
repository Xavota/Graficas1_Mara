#include "Texture.h"
#include "RenderManager.h"
#include "FreeImage.h"

namespace GraphicsModule
{
bool Texture::CreateTextureFromFile(LPCSTR pSrcFile, unsigned int Flags, eDIMENSION dim)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP* dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);

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
	{
		dib = FreeImage_Load(fif, pSrcFile);
		dib = FreeImage_ConvertTo32Bits(dib);
	}

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
#if defined(DX11)
	if (dim == eDIMENSION::TEXTURE2D)
	{
		Texture2D tex;
		TEXTURE2D_DESC texD;
		ZeroMemory(&texD, sizeof(texD));
		texD.Width = width;
		texD.Height = height;
		texD.MipLevels = 1;
		texD.ArraySize = 1;
		if (Flags & MODEL_LOAD_FORMAT_RGBA)
			texD.Format = FORMAT_R8G8B8A8_UNORM;
		else if (Flags & MODEL_LOAD_FORMAT_BGRA)
			texD.Format = FORMAT_B8G8R8A8_UNORM;
		texD.SampleDesc.Count = 1;
		texD.SampleDesc.Quality = 0;
		texD.Usage = USAGE_DEFAULT;
		texD.BindFlags = BIND_SHADER_RESOURCE;
		texD.CPUAccessFlags = 0;
		texD.MiscFlags = 0;
		
		if (FAILED(GetManager()->CreateTexture2D(&texD, NULL, tex)))
		{
			return false;
		}
		GetManager()->UpdateTexture2D(tex, bits, FreeImage_GetPitch(dib));

		
		SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		if (Flags & MODEL_LOAD_FORMAT_RGBA)
			srvd.Format = FORMAT_R8G8B8A8_UNORM;
		else if (Flags & MODEL_LOAD_FORMAT_BGRA)
			srvd.Format = FORMAT_B8G8R8A8_UNORM;
		srvd.ViewDimension = SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1; // same as orig texture

		/*if (dim == eDIMENSION::TEX_CUBE)
		{	
			texD.ArraySize = 6;
			texD.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		}/**/

		GetManager()->CreateShaderResourceView(tex, &srvd, m_texture);
		tex.Release();
	}
	else if (dim == eDIMENSION::TEX_CUBE)
	{
		Texture2D SMTexture;
		HRESULT hr = D3DX11CreateTextureFromFile(GetManager()->getDevicePtr(), pSrcFile,
			NULL, 0, (ID3D11Resource**)&SMTexture.GetTexturePtr(), 0);

		if (FAILED(hr))
		{
			return false;
		}

		TEXTURE2D_DESC SMTextureDesc;
		SMTexture.GetTexturePtr()->GetDesc((D3D11_TEXTURE2D_DESC*)&SMTextureDesc);

		SHADER_RESOURCE_VIEW_DESC SMViewDesc;
		SMViewDesc.Format = SMTextureDesc.Format;
		SMViewDesc.ViewDimension = SRV_DIMENSION_TEXTURECUBE;
		SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
		SMViewDesc.TextureCube.MostDetailedMip = 0;

		hr = GetManager()->CreateShaderResourceView(SMTexture, &SMViewDesc, m_texture);

		if (FAILED(hr))
		{
			return false;
		}
	}

	
#elif defined(OGL)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//OpenGL's image ID to map to
	GLuint gl_texID;
	//generate an OpenGL texture ID for this texture	
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	m_ID = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	//store the texture data for OpenGL use
	if (Flags & MODEL_LOAD_FORMAT_RGBA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
	else if (Flags & MODEL_LOAD_FORMAT_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);

#endif           
	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);
 	return true;
}

#if defined(DX11)
bool Texture::CreateTextureFromBuffer(Texture2D& buffer)
{
	SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1; // same as orig texture
	HRESULT hr = GetManager()->CreateShaderResourceView(buffer, &srvDesc, m_texture);
	if (FAILED(hr))
		return false;
	return true;
}
#endif
}