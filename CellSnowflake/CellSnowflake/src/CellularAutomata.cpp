#include "CellularAutomata.h"
#include <iostream>
#include <random>
#include "Cell.h"
//#include "Program.h"

CellularAutomata::CellularAutomata(GLuint vfProgObj)
	:cells(new Cell[mGridNumX*mGridNumY*mGridNumZ])
	, mvfProgObj(vfProgObj)
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

	
	for (int i_y = 0; i_y < mGridNumY; ++i_y) {
		GLfloat y = i_y * mCellSizeY;
		for (int i_z = 0; i_z < mGridNumZ; ++i_z) {
			GLfloat z = i_z * mCellSizeZ;
			float shiftX = (i_z % 2) * 0.5f * mCellSizeX;
			//i_z����Ȃ�1, �����Ȃ�0
			GLuint zIsOdd = i_z % 2;
			for (int i_x = 0; i_x < mGridNumX; ++i_x) {
				GLfloat x = i_x * mCellSizeX +shiftX;
				int pointNum = i_x + mGridNumX * i_z + i_y * mGridNumX * mGridNumZ;
				cells[pointNum].SetPosition(x, y, z);
				//z����Ȃ�flags |= MZSIODD, �����łȂ��Ȃ�flags |= 0�������Ȃ�
				cells[pointNum].SetFlagTrue(CellFlags::MZISODD * zIsOdd);

				cells[pointNum].SetFlagFalse(CellFlags::ISCRYSTAL);		//�Ȃ��Ă�����
				cells[pointNum].SetFlagFalse(CellFlags::ISEDGECRYSTAL);
				cells[pointNum].SetFlagTrue(CellFlags::ISBOUNDARY);	//Cell�̃R���X�g���N�^�̕��������H
				cells[pointNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
				cells[pointNum].diffusionMass = mInitRho;
				cells[pointNum].boundaryMass = 0.0f;
				std::mt19937 mt(pointNum);
				std::uniform_real_distribution<float> random(0.0f, 1.0f);
				cells[pointNum].ran = random(mt);
				//�[�����Ȃ�
				if (i_y == 0 || i_y == mGridNumY - 1
					|| i_z == 0 || i_z == mGridNumZ - 1
					|| i_x == 0 || i_x == mGridNumX - 1) {
					cells[pointNum].SetFlagTrue(CellFlags::ISENDOFCELLS);
					//�[�����̊g�U���ʂ������Ă݂�
					cells[pointNum].diffusionMass = 0.0f;
				}
				else {
					cells[pointNum].SetFlagFalse(CellFlags::ISENDOFCELLS);
				}
			}
		}
	}
	
	//���S�����Z���쐬
	int centerCellNum = (mGridNumX / 2) + (mGridNumZ / 2 * mGridNumX) + ((mGridNumY / 2 ) * mGridNumX * mGridNumZ);
	setInitialCells(centerCellNum);
	//setInitialCells(centerCellNum + 156);
	//Houdini�ׂ̗荇���Z�����i�[�����͏ȗ�

	//�m�F�p���_�z��I�u�W�F�N�g
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	//vbo�Ƃ���
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glBufferData(GL_ARRAY_BUFFER,
		mGridNumX * mGridNumY * mGridNumZ * sizeof(Cell), cells, GL_DYNAMIC_DRAW);

	// ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g�� in �ϐ�����Q�Ƃł���悤�ɂ���
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->color);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->flags);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->neighbourSurfaceDir);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Cell), &static_cast<const Cell *>(0)->vertexNormal);
	glEnableVertexAttribArray(4);
	
	//�������ݗpSSBO�쐬
	glGenBuffers(1, &tmpSsbo);
	//�����|�C���g��1�Ԃł����̂��H
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		mGridNumX * mGridNumY * mGridNumZ * sizeof(Cell), cells, GL_DYNAMIC_DRAW);

	//���g�p�H
	//�A�g�~�b�N�J�E���^�[�o�b�t�@�I�u�W�F�N�g�쐬
	glGenBuffers(1, &acbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
}

CellularAutomata::~CellularAutomata() {
	delete[] cells;
	glDeleteVertexArrays(1, &vao);
	//ssbo�폜
	glDeleteBuffers(1, &ssbo);
	glDeleteBuffers(1, &tmpSsbo);
	//�A�g�~�b�N�J�E���^�[�폜
	glDeleteBuffers(1, &acbo);

}

