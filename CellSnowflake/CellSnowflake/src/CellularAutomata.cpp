#include "CellularAutomata.h"
#include <iostream>
#include "Cell.h"
#include "Program.h"

CellularAutomata::CellularAutomata(float rho, int gridNumX, int gridNumY, int gridNumZ,
	float cellSizeX, float cellSizeZ, float cellSizeY) 
	:cells(new Cell[gridNumX*gridNumY*gridNumZ]), mGridNumX(gridNumX), mGridNumY(gridNumY), mGridNumZ(gridNumZ)
{	/*
	std::cout << "sizeofCell:" << sizeof(Cell) << std::endl;
	std::cout << "sizeofGLboolean:" << sizeof(GLboolean) << std::endl;
	std::cout << "cells[0].pos:   " << &cells[0].position << std::endl;
	std::cout << "cells[0].pos[1]:" << &cells[0].position[1] << std::endl;
	std::cout << "cells[0].pos[2]:" << &cells[0].position[2] << std::endl;
	std::cout << "cells[0].pos[3]:" << &cells[0].position[3] << std::endl;
	std::cout << "cells[0].col:   " << &cells[0].color << std::endl;
	std::cout << "cells[0].col[1]:" << &cells[0].color[1] << std::endl;
	std::cout << "cells[0].col[2]:" << &cells[0].color[2] << std::endl;
	std::cout << "cells[0].col[3]:" << &cells[0].color[3] << std::endl;
	std::cout << "cells[0].fla:   " << &cells[0].flags << std::endl;
	std::cout << "cells[0].dif:   " << &cells[0].diffusionMass << std::endl;
	std::cout << "cells[0].bou:   " << &cells[0].boundaryMass << std::endl;
	std::cout << "cells[0].hori:  " << &cells[0].horizontalNeighbourNum << std::endl;
	std::cout << "cells[0].ver:   " << &cells[0].verticalNeighbourNum << std::endl;

	std::cout << "sizeofcells:" << sizeof(*cells) << std::endl;
	std::cout << "cells[0].pos:   " << &cells[1].position << std::endl;
	std::cout << "cells[0].pos[1]:" << &cells[1].position[1] << std::endl;
	std::cout << "cells[0].pos[2]:" << &cells[1].position[2] << std::endl;
	std::cout << "cells[0].pos[3]:" << &cells[1].position[3] << std::endl;
	std::cout << "cells[0].col:   " << &cells[1].color << std::endl;
	std::cout << "cells[0].col[1]:" << &cells[1].color[1] << std::endl;
	std::cout << "cells[0].col[2]:" << &cells[1].color[2] << std::endl;
	std::cout << "cells[0].col[3]:" << &cells[1].color[3] << std::endl;
	std::cout << "cells[0].fla:   " << &cells[1].flags << std::endl;
	std::cout << "cells[0].dif:   " << &cells[1].diffusionMass << std::endl;
	std::cout << "cells[0].bou:   " << &cells[1].boundaryMass << std::endl;
	std::cout << "cells[0].hori:  " << &cells[1].horizontalNeighbourNum << std::endl;
	std::cout << "cells[0].ver:   " << &cells[1].verticalNeighbourNum << std::endl;
	*/
	for (int i_y = 0; i_y < gridNumY; ++i_y) {
		GLfloat y = i_y * cellSizeY;
		for (int i_z = 0; i_z < gridNumZ; ++i_z) {
			GLfloat z = i_z * cellSizeZ;
			float shiftX = (i_z % 2) * 0.5f * cellSizeX;
			GLuint zIsOdd = i_z % 2;
			for (int i_x = 0; i_x < gridNumX; ++i_x) {
				GLfloat x = i_x * cellSizeX + shiftX;
				int pointNum = i_x + gridNumX * i_z + i_y * gridNumX * gridNumZ;
				cells[pointNum].SetPosition(x, y, z);
				//�������G�ɂȂ��Ă�
				cells[pointNum].SetFlagTrue(CellFlags::MZISODD * zIsOdd);
				
				/*�v�A���g�p
				//Z����Ȃ�*2+1�A�����Ȃ�*2�B1�Ƃ̘_����
				if (getCellNumZ(pointNum) && 0b1) {
					cells[pointNum].hexMapNum = pointNum * 2 + 1;
				}
				else {
					cells[pointNum].hexMapNum = pointNum * 2;
				}
				*/

				cells[pointNum].SetFlagFalse(CellFlags::ISCRYSTAL);		//�Ȃ��Ă�����
				cells[pointNum].SetFlagFalse(CellFlags::ISEDGECRYSTAL);
				cells[pointNum].SetFlagTrue(CellFlags::ISBOUNDARY);	//Cell�̃R���X�g���N�^�̕��������H
				cells[pointNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
				cells[pointNum].diffusionMass = rho;
				cells[pointNum].boundaryMass = 0.0f;

				//�[�����Ȃ�
				if (i_y == 0 || i_y == gridNumY - 1
					|| i_z == 0 || i_z == gridNumZ - 1
					|| i_x == 0 || i_x == gridNumX - 1) {
					cells[pointNum].SetFlagTrue(CellFlags::ISENDOFCELLS);
				}
				else {
					cells[pointNum].SetFlagFalse(CellFlags::ISENDOFCELLS);
				}
			}
		}
	}
	
	//���S�_��������
	int centerCellNum = (gridNumX / 2) + (gridNumZ / 2 * gridNumX) + (gridNumY / 2 * gridNumX * gridNumZ);
	cells[centerCellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	
	//���S�_�̎����������
	SetEdgeCry(centerCellNum + 1);
	SetEdgeCry(centerCellNum - 1);
	//�����̂��킩���
	SetEdgeCry(centerCellNum + gridNumX);
	SetEdgeCry(centerCellNum - gridNumX);
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	SetEdgeCry(centerCellNum + gridNumX + zOddNum);
	SetEdgeCry(centerCellNum - gridNumX + zOddNum);

	////if�͂����ĂȂ����ǂ߂����Ⴍ����Ǝv��
	//SetEdgeCry((cells[centerCellNum].hexMapNum + gridNumX * 2 + 1) / 2);
	//SetEdgeCry((cells[centerCellNum].hexMapNum + gridNumX * 2 - 1) / 2);
	//SetEdgeCry((cells[centerCellNum].hexMapNum - gridNumX * 2 + 1) / 2);
	//SetEdgeCry((cells[centerCellNum].hexMapNum - gridNumX * 2 - 1) / 2);
	
	//�ߋ�
	//SetEdgeCry(centerCellNum + gridNumX);
	//SetEdgeCry(centerCellNum - gridNumX);
	////�Ȃ񂩂ł������₯�ǋÂ������Ƃ͂��Ȃ�
	//int cellNumZ = getCellNumZ(centerCellNum);
	//if (!(cellNumZ && 0b1)) {
	//	SetEdgeCry(centerCellNum + gridNumX - 1);
	//	SetEdgeCry(centerCellNum - gridNumX - 1);
	//}
	//else {
	//	SetEdgeCry(centerCellNum + gridNumX + 1);
	//	SetEdgeCry(centerCellNum - gridNumX + 1);
	//}
	
	
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->color);
	glEnableVertexAttribArray(1);

	//�����̓V�F�[�_�ŏ���

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

void CellularAutomata::SetEdgeCry(int cellNum) {
	cells[cellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[cellNum].SetFlagTrue(CellFlags::ISEDGECRYSTAL);
	cells[cellNum].SetFlagFalse(CellFlags::ISBOUNDARY);
	cells[cellNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
	cells[cellNum].diffusionMass = 0.0f;
	cells[cellNum].boundaryMass = 1.0f;
}


void CellularAutomata::drawCell(int count, GLuint vfProgObj) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glUseProgram(vfProgObj);
	glDrawArrays(GL_POINTS, 0, count);
}

void CellularAutomata::initialize() {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	Cell* c(static_cast<Cell*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY)));
	memcpy(c, cells, sizeof(Cell)*mGridNumX*mGridNumY*mGridNumZ);	//�Ȃ񂩂킩��񂯂Ǔ����Ă�()
	//for (int i = 0; i < mGridNumX * mGridNumY * mGridNumZ; ++i) {
	//	c[i].position = cells[i].position;
	//	c[i].color
	//	c[i].flags = cells[i].flags;
	//	c[i].diffusionMass = cells[i].diffusionMass;
	//	c[i].boundaryMass = cells[i].boundaryMass;
	//	c[i].horizontalNeighbourNum = cells[i].horizontalNeighbourNum;
	//	c[i].verticalNeighbourNum = cells[i].verticalNeighbourNum;
	//	//memcpy(c + i, cells + i, sizeof(Cell));
	//}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

}