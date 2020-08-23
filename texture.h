#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
#include <vector>

class Texture {
	public:
		Texture();
		
		void Set(const std::string& fileName);
		void Bind(unsigned int unit, GLuint program, bool isSky = false, bool isGround = false);
		
		virtual ~Texture();
	protected:
	private:
		GLuint texture;
};

class CubeMap {
	public:
		CubeMap();
		
		void Set(const std::vector<std::string> fileNames);
		void Bind(unsigned int unit, GLuint program);
		
		virtual ~CubeMap();
	protected:
	private:
		GLuint texture;
};

#endif
