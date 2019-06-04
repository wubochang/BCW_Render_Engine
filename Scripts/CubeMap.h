#ifndef _CUBE_MAP_H_
#define _CUBE_MAP_H_

#include "glad_include.h"
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

class Texture;
class CubeMap
{
public:
	CubeMap();
	~CubeMap();

	bool Initialize(std::string folderPath, std::vector<std::string> fileNames);
	bool Initialize(int width, int height);

	void SetBuffers(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	}

	unsigned int GetWidth() { return m_width; }
	unsigned int GetHeight() { return m_height; }
	unsigned int GetTextureID() { return m_textureID; }

	void EnableMipMap();

private:

	bool LoadCubeMap(std::vector<std::string> facesPaths);

	unsigned int m_textureID;

	unsigned int m_width;
	unsigned int m_height;

};

#endif // !_CUBE_MAP_H_
