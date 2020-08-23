#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include "transform.h"
#include "camera.h"

class Shader {
	public:
		Shader();
		
		void Set(const std::string& fileName);
		void Bind();
		void Update(const Camera& camera, const Transform transform = Transform());
		void UpdateLighting(Camera& camera);
		
		inline GLuint GetProgram() { return program; }
		
		virtual ~Shader();
	protected:
	private:
		static const unsigned int NUM_SHADERS = 2;
		
		enum {
			TRANSFORM_U,
			NUM_UNIFORMS
		};
		
		GLuint program;
		GLuint shaders[NUM_SHADERS];
		GLuint uniforms[NUM_UNIFORMS];
};

#endif
