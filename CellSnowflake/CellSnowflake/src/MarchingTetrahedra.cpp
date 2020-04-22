#include "MarchingTetrahedra.h"
#include "CellularAutomata.h"
#include "Cell.h"
#include "Vertex.h"
#include "Tetrahedra.h"

MarchingTetrahedra::MarchingTetrahedra(int gridNumX, int gridNumY, int gridNumZ,
	CellularAutomata* ca) 
	: mGridNumX(gridNumX), mGridNumY(gridNumY), mGridNumZ(gridNumZ),
	mVertexNum(gridNumX*gridNumY*gridNumZ), cellularAutomata(ca)
{

	//test
	//�K���ɏ�����
	/*
	Vector4 p = { 0.0, 0.0, 0.0, 1.0 };
	for (int ii = 0; ii < 5880000; ++ii) {
		v[ii].position = p;
	}
	*/

	//���b�V���`��p���_�z��I�u�W�F�N�g
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		//�Z���� * �L���[�u��̍ő咸�_��
		mGridNumX*mGridNumY*mGridNumZ * 12 * sizeof(Vertex), v, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<const Vertex *>(0)->position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), &static_cast<const Vertex *>(0)->color);
	glEnableVertexAttribArray(1);

	//�C���f�b�N�X�̃o�b�t�@�I�u�W�F�N�g
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//�Z���� * �O�p�`�̒��_�� * �L���[�u��̍ő�O�p��
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		mGridNumX*mGridNumY*mGridNumZ * 3 * 5 * sizeof(GLuint), initialIndexBuffer, GL_DYNAMIC_DRAW);


	//�R���s���[�g�pssbo���쐬
	/*
	glGenBuffers(1, &tetraBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, tetraBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		mGridNumX*mGridNumY*mGridNumZ * sizeof(Tetrahedra), nullptr, GL_DYNAMIC_DRAW);
	*/
	/*
	//�A�g�~�b�N�J�E���^�[�o�b�t�@�I�u�W�F�N�g�쐬
	glGenBuffers(1, &vertexCounterBuffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, vertexCounterBuffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	*/

	
	//���b�N�A�b�v�e�[�u��(����Ȃ���)
	glGenBuffers(1, &triangleConnectionTableBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, triangleConnectionTableBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		4096 * sizeof(GLint), triangleConnectionTable, GL_STATIC_DRAW);	//256 * (3 * 5 + 1)
	

	//���b�N�A�b�v�e�[�u��(����Ȃ���)
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Aligned16IntArray) * 4096, &triangleConnectionTableA, GL_STATIC_DRAW);
	//���j�t�H�[���u���b�N�̔ԍ����擾
	GLuint uniformBlockIndex0 = glGetUniformBlockIndex(compProgObj, "TriangleConnectionTableU");
	//���̔ԍ��ɓƎ��̔ԍ��𓖂Ă�
	glUniformBlockBinding(compProgObj, uniformBlockIndex0, 0);
}

MarchingTetrahedra::~MarchingTetrahedra() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &tetraBuffer);
	glDeleteBuffers(1, &vertexCounterBuffer);
	glDeleteBuffers(1, &triangleConnectionTableBuffer);
	delete[] v;
}

void MarchingTetrahedra::dispatchCompute() {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cellularAutomata->getSsbo());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, vbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ibo);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, tetraBuffer);
	//glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 5, vertexCounterBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, triangleConnectionTableBuffer);
	glUseProgram(compProgObj);
	
	//���[�N�O���[�v�̓Z���������N������
	glDispatchCompute(mGridNumX, mGridNumY, mGridNumZ);
	//���邩�킩���
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
	//glDrawArrays(GL_POINTS, 0, 5880000);
	glDrawElements(GL_TRIANGLES, mGridNumX*mGridNumY*mGridNumZ * 15, GL_UNSIGNED_INT, 0);
}