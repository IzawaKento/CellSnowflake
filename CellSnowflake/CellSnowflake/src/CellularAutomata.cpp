#include "CellularAutomata.h"
#include <iostream>
#include <random>
#include "Cell.h"
//#include "Program.h"

CellularAutomata::CellularAutomata(float rho, int gridNumX, int gridNumY, int gridNumZ,
	float cellSizeX, float cellSizeZ, float cellSizeY, GLuint vfProgObj)
	:cells(new Cell[gridNumX*gridNumY*gridNumZ]), mGridNumX(gridNumX), mGridNumY(gridNumY), mGridNumZ(gridNumZ)
	, mvfProgObj(vfProgObj)
{	/*
	std::cout << "sizeofCell:" << sizeof(Cell) << std::endl;
	std::cout << "sizeofGLboolean:" << sizeof(GLboolean) << std::endl;
	std::cout << "cells[0].pos:   " << &cells[0].position << std::endl;
	std::cout << "cells[0].pos[1]:" << &cells[0].position[1] << std::endl;
	std::cout << "cells[0].pos[2]:" << &cells[0].position[2] << std::endl;
	std::cout << "cells[0].pos[3]:" << &cells[0].position[3] << std::endl;
	std::cout << "cells[0].col:   " << &cells[0].color << std::endl;
	std::cout << "cells[0].col[1]:" << &cells[0].color[1] << std::endl;
	std::cout << "cells[0].col[2]:" << &cells[0].color[2] << std::endl;
	std::cout << "cells[0].col[3]:" << &cells[0].color[3] << std::endl;
	std::cout << "cells[0].fla:   " << &cells[0].flags << std::endl;
	std::cout << "cells[0].dif:   " << &cells[0].diffusionMass << std::endl;
	std::cout << "cells[0].bou:   " << &cells[0].boundaryMass << std::endl;
	std::cout << "cells[0].hori:  " << &cells[0].horizontalNeighbourNum << std::endl;
	std::cout << "cells[0].ver:   " << &cells[0].verticalNeighbourNum << std::endl;

	std::cout << "sizeofcells:" << sizeof(*cells) << std::endl;
	std::cout << "cells[0].pos:   " << &cells[1].position << std::endl;
	std::cout << "cells[0].pos[1]:" << &cells[1].position[1] << std::endl;
	std::cout << "cells[0].pos[2]:" << &cells[1].position[2] << std::endl;
	std::cout << "cells[0].pos[3]:" << &cells[1].position[3] << std::endl;
	std::cout << "cells[0].col:   " << &cells[1].color << std::endl;
	std::cout << "cells[0].col[1]:" << &cells[1].color[1] << std::endl;
	std::cout << "cells[0].col[2]:" << &cells[1].color[2] << std::endl;
	std::cout << "cells[0].col[3]:" << &cells[1].color[3] << std::endl;
	std::cout << "cells[0].fla:   " << &cells[1].flags << std::endl;
	std::cout << "cells[0].dif:   " << &cells[1].diffusionMass << std::endl;
	std::cout << "cells[0].bou:   " << &cells[1].boundaryMass << std::endl;
	std::cout << "cells[0].hori:  " << &cells[1].horizontalNeighbourNum << std::endl;
	std::cout << "cells[0].ver:   " << &cells[1].verticalNeighbourNum << std::endl;
	*/
	for (int i_y = 0; i_y < gridNumY; ++i_y) {
		GLfloat y = i_y * cellSizeY;
		for (int i_z = 0; i_z < gridNumZ; ++i_z) {
			GLfloat z = i_z * cellSizeZ;
			float shiftX = (i_z % 2) * 0.5f * cellSizeX;
			GLuint zIsOdd = i_z % 2;
			for (int i_x = 0; i_x < gridNumX; ++i_x) {
				GLfloat x = i_x * cellSizeX +shiftX;
				int pointNum = i_x + gridNumX * i_z + i_y * gridNumX * gridNumZ;
				cells[pointNum].SetPosition(x, y, z);
				//ここ複雑になってる
				cells[pointNum].SetFlagTrue(CellFlags::MZISODD * zIsOdd);
				
				/*没、未使用
				//Zが奇数なら*2+1、偶数なら*2。1との論理積
				if (getCellNumZ(pointNum) && 0b1) {
					cells[pointNum].hexMapNum = pointNum * 2 + 1;
				}
				else {
					cells[pointNum].hexMapNum = pointNum * 2;
				}
				*/

				cells[pointNum].SetFlagFalse(CellFlags::ISCRYSTAL);		//なくてもいい
				cells[pointNum].SetFlagFalse(CellFlags::ISEDGECRYSTAL);
				cells[pointNum].SetFlagTrue(CellFlags::ISBOUNDARY);	//Cellのコンストラクタの方がいい？
				cells[pointNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
				cells[pointNum].diffusionMass = rho;
				cells[pointNum].boundaryMass = 0.0f;

				//端っこなら
				if (i_y == 0 || i_y == gridNumY - 1
					|| i_z == 0 || i_z == gridNumZ - 1
					|| i_x == 0 || i_x == gridNumX - 1) {
					cells[pointNum].SetFlagTrue(CellFlags::ISENDOFCELLS);

				}
				else {
					cells[pointNum].SetFlagFalse(CellFlags::ISENDOFCELLS);
				}
			}
		}
	}
	
	//中心初期セル作成
	int centerCellNum = (mGridNumX / 2) + (mGridNumZ / 2 * mGridNumX) + ((mGridNumY / 2 ) * mGridNumX * mGridNumZ);
	setInitialCells(centerCellNum);
	//setInitialCells(centerCellNum + 156);
	//Houdiniの隣り合うセル数格納処理は省略

	//確認用頂点配列オブジェクト
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	//vboとして
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glBufferData(GL_ARRAY_BUFFER,
		gridNumX*gridNumY*gridNumZ * sizeof(Cell), cells, GL_DYNAMIC_DRAW);

	// 結合されている頂点バッファオブジェクトを in 変数から参照できるようにする
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->color);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->flags);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->neighbourSurfaceDir);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->vertexNormal);
	glEnableVertexAttribArray(4);
	
	//書き込み用SSBO作成
	glGenBuffers(1, &tmpSsbo);
	//結合ポイントは1番でいいのか？
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		gridNumX*gridNumY*gridNumZ * sizeof(Cell), cells, GL_DYNAMIC_DRAW);

	//アトミックカウンターバッファオブジェクト作成
	glGenBuffers(1, &acbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

	//マーチングキューブ用
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(TriangleConnectionTable), &tct, GL_STATIC_DRAW);
	//ユニフォームブロックの番号を取得
	GLuint uniformBlockIndex0 = glGetUniformBlockIndex(mvfProgObj, "TriangleConnectionTable");
	//その番号に独自の番号を当てる
	glUniformBlockBinding(mvfProgObj, uniformBlockIndex0, 0);
}

