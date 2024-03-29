#include "shader.h"
#include <iostream>
#include <fstream>

#ifndef WIDTH
#define WIDTH 640
#endif
#ifndef HEIGHT
#define HEIGHT 480
#endif

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader() {}

Shader::~Shader() {
	for(unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(program);
}

void Shader::Set(const std::string& fileName) {
	program = glCreateProgram();
	shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
	
	for(unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(program, shaders[i]);
	}
	
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");
	glBindAttribLocation(program, 2, "normal");
	
	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Program linking failed: ");
	
	glValidateProgram(program);
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
	
	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform");
	
	int widthLoc = glGetUniformLocation(program, "width");
	int heightLoc = glGetUniformLocation(program, "height");
	
	glUniform1i(widthLoc, WIDTH);
	glUniform1i(heightLoc, HEIGHT);
}

void Shader::Bind() {
	glUseProgram(program);
}

void Shader::Update(const Camera& camera, const Transform transform) {
	glm::mat4 model_projection_view = camera.GetViewProjection() * transform.GetModel();
	
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GL_FALSE, &model_projection_view[0][0]);
}

void Shader::UpdateLighting(Camera& camera) {
	int loc = glGetUniformLocation(program, "lightDirection");
	glm::vec3 val = camera.GetForward();
	
	glUniform3fv(loc, 3, &val[0]);
}

static GLuint CreateShader(const std::string& text, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	
	if(shader == 0) {
		std::cerr << "Error: Shader creation failed." << std::endl;
	}
	
	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringsLength[1];
	
	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringsLength[0] = text.length();
	
	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringsLength);
	glCompileShader(shader);
	
	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");
	
	return shader;
}

static std::string LoadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());
	
	std::string output;
	std::string line;
	
	if(file.is_open()) {
		while(file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	} else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}
	return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };
	
	if(isProgram) {
		glGetProgramiv(shader, flag, &success);
	} else {
		glGetShaderiv(shader, flag, &success);
	}
	
	if(success == GL_FALSE) {
		if(isProgram) {
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		} else {
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}
		
		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}
