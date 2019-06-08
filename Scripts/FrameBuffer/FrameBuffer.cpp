#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Initialize()
{
	m_texture = new Texture();
	m_texture->Initialize(GL_RGB, GL_RGB32F, SCR_WIDTH, SCR_HEIGHT, GL_FLOAT, NULL);

	Initialize(m_texture, GL_COLOR_ATTACHMENT0);
}

void FrameBuffer::Initialize(Texture* tex, GLenum attachment)
{
	m_texture = tex;
	m_attatchment = attachment;

	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_texture->GetTextureID(), 0);

	if (attachment == GL_COLOR_ATTACHMENT0)
	{
		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, tex->GetWidth(), tex->GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
	}
	else //if(attachment == GL_DEPTH_ATTACHMENT)
	{
		m_renderBuffer = -1;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		exit(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Initialize(int width, int height)
{
	m_texture = nullptr;
	m_attatchment = GL_COLOR_ATTACHMENT0; // default value

	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	glGenRenderbuffers(1, &m_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
}

void FrameBuffer::Resize(int newWidth, int newHeight)
{
	BindFrameBuffer();
	{
		if (m_texture != nullptr)
		{
			m_texture->ResizeTexture(newWidth, newHeight);
		}

		if (m_attatchment == GL_COLOR_ATTACHMENT0)
		{
			// if render buffer exists
			// for now, there's no render buffer when this frame buffer is used to render depth

			// delete previous render buffer
			glDeleteRenderbuffers(1, &m_renderBuffer);

			// generate a brand new render buffer
			glGenRenderbuffers(1, &m_renderBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
		}
	}
	UnbindFrameBuffer();
}

void FrameBuffer::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
}

void FrameBuffer::UnbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