CellularAutomata::~CellularAutomata() {
	delete[] cells;
	glDeleteVertexArrays(1, &vao);
	//ssbo削除
	glDeleteBuffers(1, &ssbo);
	glDeleteBuffers(1, &tmpSsbo);
	//アトミックカウンター削除
	glDeleteBuffers(1, &acbo);
	glDeleteBuffers(1, &ubo);

}

//直径３
void CellularAutomata::set2RadiusCells(int centerCellNum, int yOffset) {
	int tmpY = mGridNumX * mGridNumZ;
	//中心
	SetEdgeCry(centerCellNum - tmpY * yOffset);
	//周り
	SetEdgeCry(centerCellNum + 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum - 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum + mGridNumX - tmpY * yOffset);
	SetEdgeCry(centerCellNum - mGridNumX - tmpY * yOffset);
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	SetEdgeCry(centerCellNum + mGridNumX + zOddNum - tmpY * yOffset);
	SetEdgeCry(centerCellNum - mGridNumX + zOddNum - tmpY * yOffset);
}
//直径５
void CellularAutomata::set3RadiusCells(int centerCellNum, int yOffset) {
	int tmpY = mGridNumX * mGridNumZ;
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	set2RadiusCells(centerCellNum, yOffset);
	//周り
	SetEdgeCry(centerCellNum + 2 - tmpY * yOffset);
	SetEdgeCry(centerCellNum - 2 - tmpY * yOffset);
	SetEdgeCry(centerCellNum + mGridNumX - zOddNum - tmpY * yOffset);
	SetEdgeCry(centerCellNum - mGridNumX - zOddNum - tmpY * yOffset);
	SetEdgeCry(centerCellNum + mGridNumX + (zOddNum * 2) - tmpY * yOffset);
	SetEdgeCry(centerCellNum - mGridNumX + (zOddNum * 2) - tmpY * yOffset);
	SetEdgeCry(centerCellNum + (mGridNumX * 2) - tmpY * yOffset);
	SetEdgeCry(centerCellNum - (mGridNumX * 2) - tmpY * yOffset);
	SetEdgeCry(centerCellNum + (mGridNumX * 2) + 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum - (mGridNumX * 2) + 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum + (mGridNumX * 2) - 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum - (mGridNumX * 2) - 1 - tmpY * yOffset);
}

