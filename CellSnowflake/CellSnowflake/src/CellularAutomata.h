#pragma once
#include <array>
#include <GL/glew.h>
#include "Program.h"
#include "MyMath.h"

class Cell;

class CellularAutomata {
public:
	//CellularAutomata() {};
	CellularAutomata(GLuint vfProgObj, const int gridNumX, const int gridNumY, const int gridNumZ);
	~CellularAutomata();

	void setInitialCells(int centerNum);

	void copySSBO(GLuint readBuffer, GLuint writeBuffer);

	void DispatchCompute();
	
	//セルを頂点として描く
	void drawCell();

	//セル初期化
	void initialize();

	void setUniforms(GLfloat rho);
	
	//関数内でメンバを宛先も中身も書き換えない
	const float getInitRho() const {
		return mInitRho;
	}
	const int gridNumX() const {
		return mGridNumX;
	}
	const int gridNumY() const {
		return mGridNumY;
	}
	const int gridNumZ() const {
		return mGridNumZ;
	}
	//使われてない説
	const int getCellNumX(int i) {
		return (i % (mGridNumX * mGridNumZ)) % mGridNumX;
	}
	const int getCellNumY(int i) {
		return i / (mGridNumX * mGridNumZ);
	}
	const int getCellNumZ(int i) {
		return (i % (mGridNumX * mGridNumZ)) / mGridNumX;
	}
	const GLuint getSsbo() const{
		return ssbo;
	}
	//glGetUniformLocationのため
	const GLuint getDiffusion1ComProgObj() {
		return diffusion1ComProgObj;
	}

	//てすと
	Cell* GetCells() { return cells; }

	//旧Cellクラスからの移行	boundaryを気体と間違えてしまった
	enum CellFlags {
		ISCRYSTAL = 1,				//元のsnowflakeにあたる
		ISEDGECRYSTAL = (1 << 1),	//edge_snowflake
		ISBOUNDARY = (1 << 2),		//non_boundary
		ISEDGEBOUNDARY = (1 << 3),	//boundary
		MZISODD = (1 << 4),			//6角形セル周囲アクセス用
		ISENDOFCELLS = (1 << 5),
	};
private:

	//パラメータ
	//初期拡散質量
	const float mInitRho = 0.2f;
	//セルオートマトングリッドサイズ
	const int mGridNumX = 30, mGridNumY = 40, mGridNumZ = 30;
	const float mCellSizeX = 0.03f;
	const float mCellSizeZ = mCellSizeX * sin(60.0f * MyMath::pi / 180.0f);		//√3 / 2
	const float mCellSizeY = mCellSizeX * 1.10013f;						//c : a = 1.10013 : 1
	
	//セル先頭ポインタ
	Cell* cells;

	//セルを境界結晶にする
	void SetEdgeCry(int);
	//セル初期化時に使用
	void set2RadiusCells(int centerNum, int yOffset);
	void set3RadiusCells(int centerNum, int yOffset);

	//セル描画用
	GLuint vao;

	//バッファ２個いらん説
	//シェーダーストレージバッファオブジェクト
	GLuint ssbo;
	//更新データ書き込み用SSBO、更新後↑にバッファをコピーする
	GLuint tmpSsbo;		//名前これでいいんかなあ
	//一斉面成長用アトミックカウンターバッファオブジェクト
	GLuint acbo;

	//vgf
	const GLuint mvfProgObj;

	//コンピュートシェーダー用プログラムオブジェクト
	GLuint resetEdgeComProgObj = Program::loadCompProgramObj("src\\resetEdge.comp");
	GLuint computeProgramObj = Program::loadCompProgramObj("src\\compute.comp");
	GLuint neighbourCryNumComProgObj = Program::loadCompProgramObj("src\\neighbourCryNum.comp");
	GLuint diffusion1ComProgObj = Program::loadCompProgramObj("src\\diffusionFast.comp");
	GLuint diffusion2ComProgObj = Program::loadCompProgramObj("src\\diffusion2.comp");
	GLuint freezingComProgObj = Program::loadCompProgramObj("src\\freezing.comp");
	GLuint attachmentComProgObj = Program::loadCompProgramObj("src\\attachment.comp");
	GLuint meltingComProgObj = Program::loadCompProgramObj("src\\melt.comp");

	GLuint testComProgObj = Program::loadCompProgramObj("src\\test.comp");

	//ユニフォーム変数場所
	const GLint rhoLoc = glGetUniformLocation(diffusion1ComProgObj, "rho");
};