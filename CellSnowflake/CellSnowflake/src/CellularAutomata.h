#pragma once
#define _USE_MATH_DEFINES			//���������Ď�����PI��`�������������H
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

	//�Ă���
	Cell* GetCells() { return cells; }

	//��Cell�N���X����̈ڍs
	enum CellFlags {
		ISCRYSTAL = 1,				//����snowflake�ɂ�����
		ISEDGECRYSTAL = (1 << 1),	//edge_snowflake
		ISBOUNDARY = (1 << 2),		//non_boundary
		ISEDGEBOUNDARY = (1 << 3)	//boundary
	};
private:
	//�Z���擪�|�C���^
	Cell* cells;

	//�Z�������E�����ɂ���
	void SetEdgeCry(int);

	//�V�F�[�_�[�X�g���[�W�o�b�t�@�I�u�W�F�N�g
	GLuint ssbo;

	//�R���s���[�g�V�F�[�_�[�p�v���O�����I�u�W�F�N�g
	GLuint computeProgramObj = Program::loadCompProgramObj("src\\compute.comp");

};