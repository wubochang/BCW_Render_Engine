#include "GBuffer.h"

GBuffer::GBuffer()
{
}

GBuffer::~GBuffer()
{
}

void GBuffer::Initialize()
{
	glGenFramebuffers(1, &m_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	// - Position color buffer
	m_gPosition = new Texture();
	m_gPosition->Initialize(GL_RGB, GL_RGB32F, SCR_WIDTH, SCR_HEIGHT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition->GetTextureID(), 0);
	m_gPosition->SetParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_gPosition->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - Normal color buffer
	m_gNormal= new Texture();
	m_gNormal->Initialize(GL_RGB, GL_RGB32F, SCR_WIDTH, SCR_HEIGHT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal->GetTextureID(), 0);
	m_gNormal->SetParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_gNormal->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - Color + Specular color buffer
	m_gAlbedo= new Texture();
	m_gAlbedo->Initialize(GL_RGB, GL_RGB32F, SCR_WIDTH, SCR_HEIGHT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedo->GetTextureID(), 0);
	m_gAlbedo->SetParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_gAlbedo->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_gMetallicRoughnessAO = new Texture();
	m_gMetallicRoughnessAO->Initialize(GL_RGB, GL_RGB32F, SCR_WIDTH, SCR_HEIGHT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gMetallicRoughnessAO->GetTextureID(), 0);
	m_gMetallicRoughnessAO->SetParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_gMetallicRoughnessAO->SetParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	// - Create and attach depth buffer (renderbuffer)
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	// - Finally check if framebuffer is complete
	auto res = (glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (res != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "GBuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture* GBuffer::GetGPosition()
{
	return m_gPosition;
}

Texture* GBuffer::GetGNormal()
{
	return m_gNormal;
}

Texture*GBuffer::GetGAlbedo()
{
	return m_gAlbedo;
}

Texture * GBuffer::GetGMetallicRoughnessAO()
{
	return m_gMetallicRoughnessAO;
}