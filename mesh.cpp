#include "mesh.h"
#include "obj_loader.h"
#include <vector>
#include <GL/glew.h>
#include <iostream>

using namespace std;

Mesh::Mesh() {}

void Mesh::Set(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices) {
	
	IndexedModel model;
	
	for(unsigned int i = 0; i < numVertices; i++) {
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}
	
	for(unsigned int i = 0; i < numIndices; i++) {
		model.indices.push_back(indices[i]);
	}
	
	InitMesh(model);
}

void Mesh::Set(const std::string& fileName) {
	IndexedModel model = OBJModel(fileName).ToIndexedModel();
	InitMesh(model);
}

void Mesh::InitMesh(const IndexedModel& model) {
	drawCount = model.indices.size();
	
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	
	// Position Buffer
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.indices.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
	// texCoords Buffer for glm::vec2D position on texture image file
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.indices.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::Draw() {
	glBindVertexArray(vertexArrayObject);
	
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
