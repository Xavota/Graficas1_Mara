#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif
namespace GraphicsModule
{
class Buffer
{
public:
	Buffer() = default;
	~Buffer() = default;

#if defined(DX11)
	inline ID3D11Buffer*& getBufferPtr() { return m_Buffer; }
#endif
	inline void Release() 
	{ 
#if defined(DX11)
		if (m_Buffer) m_Buffer->Release(); 
#endif
	}

private:
#if defined(DX11)
	ID3D11Buffer* m_Buffer = nullptr;
#endif
};
}
