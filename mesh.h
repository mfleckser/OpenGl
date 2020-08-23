#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include "obj_loader.h"

class Vertex {
	public:
		Vertex() {}
		Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(1,1,1)) {
			this->pos = pos;
			this->texCoord = texCoord;
			this->normal = normal;
		}
		
		void Set(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(1,1,1)) {
			this->pos = pos;
			this->texCoord = texCoord;
			this->normal = normal;
		}
		
		inline glm::vec3* GetPos() { return &pos; }
		inline glm::vec2* GetTexCoord() { return &texCoord; }
		inline glm::vec3* GetNormal() { return &normal; }
	protected:
	private:
		glm::vec3 pos;
		glm::vec2 texCoord;
		glm::vec3 normal;
};

class Mesh {
	public:
		Mesh();
		
		void Set(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
		void Set(const std::string& fileName);
		void Draw();
			
		virtual ~Mesh();
	protected:
	private:
		void InitMesh(const IndexedModel& model);
	
		enum {
			POSITION_VB,
			TEXCOORD_VB,
			NORMAL_VB,
			INDEX_VB,
			NUM_BUFFERS
			};
	
		GLuint vertexArrayObject;
		GLuint vertexArrayBuffers[NUM_BUFFERS];
		unsigned int drawCount;
};

#endif
