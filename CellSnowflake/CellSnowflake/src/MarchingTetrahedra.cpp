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

	glUseProgram(vfProgObj);
	litposLoc = glGetUniformLocation(vfProgObj, "litpos");
	litambLoc = glGetUniformLocation(vfProgObj, "litamb");
	litdiffLoc = glGetUniformLocation(vfProgObj, "litdiff");
	litspecLoc = glGetUniformLocation(vfProgObj, "litspec");
	//Unifrom変数に値を入れる
	glUniform4fv(litposLoc, 1, Lpos.data());
	glUniform3fv(litambLoc, 1, Lamb.data());
	glUniform3fv(litdiffLoc, 1, Ldiff.data());
	glUniform3fv(litspecLoc, 1, Lspec.data());
	//でばっぐ
	GLfloat vv[4]{};
	glGetUniformfv(vfProgObj, litposLoc, vv);
	std::cout << "litpos:" << vv[3] << std::endl;
	std::cout << "litposLoc:" << litposLoc << std::endl;
	std::cout << "Locmodel:" << glGetUniformLocation(vfProgObj, "modelview") << std::endl;
	std::cout << "Locproj:" << glGetUniformLocation(vfProgObj, "projection") << std::endl;
}

MarchingTetrahedra::~MarchingTetrahedra() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	//glDeleteBuffers(1, &tetraBuffer);
	//glDeleteBuffers(1, &vertexCounterBuffer);
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

void MarchingTetrahedra::drawMesh() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUseProgram(vfProgObj);
	//glDrawArrays(GL_POINTS, 0, 5880000);
	glDrawElements(GL_TRIANGLES, mGridNumX*mGridNumY*mGridNumZ * 15, GL_UNSIGNED_INT, 0);
}