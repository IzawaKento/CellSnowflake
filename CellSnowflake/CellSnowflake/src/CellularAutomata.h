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

	void copySSBO(GLuint readBuffer, GLuint writeBuffer);

	void DispatchCompute(int gridNumX, int gridNumY, int gridNumZ);
	
	//�Z���𒸓_�Ƃ��ĕ`��
	void drawCell(int count, GLuint vfProgObj);

	//�Z��������
	void initialize();
	
	//�����₯�ǂ����̂�?
	int getCellNumX(int i) {
		return (i % (mGridNumX * mGridNumZ)) % mGridNumX;
	}
	int getCellNumY(int i) {
		return i / (mGridNumX * mGridNumZ);
	}
	int getCellNumZ(int i) {
		return (i % (mGridNumX * mGridNumZ)) / mGridNumX;
	}

	//�Ă���
	Cell* GetCells() { return cells; }

	//��Cell�N���X����̈ڍs
	enum CellFlags {
		ISCRYSTAL = 1,				//����snowflake�ɂ�����
		ISEDGECRYSTAL = (1 << 1),	//edge_snowflake
		ISBOUNDARY = (1 << 2),		//non_boundary
		ISEDGEBOUNDARY = (1 << 3),	//boundary
		MZISODD = (1 << 4),
		ISENDOFCELLS = (1 << 5),
	};
private:
	//�Z���擪�|�C���^
	Cell* cells;

	//�Z�������E�����ɂ���
	void SetEdgeCry(int);

	//�Z���`��p
	GLuint vao;
	//�V�F�[�_�[�X�g���[�W�o�b�t�@�I�u�W�F�N�g
	GLuint ssbo;
	//�X�V�f�[�^�������ݗpSSBO�A�X�V�な�Ƀo�b�t�@���R�s�[����
	GLuint tmpSsbo;		//���O����ł����񂩂Ȃ�

	//�R���s���[�g�V�F�[�_�[�p�v���O�����I�u�W�F�N�g
	GLuint computeProgramObj = Program::loadCompProgramObj("src\\compute.comp");
	GLuint diffusion1ComProgObj = Program::loadCompProgramObj("src\\diffusion1.comp");
	GLuint diffusion2ComProgObj = Program::loadCompProgramObj("src\\diffusion2.comp");


	//�悭�g�������Ȓl�Ȃ̂ŕۑ�
	const int mGridNumX, mGridNumY, mGridNumZ;
};