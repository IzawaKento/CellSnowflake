#include "CellularAutomata.h"
#include <iostream>
#include "Cell.h"
#include "Program.h"

CellularAutomata::CellularAutomata(float rho, int gridNumX, int gridNumY, int gridNumZ,
	float cellSizeX, float cellSizeZ, float cellSizeY) 
	:cells(new Cell[gridNumX*gridNumY*gridNumZ])
{
	
	for (int i_y = 0; i_y < gridNumY; ++i_y) {
		GLfloat y = i_y * cellSizeY;
		for (int i_z = 0; i_z < gridNumZ; ++i_z) {
			GLfloat z = i_z * cellSizeZ;
			float shiftX = (i_z % 2) * 0.5f * cellSizeX;
			for (int i_x = 0; i_x < gridNumX; ++i_x) {
				GLfloat x = i_x * cellSizeX + shiftX;
				int pointNum = i_x + gridNumX * i_z + i_y * gridNumX * gridNumZ;
				cells[pointNum].SetPosition(x, y, z);
				////cells[pointNum].SetFlagFalse(Cell::CellFlags::ISCRYSTAL);		//なくてもいい
				////cells[pointNum].SetFlagFalse(Cell::CellFlags::ISEDGECRYSTAL);
				//cells[pointNum].SetFlagTrue(CellFlags::ISBOUNDARY);	//Cellのコンストラクタの方がいい？
				////cells[pointNum].SetFlagFalse(Cell::CellFlags::ISEDGEBOUNDARY);
			}
		}
	}
	/*
	//中心点を結晶に
	int centerCellNum = (gridNumX * gridNumY * gridNumZ - 1) / 2;
	cells[centerCellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[centerCellNum].SetFlagTrue(CellFlags::ISEDGECRYSTAL);

	//中心点の周りを結晶に
	SetEdgeCry(centerCellNum + 1);
	SetEdgeCry(centerCellNum - 1);
	SetEdgeCry(centerCellNum + gridNumX);
	SetEdgeCry(centerCellNum - gridNumX);
	//なんかできそうやけど凝ったことはしない
	int cellNumZ = (centerCellNum % (gridNumX * gridNumZ)) / gridNumX;
	if (cellNumZ % 2 == 0) {
		SetEdgeCry(centerCellNum + gridNumX + 1);
		SetEdgeCry(centerCellNum - gridNumX + 1);
	}
	else {
		SetEdgeCry(centerCellNum + gridNumX - 1);
		SetEdgeCry(centerCellNum - gridNumX - 1);
	}
	*/
	
	//Houdiniの隣り合うセル数格納処理は省略

	//確認用頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	//vboとして
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glBufferData(GL_ARRAY_BUFFER,
		gridNumX*gridNumY*gridNumZ * sizeof(Cell), cells, GL_STATIC_DRAW);
	// 結合されている頂点バッファオブジェクトを in 変数から参照できるようにする
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/*
	//シェーダーストレージバッファオブジェクトの作成
	glGenBuffers(1, &ssbo);
	// シェーダストレージバッファオブジェクトを 0 番の結合ポイントに結合する
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	//データを送る
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		gridNumX*gridNumY*gridNumZ * sizeof(Cell), cells, GL_STATIC_DRAW);	//わからんけどとりあえずSTATIC, DRAW
	*/
	

	/*没
	//まとめられそう
	const GLint localSizeXLoc(glGetUniformLocation(computeProgramObj, "localSizeX"));
	const GLint localSizeYLoc(glGetUniformLocation(computeProgramObj, "localSizeY"));
	const GLint localSizeZLoc(glGetUniformLocation(computeProgramObj, "localSizeZ"));
	//ユニフォーム変数に値を入れる
	glUniform1i(localSizeXLoc, gridNumX);
	glUniform1i(localSizeYLoc, gridNumY);
	glUniform1i(localSizeZLoc, gridNumZ);
	*/

	//続きはシェーダで書く？

}

CellularAutomata::~CellularAutomata() {
	delete[] cells;
	//ssbo削除
	glDeleteBuffers(1, &ssbo);
}

//毎フレーム行うコンピュートシェーダの実行
void CellularAutomata::DispatchCompute(int gridNumX, int gridNumY, int gridNumZ) {

	// シェーダストレージバッファオブジェクトを 0 番の結合ポイントに結合する
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	// 更新用のシェーダプログラムの使用開始
	glUseProgram(computeProgramObj);
	//引数は３次元でx, y, zのワークグループを起動する数
	glDispatchCompute(gridNumX *gridNumY * gridNumZ, 1, 1);
}
/*
void CellularAutomata::SetEdgeCry(int cellNum) {
	cells[cellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[cellNum].SetFlagTrue(CellFlags::ISEDGECRYSTAL);
	cells[cellNum].SetFlagFalse(CellFlags::ISBOUNDARY);
	cells[cellNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
	cells[cellNum].SetDiffusionMass(0.0f);
	cells[cellNum].SetBoundaryMass(1.0f);
}
*/

void CellularAutomata::drawCell(int count) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glUseProgram(vertfragProgramObj);
	glDrawArrays(GL_POINTS, 0, count);
}