void CellularAutomata::setInitialCells(int centerCellNum) {
	//頂点を結晶に
	/*
	cells[centerCellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[centerCellNum].SetFlagFalse(CellFlags::ISEDGECRYSTAL);
	cells[centerCellNum].SetFlagFalse(CellFlags::ISBOUNDARY);
	cells[centerCellNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
	cells[centerCellNum].diffusionMass = 0.0f;
	cells[centerCellNum].boundaryMass = 1.0f;
	*/
	
	/*
	//直径３
	SetEdgeCry(centerCellNum + 1);
	SetEdgeCry(centerCellNum - 1);
	//いいのかわからん
	SetEdgeCry(centerCellNum + mGridNumX);
	SetEdgeCry(centerCellNum - mGridNumX);
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	SetEdgeCry(centerCellNum + mGridNumX + zOddNum);
	SetEdgeCry(centerCellNum - mGridNumX + zOddNum);
	//上下も
	SetEdgeCry(centerCellNum + mGridNumX * mGridNumZ);
	SetEdgeCry(centerCellNum - mGridNumX * mGridNumZ);
	*/
	
	
	int tmpY = mGridNumX * mGridNumZ;
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	
	////てすと用
	//for (int i = 0; i < 16; ++i) {
	//	for (int j = 0; j < 10; ++j) {
	//		for (int k = 0; k < 12; ++k) {
	//			SetEdgeCry(centerCellNum + k + mGridNumX * j + mGridNumX*mGridNumZ * i);
	//		}
	//	}
	//}
	

	
	SetEdgeCry(centerCellNum);
	set2RadiusCells(centerCellNum, 1);
	set3RadiusCells(centerCellNum, 2);
	set3RadiusCells(centerCellNum, 3);
	set3RadiusCells(centerCellNum, 4);
	set2RadiusCells(centerCellNum, 5);
	SetEdgeCry(centerCellNum - tmpY * 6);
	

	//以下不要
	////ifはつかってないけどめっちゃくそやと思う
	//SetEdgeCry((cells[centerCellNum].hexMapNum + gridNumX * 2 + 1) / 2);
	//SetEdgeCry((cells[centerCellNum].hexMapNum + gridNumX * 2 - 1) / 2);
	//SetEdgeCry((cells[centerCellNum].hexMapNum - gridNumX * 2 + 1) / 2);
	//SetEdgeCry((cells[centerCellNum].hexMapNum - gridNumX * 2 - 1) / 2);

	//過去
	//SetEdgeCry(centerCellNum + gridNumX);
	//SetEdgeCry(centerCellNum - gridNumX);
	////なんかできそうやけど凝ったことはしない
	//int cellNumZ = getCellNumZ(centerCellNum);
	//if (!(cellNumZ && 0b1)) {
	//	SetEdgeCry(centerCellNum + gridNumX - 1);
	//	SetEdgeCry(centerCellNum - gridNumX - 1);
	//}
	//else {
	//	SetEdgeCry(centerCellNum + gridNumX + 1);
	//	SetEdgeCry(centerCellNum - gridNumX + 1);
	//}
}

