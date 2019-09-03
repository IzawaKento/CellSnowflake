#include "Object.h"
#include <GL/glew.h>
#include "Vertex.h"

Object::Object(GLint size, GLsizei vertexcount, const Vertex *vertex)
{
	// ���_�z��I�u�W�F�N�g
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// ���_�o�b�t�@�I�u�W�F�N�g
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);
	// ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g�� in �ϐ�����Q�Ƃł���悤�ɂ���
	glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

Object::~Object() {
	// ���_�z��I�u�W�F�N�g���폜����
	glDeleteVertexArrays(1, &vao);
	// ���_�o�b�t�@�I�u�W�F�N�g���폜����
	glDeleteBuffers(1, &vbo);
}

void Object::bind() const {
	// �`�悷�钸�_�z��I�u�W�F�N�g���w�肷��
	glBindVertexArray(vao);
}