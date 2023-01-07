#ifndef _TEXTURELOADER_H_
#define _TEXTURELOADER_H_

#include <GL/glew.h>
#include <vector>
#include <string>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

class TextureLoader
{
public:
	static TextureLoader& instance();
    GLuint m_textureID;
	GLuint loadTexture(std::string filename);
	GLuint loadDDS(const char * imagepath);
	GLuint loadCubemap(std::string path, std::string extension);	
	GLuint cubemapTexture;	
	int tWidth, tHeight, tChannels;    
	int m_Error;
	int m_Width, m_Height, m_Channels;

	std::vector<std::string>logtext;
	
private:	
	
	
	
};

#endif



