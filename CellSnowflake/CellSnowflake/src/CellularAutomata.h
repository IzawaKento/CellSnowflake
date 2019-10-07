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

	void copySSBO(GLuint readBuffer, GLuint writeBuffer);

	void DispatchCompute(int gridNumX, int gridNumY, int gridNumZ);
	
	//セルを頂点として描く
	void drawCell(int count, GLuint vfProgObj);

	//セル初期化
	void initialize();
	
	//安直やけどいいのか?
	int getCellNumX(int i) {
		return (i % (mGridNumX * mGridNumZ)) % mGridNumX;
	}
	int getCellNumY(int i) {
		return i / (mGridNumX * mGridNumZ);
	}
	int getCellNumZ(int i) {
		return (i % (mGridNumX * mGridNumZ)) / mGridNumX;
	}

	//てすと
	Cell* GetCells() { return cells; }

	//旧Cellクラスからの移行
	enum CellFlags {
		ISCRYSTAL = 1,				//元のsnowflakeにあたる
		ISEDGECRYSTAL = (1 << 1),	//edge_snowflake
		ISBOUNDARY = (1 << 2),		//non_boundary
		ISEDGEBOUNDARY = (1 << 3),	//boundary
		MZISODD = (1 << 4),
		ISENDOFCELLS = (1 << 5),
	};
private:
	//セル先頭ポインタ
	Cell* cells;

	//セルを境界結晶にする
	void SetEdgeCry(int);

	//セル描画用
	GLuint vao;

	//バッファ２個いらん説
	//シェーダーストレージバッファオブジェクト
	GLuint ssbo;
	//更新データ書き込み用SSBO、更新後↑にバッファをコピーする
	GLuint tmpSsbo;		//名前これでいいんかなあ

	//コンピュートシェーダー用プログラムオブジェクト
	GLuint resetEdgeComProgObj = Program::loadCompProgramObj("src\\resetEdge.comp");
	GLuint computeProgramObj = Program::loadCompProgramObj("src\\compute.comp");
	GLuint neighbourCryNumComProgObj = Program::loadCompProgramObj("src\\neighbourCryNum.comp");
	GLuint diffusion1ComProgObj = Program::loadCompProgramObj("src\\diffusion1.comp");
	GLuint diffusion2ComProgObj = Program::loadCompProgramObj("src\\diffusion2.comp");
	GLuint freezingComProgObj = Program::loadCompProgramObj("src\\freezing.comp");
	GLuint attachmentComProgObj = Program::loadCompProgramObj("src\\attachment.comp");
	GLuint meltingComProgObj = Program::loadCompProgramObj("src\\melt.comp");


	//よく使いそうな値なので保存
	const int mGridNumX, mGridNumY, mGridNumZ;
};