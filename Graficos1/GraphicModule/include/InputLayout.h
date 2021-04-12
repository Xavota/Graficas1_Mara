#pragma once
#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class InputLayout
{
public:
	InputLayout() = default;
	~InputLayout() = default;

#if defined(DX11)
	inline ID3D11InputLayout*& getInputLayout() { return m_inputLayout; }
#endif
	void Release() 
	{
#if defined(DX11)
		if (m_inputLayout) m_inputLayout->Release(); 
#endif
	}

private:
#if defined(DX11)
	ID3D11InputLayout* m_inputLayout;
#endif
};
}