//���a�R
void CellularAutomata::set2RadiusCells(int centerCellNum, int yOffset) {
	int tmpY = mGridNumX * mGridNumZ;
	//���S
	SetEdgeCry(centerCellNum - tmpY * yOffset);
	//����
	SetEdgeCry(centerCellNum + 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum - 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum + mGridNumX - tmpY * yOffset);
	SetEdgeCry(centerCellNum - mGridNumX - tmpY * yOffset);
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	SetEdgeCry(centerCellNum + mGridNumX + zOddNum - tmpY * yOffset);
	SetEdgeCry(centerCellNum - mGridNumX + zOddNum - tmpY * yOffset);
}
//���a�T
void CellularAutomata::set3RadiusCells(int centerCellNum, int yOffset) {
	int tmpY = mGridNumX * mGridNumZ;
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	set2RadiusCells(centerCellNum, yOffset);
	//����
	SetEdgeCry(centerCellNum + 2 - tmpY * yOffset);
	SetEdgeCry(centerCellNum - 2 - tmpY * yOffset);
	SetEdgeCry(centerCellNum + mGridNumX - zOddNum - tmpY * yOffset);
	SetEdgeCry(centerCellNum - mGridNumX - zOddNum - tmpY * yOffset);
	SetEdgeCry(centerCellNum + mGridNumX + (zOddNum * 2) - tmpY * yOffset);
	SetEdgeCry(centerCellNum - mGridNumX + (zOddNum * 2) - tmpY * yOffset);
	SetEdgeCry(centerCellNum + (mGridNumX * 2) - tmpY * yOffset);
	SetEdgeCry(centerCellNum - (mGridNumX * 2) - tmpY * yOffset);
	SetEdgeCry(centerCellNum + (mGridNumX * 2) + 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum - (mGridNumX * 2) + 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum + (mGridNumX * 2) - 1 - tmpY * yOffset);
	SetEdgeCry(centerCellNum - (mGridNumX * 2) - 1 - tmpY * yOffset);
}

void CellularAutomata::setInitialCells(int centerCellNum) {
	//���_��������
	/*
	cells[centerCellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[centerCellNum].SetFlagFalse(CellFlags::ISEDGECRYSTAL);
	cells[centerCellNum].SetFlagFalse(CellFlags::ISBOUNDARY);
	cells[centerCellNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
	cells[centerCellNum].diffusionMass = 0.0f;
	cells[centerCellNum].boundaryMass = 1.0f;
	*/
	
	/*
	//���a�R
	SetEdgeCry(centerCellNum + 1);
	SetEdgeCry(centerCellNum - 1);
	//�����̂��킩���
	SetEdgeCry(centerCellNum + mGridNumX);
	SetEdgeCry(centerCellNum - mGridNumX);
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	SetEdgeCry(centerCellNum + mGridNumX + zOddNum);
	SetEdgeCry(centerCellNum - mGridNumX + zOddNum);
	//�㉺��
	SetEdgeCry(centerCellNum + mGridNumX * mGridNumZ);
	SetEdgeCry(centerCellNum - mGridNumX * mGridNumZ);
	*/
	
	
	int tmpY = mGridNumX * mGridNumZ;
	int zOddNum = static_cast<int>(cells[centerCellNum].isFlag(CellFlags::MZISODD)) * 2 - 1;
	
	////�Ă��Ɨp
	//for (int i = 0; i < 16; ++i) {
	//	for (int j = 0; j < 10; ++j) {
	//		for (int k = 0; k < 12; ++k) {
	//			SetEdgeCry(centerCellNum + k + mGridNumX * j + mGridNumX*mGridNumZ * i);
	//		}
	//	}
	//}
	
	SetEdgeCry(centerCellNum);
	set2RadiusCells(centerCellNum, 1);
	set3RadiusCells(centerCellNum, 2);
	set3RadiusCells(centerCellNum, 3);
	set3RadiusCells(centerCellNum, 4);
	set2RadiusCells(centerCellNum, 5);
	SetEdgeCry(centerCellNum - tmpY * 6);
}

void CellularAutomata::copySSBO(GLuint readBuffer, GLuint writeBuffer) {
	//SSBO���R�s�[
	//SSBO�̂܂܂ł͈����I�ɃR�s�[�ł��Ȃ�
	glBindBuffer(GL_COPY_READ_BUFFER, readBuffer);
	glBindBuffer(GL_COPY_WRITE_BUFFER, writeBuffer);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
		0, 0, mGridNumX*mGridNumY*mGridNumZ * sizeof(Cell));
	//���
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

