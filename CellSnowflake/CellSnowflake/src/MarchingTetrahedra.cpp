#include "MarchingTetrahedra.h"
#include "CellularAutomata.h"
#include "Vertex.h"
#include "Tetrahedra.h"

MarchingTetrahedra::MarchingTetrahedra(int gridNumX, int gridNumY, int gridNumZ,
	CellularAutomata* ca) 
	: mGridNumX(gridNumX), mGridNumY(gridNumY), mGridNumZ(gridNumZ),
	mVertexNum(gridNumX*gridNumY*gridNumZ), cellularAutomata(ca)
{
	//���b�V���`��p���_�z��I�u�W�F�N�g
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		//�Z���� * �L���[�u��̍ő咸�_��
		mGridNumX*mGridNumY*mGridNumZ * 12 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<const Vertex *>(0)->position);
	glEnableVertexAttribArray(0);

	//�C���f�b�N�X�̃o�b�t�@�I�u�W�F�N�g
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//�Z���� * �O�p�`�̒��_�� * �L���[�u��̍ő�O�p��
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mGridNumX*mGridNumY*mGridNumZ * 3 * 5 * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);


	//�R���s���[�g�pssbo���쐬
	glGenBuffers(1, &tetraBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, tetraBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		mGridNumX*mGridNumY*mGridNumZ * sizeof(Tetrahedra), nullptr, GL_DYNAMIC_DRAW);

	//�A�g�~�b�N�J�E���^�[�o�b�t�@�I�u�W�F�N�g�쐬
	glGenBuffers(1, &vertexCounterBuffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vertexCounterBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
}

MarchingTetrahedra::~MarchingTetrahedra() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &tetraBuffer);
	glDeleteBuffers(1, &vertexCounterBuffer);
}

void MarchingTetrahedra::dispatchCompute() {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cellularAutomata->getSsbo());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ibo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, tetraBuffer);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 5, vertexCounterBuffer);
	glUseProgram(compProgObj);
	//���[�N�O���[�v�̓Z���������N������
	glDispatchCompute(mGridNumX * mGridNumY * mGridNumZ, 1, 1);
	//���邩�킩���A�ꉞ
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

//���g�p
void MarchingTetrahedra::marchingHoneycomb() {
	for (int iX = 0; iX < mGridNumX; iX++) {
		for (int iY = 0; iY < mGridNumY; iY++) {
			for (int iZ = 0; iZ < mGridNumZ; iZ++)
			{
				marchingHexagonalPrism();
			}
		}
	}
	if(vertexCount > 0)
		drawMesh();

}

void MarchingTetrahedra::marchingHexagonalPrism() {

}

void MarchingTetrahedra::marchingTetra() {

}

void MarchingTetrahedra::drawMesh() {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUseProgram(vfProgObj);
	//glDrawArrays(GL_POINTS, 0, vertexCount);
	glDrawElements(GL_TRIANGLES, mGridNumX*mGridNumY*mGridNumZ * 15, GL_UNSIGNED_INT, 0);
}