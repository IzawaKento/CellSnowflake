#pragma once
#include <GL/glew.h>
#include "Program.h"

class CellularAutomata;

class MarchingTetrahedra {
public:
	MarchingTetrahedra(int gridNumX, int gridNumY, int gridNumZ,
		CellularAutomata* cellAutoPtr);
	~MarchingTetrahedra();

	void marchingTetra();
	void marchingHexagonalPrism();
	void marchingHoneycomb();

	void dispatchCompute();

	void drawMesh();	//•`‰æ—pƒNƒ‰ƒX‚ÉˆÚ“®‚µ‚½•û‚ª‚¢‚¢‚©‚à

private:
	GLuint vao;
	GLuint vbo;
	GLuint tetraBuffer;
	GLuint vertexCounterBuffer;

	GLuint vfProgObj = Program::loadProgramObj("src\\marching.vert", "src\\marching.frag");
	GLuint compProgObj = Program::loadCompProgramObj("src\\marching.comp");

	const GLint modelviewLoc = glGetUniformLocation(vfProgObj, "modelview");
	const GLint projectionLoc = glGetUniformLocation(vfProgObj, "projection");

	const int mGridNumX, mGridNumY, mGridNumZ, mVertexNum, mTetraPerHex = 12, mMaxTriPerTetra = 2;

	int vertexCount = -1;

	const CellularAutomata* cellularAutomata;

};