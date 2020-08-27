#include "chunk.h"

unsigned short int textures[1][6] = {
	{ 0, 0, 0, 0, 0, 0 }
};

Chunk::Chunk() {
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 16; j++) {
			for(int k = 0; k < 16; k++) {
				this->blocks[i][j][k] = 0;
			}
		}
	}
	for(int i = 0; i < 16; i++) {
			this->blocks[i][0][1] = 1;
	}
}

unsigned short int offset[6][4][3] = {
	{ {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1} },
	{ {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1} },
	{ {1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1} },
	{ {0, 0, 1}, {0, 1, 1}, {0, 1, 0}, {0, 0, 0} },
	{ {1, 0, 1}, {1, 1, 1}, {0, 1, 1}, {0, 0, 1} },
	{ {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 0} }
};

unsigned short int texOffset[4][2] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 1 },
	{ 1, 0 }
};

unsigned short int texIndicies[1][6] = {
	{ 0, 0, 0, 0, 0, 0 }, // Dirt
};

Vertex GetVertex(int i, int j, int k, unsigned short int side, unsigned short int index, unsigned short int block_id) {
	int x = i + offset[side][index][0];
	int y = j + offset[side][index][1];
	int z = k + offset[side][index][2];
	unsigned short int texX, texY;
	texX = (block_id - 1) % TEX_WIDTH;
	texY = ((TEX_HEIGHT * TEX_WIDTH) - block_id - 2) / TEX_WIDTH;
	return Vertex(glm::vec3(x, y, z), glm::vec2((float)(texX + texOffset[index][0]) / TEX_WIDTH, (float)(texY + texOffset[index][1]) / TEX_HEIGHT));
}

void AddFace(int i, int j, int k, unsigned short int side, std::vector<Vertex>* verticies, std::vector<unsigned int>* indicies, unsigned short int block_id) {
	unsigned short int indexOffsets[6] = { 4, 3, 2, 4, 2, 1 };
	for(int x = 0; x < 4; x++) {
		verticies->push_back(GetVertex(i, j, k, side, x, block_id));
	}
	for(int x = 0; x < 6; x++) {
		indicies->push_back(verticies->size() - indexOffsets[x]);
	}
}

void Chunk::SetMesh() {
	std::vector<Vertex> verticies;
	std::vector<unsigned int> indicies;
	
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 16; j++) {
			for(int k = 0; k < 16; k++) {
				unsigned short int block_id = blocks[i][j][k];
				if(block_id == 0) {
					continue;
				}
				std::cout << block_id;
				if(j != 15) { // positive Y
					if(!blocks[i][j + 1][k]) {
						AddFace(i, j, k, 0, &verticies, &indicies, block_id);
					}
				} else {
					AddFace(i, j, k, 0, &verticies, &indicies, block_id);
				}
				if(j != 0) { // negative Y
					if(!blocks[i][j - 1][k]) {
						AddFace(i, j, k, 1, &verticies, &indicies, block_id);
					}
				} else {
					AddFace(i, j, k, 1, &verticies, &indicies, block_id);
				}
				if(i != 15) { // positive X
					if(!blocks[i + 1][j][k]) {
						AddFace(i, j, k, 2, &verticies, &indicies, block_id);
					}
				} else {
					AddFace(i, j, k, 2, &verticies, &indicies, block_id);
				}
				if(i != 0) { // negative X
					if(!blocks[i - 1][j][k]) {
						AddFace(i, j, k, 3, &verticies, &indicies, block_id);
					}
				} else {
					AddFace(i, j, k, 3, &verticies, &indicies, block_id);
				}
				if(k != 15) { // positive Z
					if(!blocks[i][j][k + 1]) {
						AddFace(i, j, k, 4, &verticies, &indicies, block_id);
					}
				} else {
					AddFace(i, j, k, 4, &verticies, &indicies, block_id);
				}
				if(k != 0) { // negative Z
					if(!blocks[i][j][k - 1]) {
						AddFace(i, j, k, 5, &verticies, &indicies, block_id);
					}
				} else {
					AddFace(i, j, k, 5, &verticies, &indicies, block_id);
				}
			}
		}
	}
	std::cout << "HI";
	vData->Set(&verticies[0], verticies.size(), &indicies[0], indicies.size());
}
