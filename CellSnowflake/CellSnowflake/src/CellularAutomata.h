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

	void DispatchCompute(int gridNumX, int gridNumY, int gridNumZ);
	
	//セルを頂点として描く
	void drawCell(int count, GLuint vfProgObj);

	//てすと
	Cell* GetCells() { return cells; }

	//旧Cellクラスからの移行
	enum CellFlags {
		ISCRYSTAL = 1,				//元のsnowflakeにあたる
		ISEDGECRYSTAL = (1 << 1),	//edge_snowflake
		ISBOUNDARY = (1 << 2),		//non_boundary
		ISEDGEBOUNDARY = (1 << 3)	//boundary
	};
private:
	//セル先頭ポインタ
	Cell* cells;

	//セルを境界結晶にする
	//void SetEdgeCry(int);

	//セル描画用
	GLuint vao;
	//シェーダーストレージバッファオブジェクト
	GLuint ssbo;

	//描画用プログラムオブジェクト
	//GLuint vertfragProgramObj = Program::loadProgramObj("src\\point.vert", "src\\point.frag");
	//コンピュートシェーダー用プログラムオブジェクト
	GLuint computeProgramObj = Program::loadCompProgramObj("src\\compute.comp");

};