//���t���[���s���R���s���[�g�V�F�[�_�̎��s
void CellularAutomata::DispatchCompute() {

	//resetEdge
	// �V�F�[�_�X�g���[�W�o�b�t�@�I�u�W�F�N�g�� 0 �Ԃ̌����|�C���g�Ɍ�������
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	// �������ݗpSSBO�� 1 �Ԃ̌����|�C���g�Ɍ�������
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	//�A�g�~�b�N�J�E���^�[�� 2 �Ԃ̌����|�C���g�Ɍ�������
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	// �X�V�p�̃V�F�[�_�v���O�����̎g�p�J�n
	glUseProgram(resetEdgeComProgObj);
	//�����͂R������x, y, z�̃��[�N�O���[�v���N�����鐔
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	//�������ւ̃A�N�Z�X���~�߂�
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//�X�V��SSBO��ǂݎ��pSSBO�ɃR�s�[
	copySSBO(tmpSsbo, ssbo);

	//setBoundary
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(computeProgramObj);
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//�X�V��SSBO��ǂݎ��pSSBO�ɃR�s�[
	copySSBO(tmpSsbo, ssbo);

	//setNeighbourCryNum
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(neighbourCryNumComProgObj);
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//�X�V��SSBO��ǂݎ��pSSBO�ɃR�s�[
	copySSBO(tmpSsbo, ssbo);

	//diffusion1
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(diffusion1ComProgObj);
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//�X�V��SSBO��ǂݎ��pSSBO�ɃR�s�[
	copySSBO(tmpSsbo, ssbo);

	/*
	//���g�pdiffusion2
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glUseProgram(diffusion2ComProgObj);
	glDispatchCompute(gridNumX, gridNumY, gridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//�X�V��SSBO��ǂݎ��pSSBO�ɃR�s�[
	copySSBO(tmpSsbo, ssbo);
	*/
	
	//freezing
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(freezingComProgObj);
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//�X�V��SSBO��ǂݎ��pSSBO�ɃR�s�[
	copySSBO(tmpSsbo, ssbo);
	
	//attachment
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(attachmentComProgObj);
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//�X�V��SSBO��ǂݎ��pSSBO�ɃR�s�[
	copySSBO(tmpSsbo, ssbo);
	
	//melting
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, acbo);
	glUseProgram(meltingComProgObj);
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);	//�Ȃ��Ă�������
	//�X�V��SSBO��ǂݎ��pSSBO�ɃR�s�[
	copySSBO(tmpSsbo, ssbo);
	
}

void CellularAutomata::SetEdgeCry(int cellNum) {
	cells[cellNum].SetFlagTrue(CellFlags::ISCRYSTAL);
	cells[cellNum].SetFlagTrue(CellFlags::ISEDGECRYSTAL);
	cells[cellNum].SetFlagFalse(CellFlags::ISBOUNDARY);
	cells[cellNum].SetFlagFalse(CellFlags::ISEDGEBOUNDARY);
	cells[cellNum].diffusionMass = 0.0f;
	cells[cellNum].boundaryMass = 1.0f;
}


void CellularAutomata::drawCell() {
	
	/*Cell* dc(static_cast<Cell*>(glMapNamedBufferEXT(ssbo, GL_WRITE_ONLY)));
	for (int i = 0; i < count; ++i)
	{
		dc[i].flags = cells[i].flags;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);*/

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, ssbo);
	glUseProgram(mvfProgObj);
	glDrawArrays(GL_POINTS, 0, mGridNumX * mGridNumY * mGridNumZ);
}

void CellularAutomata::initialize() {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	glNamedBufferSubDataEXT(ssbo, 0, 
		mGridNumX*mGridNumY*mGridNumZ * sizeof(Cell), cells);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmpSsbo);
	glNamedBufferSubDataEXT(tmpSsbo, 0,
		mGridNumX*mGridNumY*mGridNumZ * sizeof(Cell), cells);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, acbo);
	glNamedBufferSubDataEXT(acbo, 0, sizeof(GLuint), nullptr);
}

void CellularAutomata::setUniforms(GLfloat rho) {
	glUseProgram(diffusion1ComProgObj);
	glUniform1f(rhoLoc, rho);
}