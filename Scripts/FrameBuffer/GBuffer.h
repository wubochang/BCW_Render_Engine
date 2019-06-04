#pragma once
#ifndef _GBUFFER_H_
#define _GBUFFER_H_

#include "FrameBuffer.h"
#include "Texture.h"


class GBuffer 
{
public:
	GBuffer ();
	~GBuffer ();

	void Initialize();

	Texture* GetGPosition();
	Texture* GetGNormal();
	Texture* GetGAlbedo();
	Texture* GetGMetallicRoughnessAO();

	void BindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	}

	void UnbindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int GetFrameBuffer()
	{
		return m_frameBuffer;
	}


private:

	Texture* m_gPosition;
	Texture* m_gNormal;
	Texture* m_gAlbedo;
	Texture* m_gMetallicRoughnessAO;

	unsigned int m_frameBuffer;
};

#endif // !_GBUFFER_H_
