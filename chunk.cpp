#include "chunk.h"

Chunk::Chunk() {
	unsigned char block_type;
	for(int i = 0; i < 16; i++) {
		for(int j = 0; j < 16; j++) {
			for(int k = 0; k < 16; k++) {
				if(j == 15) block_type = 2;
				else block_type = 1;
				this->blocks[i][j][k] = block_type;
			}
		}
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

unsigned short int texIndicies[3][6] = {
	{}, // Air
	{ 0, 0, 0, 0, 0, 0 }, // Dirt
	{ 1, 0, 2, 2, 2, 2 }, // Grass
};

Vertex GetVertex(int i, int j, int k, unsigned short int side, unsigned short int index, unsigned short int texIndex) {
	int x = i + offset[side][index][0];
	int y = j + offset[side][index][1];
	int z = k + offset[side][index][2];
	unsigned short int texX, texY;
	texX = texIndex % TEX_WIDTH;
	texY = ((TEX_HEIGHT * TEX_WIDTH) - texIndex - 3) / TEX_WIDTH;
	return Vertex(glm::vec3(x, y, z), glm::vec2((float)(texX + texOffset[index][0]) / TEX_WIDTH, (float)(texY + texOffset[index][1]) / TEX_HEIGHT));
}

void AddFace(int i, int j, int k, unsigned short int side, std::vector<Vertex>* verticies, std::vector<unsigned int>* indicies, unsigned short int texIndex) {
	unsigned short int indexOffsets[6] = { 4, 3, 2, 4, 2, 1 };
	for(int x = 0; x < 4; x++) {
		verticies->push_back(GetVertex(i, j, k, side, x, texIndex));
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
				//std::cout << block_id;
				if(j != 15) { // positive Y
					if(!blocks[i][j + 1][k]) {
						AddFace(i, j, k, 0, &verticies, &indicies, texIndicies[block_id][0]);
					}
				} else {
					AddFace(i, j, k, 0, &verticies, &indicies, texIndicies[block_id][0]);
				}
				if(j != 0) { // negative Y
					if(!blocks[i][j - 1][k]) {
						AddFace(i, j, k, 1, &verticies, &indicies, texIndicies[block_id][1]);
					}
				} else {
					AddFace(i, j, k, 1, &verticies, &indicies, texIndicies[block_id][1]);
				}
				if(i != 15) { // positive X
					if(!blocks[i + 1][j][k]) {
						AddFace(i, j, k, 2, &verticies, &indicies, texIndicies[block_id][2]);
					}
				} else {
					AddFace(i, j, k, 2, &verticies, &indicies, texIndicies[block_id][2]);
				}
				if(i != 0) { // negative X
					if(!blocks[i - 1][j][k]) {
						AddFace(i, j, k, 3, &verticies, &indicies, texIndicies[block_id][3]);
					}
				} else {
					AddFace(i, j, k, 3, &verticies, &indicies, texIndicies[block_id][3]);
				}
				if(k != 15) { // positive Z
					if(!blocks[i][j][k + 1]) {
						AddFace(i, j, k, 4, &verticies, &indicies, texIndicies[block_id][4]);
					}
				} else {
					AddFace(i, j, k, 4, &verticies, &indicies, texIndicies[block_id][4]);
				}
				if(k != 0) { // negative Z
					if(!blocks[i][j][k - 1]) {
						AddFace(i, j, k, 5, &verticies, &indicies, texIndicies[block_id][5]);
					}
				} else {
					AddFace(i, j, k, 5, &verticies, &indicies, texIndicies[block_id][5]);
				}
			}
		}
	}
	vData->Set(&verticies[0], verticies.size(), &indicies[0], indicies.size());
}
