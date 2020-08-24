#ifndef BLOCK_H
#define BLOCK_H

#include "mesh.h"

class Block {
	public:
		Block(glm::vec3 pos, unsigned short int* texIndicies);
		Block() {};
		
		inline glm::vec3 GetPos() { return glm::vec3(position[0], position[1], position[2]); }
		inline void Set(glm::vec3 pos, unsigned short int* texIndicies) { 
			this->position[0] = pos.x;
			this->position[1] = pos.y;
			this->position[2] = pos.z;
			for(int i = 0; i < 6; i++) {
				this->texIndicies[i] = texIndicies[i];
			}
		}
		
		Mesh* GetMesh(unsigned short int rows, unsigned short int cols);
	protected:
	private:
		short int position[3];
		short int* x = &position[0];
		short int* y = &position[1];
		short int* z = &position[2];
		unsigned short int texIndicies[6];
		std::string block_id;
		
		Mesh vData;
};

#endif