void CellularAutomata::copySSBO(GLuint readBuffer, GLuint writeBuffer) {
	//SSBOをコピー
	//SSBOのままでは引数的にコピーできない
	glBindBuffer(GL_COPY_READ_BUFFER, readBuffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, writeBuffer);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
		0, 0, mGridNumX*mGridNumY*mGridNumZ * sizeof(Cell));
	//解放
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

//毎フレーム行うコンピュートシェーダの実行
void CellularAutomata::DispatchCompute(int gridNumX, int gridNumY, int gridNumZ) {

	//resetEdge
	// シェーダストレージバッファオブジェクトを 0 番の結合ポイントに結合する
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	// 書き込み用SSBOを 1 番の結合ポイントに結合する
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	//アトミックカウンターを 2 番の結合ポイントに結合する
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	// 更新用のシェーダプログラムの使用開始
	glUseProgram(resetEdgeComProgObj);
	//引数は３次元でx, y, zのワークグループを起動する数
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	//メモリへのアクセスを止める
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//更新後SSBOを読み取り用SSBOにコピー
	copySSBO(tmpSsbo, ssbo);

	//setBoundary
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(computeProgramObj);
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//更新後SSBOを読み取り用SSBOにコピー
	copySSBO(tmpSsbo, ssbo);

	//setNeighbourCryNum
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(neighbourCryNumComProgObj);
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//更新後SSBOを読み取り用SSBOにコピー
	copySSBO(tmpSsbo, ssbo);

	//diffusion1
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(diffusion1ComProgObj);
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//更新後SSBOを読み取り用SSBOにコピー
	copySSBO(tmpSsbo, ssbo);

	/*
	//未使用diffusion2
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glUseProgram(diffusion2ComProgObj);
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//更新後SSBOを読み取り用SSBOにコピー
	copySSBO(tmpSsbo, ssbo);
	*/
	
	//freezing
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(freezingComProgObj);
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//更新後SSBOを読み取り用SSBOにコピー
	copySSBO(tmpSsbo, ssbo);
	
	//attachment
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(attachmentComProgObj);
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//更新後SSBOを読み取り用SSBOにコピー
	copySSBO(tmpSsbo, ssbo);
	
	//melting
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(meltingComProgObj);
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);	//なくていいかも
	//更新後SSBOを読み取り用SSBOにコピー
	copySSBO(tmpSsbo, ssbo);
	
}

void CellularAutomata::SetEdgeCry(int cellNum) {
	cells[cellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[cellNum].SetFlagTrue(CellFlags::ISEDGECRYSTAL);
	cells[cellNum].SetFlagFalse(CellFlags::ISBOUNDARY);
	cells[cellNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
	cells[cellNum].diffusionMass = 0.0f;
	cells[cellNum].boundaryMass = 1.0f;
}


void CellularAutomata::drawCell(int count, GLuint vfProgObj) {
	
	/*Cell* dc(static_cast<Cell*>(glMapNamedBufferEXT(ssbo, GL_WRITE_ONLY)));
	for (int i = 0; i < count; ++i)
	{
		dc[i].flags = cells[i].flags;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);*/

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
	glUseProgram(vfProgObj);
	glDrawArrays(GL_POINTS, 0, count);
}

void CellularAutomata::initialize() {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glNamedBufferSubDataEXT(ssbo, 0, 
		mGridNumX*mGridNumY*mGridNumZ * sizeof(Cell), cells);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glNamedBufferSubDataEXT(tmpSsbo, 0,
		mGridNumX*mGridNumY*mGridNumZ * sizeof(Cell), cells);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, acbo);
	glNamedBufferSubDataEXT(acbo, 0, sizeof(GLuint), nullptr);
}