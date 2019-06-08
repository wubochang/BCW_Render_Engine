#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include "Texture.h"
#include "GLWindow.h"


class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Initialize();
	void Initialize(Texture* tex, GLenum attachment);
	void Initialize(int width, int height);

	void Resize(int newWidth, int newHeight);

	void BindFrameBuffer();
	void UnbindFrameBuffer();

	Texture* GetTexture()
	{
		return m_texture;
	}

	int GetFrameBuffer()
	{
		return m_frameBuffer;
	}

protected:

	Texture* m_texture;
	unsigned int m_frameBuffer;
	unsigned int m_renderBuffer;
	GLenum m_attatchment;

};

#endif // !_FRAME_BUFFER_H_
