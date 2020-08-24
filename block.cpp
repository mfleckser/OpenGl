#include "block.h"

bool offset[6][4][3] = {
	{ {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1} },
	{ {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1} },
	{ {0, 0, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1} },
	{ {1, 0, 1}, {1, 1, 1}, {1, 1, 0}, {1, 0, 0} },
	{ {1, 0, 1}, {1, 1, 1}, {0, 1, 1}, {0, 0, 1} },
	{ {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 0} }
};

unsigned int texOffset[4][2] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 1 },
	{ 1, 0 }
};

unsigned int indicies[36] = {
	0, 1, 2, 0, 2, 3,
	4, 5, 6, 4, 6, 7,
	8, 9, 10, 8, 10, 11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20, 21, 22, 20, 22, 23
};

Block::Block(glm::vec3 pos, unsigned short int* texIndicies) {
	this->position[0] = pos.x;
	this->position[1] = pos.y;
	this->position[2] = pos.z;
	for(int i = 0; i < 6; i++) {
		this->texIndicies[i] = texIndicies[i];
	}
}

Mesh* Block::GetMesh(unsigned short int rows, unsigned short int cols) {
	Vertex verticies[24];
	
	for(unsigned int i = 0; i < 6; i++) {
		unsigned short int texX, texY;
		texX = texIndicies[i] % cols;
		texY = ((rows * cols) - texIndicies[i] - 1) / cols;
		for(unsigned int j = 0; j < 4; j++) {
			verticies[j + 4 * i].Set(glm::vec3(*x + offset[i][j][0], *y + offset[i][j][1], *z + offset[i][j][2]), glm::vec2((float)(texX + texOffset[j][0])/cols, (float)(texY + texOffset[j][1])/rows));
		}
	}
	
	vData.Set(&verticies[0], 24, &indicies[0], 36);
	
	return &vData;
}
