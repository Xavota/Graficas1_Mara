#include "RenderTargetView.h"
#include <iostream>

namespace GraphicsModule
{
#if defined(OGL)
    void RenderTargetView::Init(unsigned int textureAtchID, unsigned int id)
	{
        if (id != 1234)
        {
            m_frameBuffer = id;
        }
        else
        {
		    glGenFramebuffers(1, &m_frameBuffer);
        }
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

        m_textureAtatchedID = textureAtchID;
    }
    unsigned int RenderTargetView::getID()
    {
        return m_frameBuffer;
    }
    unsigned int RenderTargetView::getTextureAtatchedID()
    {
        return m_textureAtatchedID;
    }
#endif
}