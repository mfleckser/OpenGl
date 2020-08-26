#ifndef CHUNK_H
#define CHUNK_H

#ifndef TEX_WIDTH
#define TEX_WIDTH 5
#endif

#ifndef TEX_HEIGHT
#define TEX_HEIGHT 5
#endif

#include "mesh.h"
#include <iostream>
#include <vector>

class Chunk {
	public:
		Chunk();
		
		void SetMesh();
		
		inline Mesh* GetMesh() { return vData; }
		
		inline unsigned char* GetBlocks() { return &blocks[0][0][0]; }
	private:
	protected:
		int position[3];
		int *x = &position[0];
		int *y = &position[1];
		int *z = &position[2];
		unsigned char blocks[16][16][16];
		Mesh m;
		Mesh* vData = &m;
};

#endif
