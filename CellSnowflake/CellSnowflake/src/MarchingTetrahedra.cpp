#include "MarchingTetrahedra.h"
#include "CellularAutomata.h"
#include "Cell.h"
#include "Vertex.h"
#include "Tetrahedra.h"

MarchingTetrahedra::MarchingTetrahedra(int gridNumX, int gridNumY, int gridNumZ,
	CellularAutomata* ca) 
	: mGridNumX(gridNumX), mGridNumY(gridNumY), mGridNumZ(gridNumZ),
	mVertexNum(gridNumX*gridNumY*gridNumZ), cellularAutomata(ca)
{

	//test
	//適当に初期化
	/*
	Vector4 p = { 0.0, 0.0, 0.0, 1.0 };
	for (int ii = 0; ii < 5880000; ++ii) {
		v[ii].position = p;
	}
	*/

	//メッシュ描画用頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		//セル数 * キューブ一つの最大頂点数
		mGridNumX*mGridNumY*mGridNumZ * 12 * sizeof(Vertex), v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<const Vertex *>(0)->position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<const Vertex *>(0)->color);
	glEnableVertexAttribArray(1);

	//インデックスのバッファオブジェクト
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//セル数 * 三角形の頂点数 * キューブ一つの最大三角数
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mGridNumX*mGridNumY*mGridNumZ * 3 * 5 * sizeof(GLuint), initialIndexBuffer, GL_DYNAMIC_DRAW);


	//コンピュート用ssboを作成
	/*
	glGenBuffers(1, &tetraBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, tetraBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		mGridNumX*mGridNumY*mGridNumZ * sizeof(Tetrahedra), nullptr, GL_DYNAMIC_DRAW);
	*/
	/*
	//アトミックカウンターバッファオブジェクト作成
	glGenBuffers(1, &vertexCounterBuffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vertexCounterBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	*/

	
	//ルックアップテーブル(巨大なため)
	glGenBuffers(1, &triangleConnectionTableBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, triangleConnectionTableBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		4096 * sizeof(GLint), triangleConnectionTable, GL_STATIC_DRAW);	//256 * (3 * 5 + 1)
	

	//ルックアップテーブル(巨大なため)
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Aligned16IntArray) * 4096, &triangleConnectionTableA, GL_STATIC_DRAW);
	//ユニフォームブロックの番号を取得
	GLuint uniformBlockIndex0 = glGetUniformBlockIndex(compProgObj, "TriangleConnectionTableU");
	//その番号に独自の番号を当てる
	glUniformBlockBinding(compProgObj, uniformBlockIndex0, 0);
}

MarchingTetrahedra::~MarchingTetrahedra() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &tetraBuffer);
	glDeleteBuffers(1, &vertexCounterBuffer);
	glDeleteBuffers(1, &triangleConnectionTableBuffer);
	delete[] v;
}

void MarchingTetrahedra::dispatchCompute() {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cellularAutomata->getSsbo());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ibo);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, tetraBuffer);
	//glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 5, vertexCounterBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, triangleConnectionTableBuffer);
	glUseProgram(compProgObj);
	
	//ワークグループはセル数だけ起動する
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	//いるかわからん
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUseProgram(vfProgObj);
	//glDrawArrays(GL_POINTS, 0, 5880000);
	glDrawElements(GL_TRIANGLES, mGridNumX*mGridNumY*mGridNumZ * 15, GL_UNSIGNED_INT, 0);
}