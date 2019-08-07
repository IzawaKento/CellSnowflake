#pragma once
#define _USE_MATH_DEFINES			//もしかして自分でPI定義した方が高速？
#include <cmath>
#include <GL/glew.h>
#include "Program.h"

class Cell;

class CellularAutomata {
public:
	//CellularAutomata() {};
	CellularAutomata(float rho, int gridNumX, int gridNumY, int gridNumZ,
		float cellSizeX, float cellSizeZ, float cellSizeY);
	~CellularAutomata();

	void DispatchCompute();

	//てすと
	Cell* GetCells() { return cells; }
private:
	//セル先頭ポインタ
	Cell* cells;

	//セルを境界結晶にする
	void SetEdgeCry(int);

	//シェーダーストレージバッファオブジェクト
	GLuint ssbo;

	//コンピュートシェーダー用プログラムオブジェクト
	GLuint computeProgramObj = Program::loadCompProgramObj("src\\compute.comp");

};