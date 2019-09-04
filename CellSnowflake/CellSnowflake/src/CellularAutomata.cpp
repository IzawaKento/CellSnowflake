#include "CellularAutomata.h"
#include <iostream>
#include "Cell.h"
#include "Program.h"

CellularAutomata::CellularAutomata(float rho, int gridNumX, int gridNumY, int gridNumZ,
	float cellSizeX, float cellSizeZ, float cellSizeY) 
	:cells(new Cell[gridNumX*gridNumY*gridNumZ])
{
	
	for (int i_y = 0; i_y < gridNumY; ++i_y) {
		GLfloat y = i_y * cellSizeY;
		for (int i_z = 0; i_z < gridNumZ; ++i_z) {
			GLfloat z = i_z * cellSizeZ;
			float shiftX = (i_z % 2) * 0.5f * cellSizeX;
			for (int i_x = 0; i_x < gridNumX; ++i_x) {
				GLfloat x = i_x * cellSizeX + shiftX;
				int pointNum = i_x + gridNumX * i_z + i_y * gridNumX * gridNumZ;
				cells[pointNum].SetPosition(x, y, z);
				////cells[pointNum].SetFlagFalse(Cell::CellFlags::ISCRYSTAL);		//�Ȃ��Ă�����
				////cells[pointNum].SetFlagFalse(Cell::CellFlags::ISEDGECRYSTAL);
				//cells[pointNum].SetFlagTrue(CellFlags::ISBOUNDARY);	//Cell�̃R���X�g���N�^�̕��������H
				////cells[pointNum].SetFlagFalse(Cell::CellFlags::ISEDGEBOUNDARY);
			}
		}
	}
	/*
	//���S�_��������
	int centerCellNum = (gridNumX * gridNumY * gridNumZ - 1) / 2;
	cells[centerCellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[centerCellNum].SetFlagTrue(CellFlags::ISEDGECRYSTAL);

	//���S�_�̎����������
	SetEdgeCry(centerCellNum + 1);
	SetEdgeCry(centerCellNum - 1);
	SetEdgeCry(centerCellNum + gridNumX);
	SetEdgeCry(centerCellNum - gridNumX);
	//�Ȃ񂩂ł������₯�ǋÂ������Ƃ͂��Ȃ�
	int cellNumZ = (centerCellNum % (gridNumX * gridNumZ)) / gridNumX;
	if (cellNumZ % 2 == 0) {
		SetEdgeCry(centerCellNum + gridNumX + 1);
		SetEdgeCry(centerCellNum - gridNumX + 1);
	}
	else {
		SetEdgeCry(centerCellNum + gridNumX - 1);
		SetEdgeCry(centerCellNum - gridNumX - 1);
	}
	*/
	
	//Houdini�ׂ̗荇���Z�����i�[�����͏ȗ�

	//�m�F�p���_�z��I�u�W�F�N�g
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	//vbo�Ƃ���
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glBufferData(GL_ARRAY_BUFFER,
		gridNumX*gridNumY*gridNumZ * sizeof(Cell), cells, GL_STATIC_DRAW);
	// ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g�� in �ϐ�����Q�Ƃł���悤�ɂ���
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/*
	//�V�F�[�_�[�X�g���[�W�o�b�t�@�I�u�W�F�N�g�̍쐬
	glGenBuffers(1, &ssbo);
	// �V�F�[�_�X�g���[�W�o�b�t�@�I�u�W�F�N�g�� 0 �Ԃ̌����|�C���g�Ɍ�������
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	//�f�[�^�𑗂�
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		gridNumX*gridNumY*gridNumZ * sizeof(Cell), cells, GL_STATIC_DRAW);	//�킩��񂯂ǂƂ肠����STATIC, DRAW
	*/
	

	/*�v
	//�܂Ƃ߂�ꂻ��
	const GLint localSizeXLoc(glGetUniformLocation(computeProgramObj, "localSizeX"));
	const GLint localSizeYLoc(glGetUniformLocation(computeProgramObj, "localSizeY"));
	const GLint localSizeZLoc(glGetUniformLocation(computeProgramObj, "localSizeZ"));
	//���j�t�H�[���ϐ��ɒl������
	glUniform1i(localSizeXLoc, gridNumX);
	glUniform1i(localSizeYLoc, gridNumY);
	glUniform1i(localSizeZLoc, gridNumZ);
	*/

	//�����̓V�F�[�_�ŏ����H

}

CellularAutomata::~CellularAutomata() {
	delete[] cells;
	//ssbo�폜
	glDeleteBuffers(1, &ssbo);
}

//���t���[���s���R���s���[�g�V�F�[�_�̎��s
void CellularAutomata::DispatchCompute(int gridNumX, int gridNumY, int gridNumZ) {

	// �V�F�[�_�X�g���[�W�o�b�t�@�I�u�W�F�N�g�� 0 �Ԃ̌����|�C���g�Ɍ�������
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	// �X�V�p�̃V�F�[�_�v���O�����̎g�p�J�n
	glUseProgram(computeProgramObj);
	//�����͂R������x, y, z�̃��[�N�O���[�v���N�����鐔
	glDispatchCompute(gridNumX *gridNumY * gridNumZ, 1, 1);
}
/*
void CellularAutomata::SetEdgeCry(int cellNum) {
	cells[cellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[cellNum].SetFlagTrue(CellFlags::ISEDGECRYSTAL);
	cells[cellNum].SetFlagFalse(CellFlags::ISBOUNDARY);
	cells[cellNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
	cells[cellNum].SetDiffusionMass(0.0f);
	cells[cellNum].SetBoundaryMass(1.0f);
}
*/

void CellularAutomata::drawCell(int count) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glUseProgram(vertfragProgramObj);
	glDrawArrays(GL_POINTS, 0, count);
}