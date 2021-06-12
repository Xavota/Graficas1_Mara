#include "RasterizeState.h"

namespace GraphicsModule
{
#if defined(OGL)
    void RasterizeState::setCullingMode(unsigned int cull)
    {
        m_culling = cull;
    }
    unsigned int RasterizeState::getCullingMode()
    {
        return m_culling;
    }
#endif
}