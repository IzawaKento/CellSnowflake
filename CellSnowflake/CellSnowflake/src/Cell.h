#pragma once
#include "GL/glew.h"
#include "Vector.h"

class Cell {
//���͂�S��public�ɂȂ��Ă����Ă邯�ǁAstruct�ɕς����ق�����������
public:
	alignas(16) Vector4 position;	//4byte * 4
	//���_�J���[
	alignas(16) GLfloat color[4] = {0.1f, 0.3f, 0.4f, 1.0f};
	//�Z�p�`�}�b�v�����l�@���g�p
	GLint hexMapNum;
	//�Z���z���Z�l������ǂ���
	GLboolean mZIsOdd;
	/*
	1��: isCrystal
	2��: isEdgeCrystal
	4��: isBoundary
	8��: isEdgeBoundary
	*/
	//�r�b�g���Z�ł�₱�������Ƃ��Ă݂�B���ʂɂS������
	GLuint flags = 0;
	GLfloat diffusionMass = 0.0f;
	GLfloat boundaryMass = 0.0f;
	GLint horizontalNeighbourNum = 0;
	GLint verticalNeighbourNum = 0;

	void SetPosition(GLfloat x, GLfloat y, GLfloat z) {
		position[0] = x;
		position[1] = y;
		position[2] = z;
		position[3] = 1.0f;
	}

	void SetFlagTrue(unsigned flagID);
	void SetFlagFalse(unsigned flagID);
};