#pragma once
#include "GL/glew.h"
#include "Vector.h"

class Cell {
//もはや全部publicになってもうてるけど、structに変えたほうがいいかな
public:
	alignas(16) Vector4 position;	//4byte * 4
	//頂点カラー
	alignas(16) GLfloat color[4] = {0.1f, 0.3f, 0.4f, 1.0f};
	//推定された近傍面方向
	alignas(16) Vector3 neighbourSurfaceDir = { 0,0,0 };
	alignas(16) GLint slicedNeighbourNums[3] = { 0,0,0 };
	/*
	1桁: isCrystal
	2桁: isEdgeCrystal
	4桁: isBoundary
	8桁: isEdgeBoundary
	16桁:mZIsOdd		セル配列のZ値が奇数かどうか
	32桁:isEndofCells
	*/
	//ビット演算でややこしいことしてみる。無駄に6つもある
	alignas(4)GLuint flags = 0;
	alignas(4)GLfloat diffusionMass = 0.0f;
	alignas(4)GLfloat boundaryMass = 0.0f;
	alignas(4)GLint horizontalNeighbourNum = 0;
	alignas(4)GLint verticalNeighbourNum = 0;
	//てすと
	alignas(4)GLint neighbourNum = 0;

	void SetPosition(GLfloat x, GLfloat y, GLfloat z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		position[3] = 1.0f;
	}

	bool isFlag(unsigned flagID);
	void SetFlagTrue(unsigned flagID);
	void SetFlagFalse(unsigned flagID);
};