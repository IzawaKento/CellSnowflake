#include "MarchingTetrahedra.h"
#include "CellularAutomata.h"
#include "Vertex.h"
#include "Tetrahedra.h"

MarchingTetrahedra::MarchingTetrahedra(int gridNumX, int gridNumY, int gridNumZ,
	CellularAutomata* ca) 
	: mGridNumX(gridNumX), mGridNumY(gridNumY), mGridNumZ(gridNumZ),
	mVertexNum(gridNumX*gridNumY*gridNumZ), cellularAutomata(ca)
{
	//メッシュ描画用頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		//セル数 * 24個も作るんか…
		mGridNumX*mGridNumY*mGridNumZ * mTetraPerHex * mMaxTriPerTetra * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<const Vertex *>(0)->position);
	glEnableVertexAttribArray(0);

	//コンピュート用ssboを作成
	glGenBuffers(1, &tetraBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, tetraBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		mGridNumX*mGridNumY*mGridNumZ * sizeof(Tetrahedra), nullptr, GL_DYNAMIC_DRAW);

	//アトミックカウンターバッファオブジェクト作成
	glGenBuffers(1, &vertexCounterBuffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vertexCounterBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
}

MarchingTetrahedra::~MarchingTetrahedra() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &tetraBuffer);
	glDeleteBuffers(1, &vertexCounterBuffer);
}

void MarchingTetrahedra::dispatchCompute() {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cellularAutomata->getSsbo());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, tetraBuffer);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 5, vertexCounterBuffer);
	glUseProgram(compProgObj);
	//ワークグループはセル数だけ起動する
	glDispatchCompute(mGridNumX * mGridNumY * mGridNumZ, 1, 1);
	//いるかわからん、一応
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

//未使用
void MarchingTetrahedra::marchingHoneycomb() {
	for (int iX = 0; iX < mGridNumX; iX++) {
		for (int iY = 0; iY < mGridNumY; iY++) {
			for (int iZ = 0; iZ < mGridNumZ; iZ++)
			{
				marchingHexagonalPrism();
			}
		}
	}
	if(vertexCount > 0)
		drawMesh();

}

void MarchingTetrahedra::marchingHexagonalPrism() {

}

void MarchingTetrahedra::marchingTetra() {

}

void MarchingTetrahedra::drawMesh() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glUseProgram(vfProgObj);
	glDrawArrays(GL_POINTS, 0, vertexCount);
}