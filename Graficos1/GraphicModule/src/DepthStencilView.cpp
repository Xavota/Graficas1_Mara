#include "DepthStencilView.h"

namespace GraphicsModule
{
#if defined(OGL)
	void DepthStencilView::Init(int width, int height)
	{
		glGenRenderbuffers(1, &m_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	}
	unsigned int DepthStencilView::getID()
	{
		return m_depthBuffer;
	}
#endif
}