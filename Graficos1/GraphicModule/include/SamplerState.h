#pragma once
#if defined(DX11)
#include <d3d11.h>
#endif

namespace GraphicsModule
{
class SamplerState
{
public:
	SamplerState() = default;
	~SamplerState() = default;

#if defined(DX11)
	inline ID3D11SamplerState*& getSamplerState() { return m_samplerState; }
	void Release() { if (m_samplerState) m_samplerState->Release(); }
#endif

private:
#if defined(DX11)
	ID3D11SamplerState* m_samplerState;
#endif
};
}