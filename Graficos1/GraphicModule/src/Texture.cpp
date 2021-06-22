#include "Texture.h"
#include "RenderManager.h"
#include "FreeImage.h"
#if defined(OGL)
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>
#endif

namespace GraphicsModule
{
bool Texture::CreateTextureFromFile(std::vector<std::string> pSrcFile, unsigned int Flags, eDIMENSION dim)
{
	std::vector<BYTE*> bits;
	std::vector<FIBITMAP*> dib;
	std::vector<unsigned int> width, height;
	for (int i = 0; i < pSrcFile.size(); i++)
	{
		bits.push_back(0);
		width.push_back(0);
		height.push_back(0);
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		dib.push_back(0);
		//pointer to the image data
		//image width and height

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(pSrcFile[i].c_str(), 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(pSrcFile[i].c_str());
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return false;

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
		{
			dib[i] = FreeImage_Load(fif, pSrcFile[i].c_str());
			dib[i] = FreeImage_ConvertTo32Bits(dib[i]);
		}

		//if the image failed to load, return failure
		if (!dib[i])
			return false;

		//retrieve the image data
		bits[i] = FreeImage_GetBits(dib[i]);
		//get the image width and height
		width[i] = FreeImage_GetWidth(dib[i]);
		height[i] = FreeImage_GetHeight(dib[i]);
		//if this somehow one of these failed (they shouldn't), return failure
		if ((bits[i] == 0) || (width[i] == 0) || (height[i] == 0))
			return false;
	}
#if defined(DX11)
	if (dim == eDIMENSION::TEXTURE2D)
	{
		Texture2D tex;
		TEXTURE2D_DESC texD;
		ZeroMemory(&texD, sizeof(texD));
		texD.Width = width[0];
		texD.Height = height[0];
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
		GetManager()->UpdateTexture2D(tex, bits[0], FreeImage_GetPitch(dib[0]));

		
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
		HRESULT hr = D3DX11CreateTextureFromFile(GetManager()->getDevicePtr(), pSrcFile[0].c_str(),
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
	if (dim == eDIMENSION::TEXTURE2D)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//OpenGL's image ID to map to
		GLuint gl_texID = 0;
		//generate an OpenGL texture ID for this texture	
		glGenTextures(1, &gl_texID);
		//store the texture ID mapping
		m_ID = gl_texID;
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, gl_texID);
		//store the texture data for OpenGL use
		if (Flags & MODEL_LOAD_FORMAT_RGBA)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[0], height[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, bits[0]);
		else if (Flags & MODEL_LOAD_FORMAT_BGRA)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[0], height[0], 0, GL_BGRA, GL_UNSIGNED_BYTE, bits[0]);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else if (dim == eDIMENSION::TEX_CUBE)
	{
		GLuint gl_texID;
		glGenTextures(1, &gl_texID);

		m_ID = gl_texID;

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		for (unsigned int i = 0; i < bits.size(); i++)
		{
			if (Flags & MODEL_LOAD_FORMAT_RGBA)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width[i], height[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, bits[i]);
			else if (Flags & MODEL_LOAD_FORMAT_BGRA)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width[i], height[i], 0, GL_BGRA, GL_UNSIGNED_BYTE, bits[i]);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}

#endif           
	//Free FreeImage's copy of the data
	for (int i = 0; i < dib.size(); i++)
	{
		FreeImage_Unload(dib[i]);
	}
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
#elif defined(OGL)
void Texture::CreateEmptyTexture(int width, int height)
{
	glGenTextures(1, &m_ID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
#endif
}