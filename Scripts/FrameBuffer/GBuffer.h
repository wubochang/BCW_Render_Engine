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

	void Resize(int newWidth, int newHeight);

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

	Texture* m_textures[4];

	unsigned int m_frameBuffer;
	unsigned int m_renderBuffer;

};

#endif // !_GBUFFER_H_
