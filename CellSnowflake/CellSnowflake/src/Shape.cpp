#include "Shape.h"
#include <memory>
#include <GL/glew.h>
#include "Object.h"

// size: ���_�̈ʒu�̎���
	// vertexcount: ���_�̐�
	// vertex: ���_�������i�[�����z��
Shape::Shape(GLint size, GLsizei vertexCount, const Vertex* vertex)
	: object(new Object(size, vertexCount, vertex))
	, vertexCount(vertexCount)
{
}

void Shape::draw() const {
	// ���_�z��I�u�W�F�N�g����������
	object->bind();
	// �`������s����
	execute();
}

void Shape::execute() const {
	//�`�悷��
	glDrawArrays(GL_POINTS, 0, vertexCount);
	//glDrawArrays(GL_LINE_STRIP, 0, vertexCount);
}