#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
class Texture {
	public:
		Texture(const std::string& filename);

		void loadFromFile();
		void bindToTextureUnit(const GLenum texture);

		~Texture();

	private:
		const std::string& filename;
		bool needsTexture;
		GLuint texture;
};
#endif // TEXTURE_H
