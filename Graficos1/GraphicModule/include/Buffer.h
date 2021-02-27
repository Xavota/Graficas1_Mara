#pragma once

#if defined(DX11)
#include <d3d11.h>
#endif

class Buffer
{
public:
	Buffer() = default;
	~Buffer() = default;

	inline ID3D11Buffer*& getBufferPtr() { return m_Buffer; }
	inline void Release() { if (m_Buffer) m_Buffer->Release(); }

private:
	ID3D11Buffer* m_Buffer;
};

