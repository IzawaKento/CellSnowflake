#pragma once
#include "GL/glew.h"
#include "Vector.h"

class Cell {
//���͂�S��public�ɂȂ��Ă����Ă邯�ǁAstruct�ɕς����ق�����������
public:
	alignas(16) Vector4 position;	//4byte * 4
	//���_�J���[
	alignas(16) GLfloat color[4] = {0.1f, 0.3f, 0.4f, 1.0f};
	//���肳�ꂽ�ߖT�ʕ���
	alignas(16) Vector3 neighbourSurfaceDir = { 0,0,0 };
	alignas(16) GLint slicedNeighbourNums[3] = { 0,0,0 };
	/*
	1��: isCrystal
	2��: isEdgeCrystal
	4��: isBoundary
	8��: isEdgeBoundary
	16��:mZIsOdd		�Z���z���Z�l������ǂ���
	32��:isEndofCells
	*/
	//�r�b�g���Z�ł�₱�������Ƃ��Ă݂�B���ʂ�6������
	alignas(4)GLuint flags = 0;
	alignas(4)GLfloat diffusionMass = 0.0f;
	alignas(4)GLfloat boundaryMass = 0.0f;
	alignas(4)GLint horizontalNeighbourNum = 0;
	alignas(4)GLint verticalNeighbourNum = 0;
	//�Ă���
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