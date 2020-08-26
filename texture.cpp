#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>
#include <cassert>
#include <iostream>

Texture::Texture() {}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
}

void Texture::Set(const std::string& fileName) {
	int width, height, numComponents;
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);
	
	if(imageData == NULL) {
		std::cerr << "Texture Loading Failed for Texture: " << fileName << std::endl;
	}
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	
	stbi_image_free(imageData);
}

void Texture::Bind(unsigned int unit, GLuint program, bool isSky) {
	assert(unit >= 0 && unit <= 31);
	
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(GL_TEXTURE0 + unit, texture);
	glUniform1i(glGetUniformLocation(program, "isSky"), isSky);
}

CubeMap::CubeMap() {}

CubeMap::~CubeMap() {
	glDeleteTextures(1, &texture);
}

void CubeMap::Set(const std::vector<std::string> fileNames) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	
	int width, height, numComponents;
	for(unsigned int i = 0; i < fileNames.size(); i++) {
		unsigned char* imageData = stbi_load(fileNames[i].c_str(), &width, &height, &numComponents, 4);
		
		if(imageData) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		} else {
			std::cerr << "Texture Loading Failed for Texture: " << fileNames[i] << std::endl;
		}
		stbi_image_free(imageData);
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void CubeMap::Bind(unsigned int unit, GLuint program) {
	assert(unit >= 0 && unit <= 31);
	
	glActiveTexture(GL_TEXTURE1 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glUniform1i(glGetUniformLocation(program, "skybox"), texture);
}
