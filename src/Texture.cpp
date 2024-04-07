#include "Texture.h"

Texture::Texture(const std::string& filename) :
	filename(filename),
	needsTexture(true),
	texture(0)
{}

void Texture::loadFromFile(){
	int width = 0;
	int height = 0;
	int channels = 0;
	if(this->needsTexture){
		glGenTextures(1, &this->texture);
		this->needsTexture = false;
	}
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char* data = stbi_load(this->filename.c_str(), &width, &height, &channels, 0);
	if(data){
		switch(channels){
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		}
		stbi_image_free(data);
	}
}

void Texture::bindToTextureUnit(const GLenum texture){
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);
}

Texture::~Texture(){}
