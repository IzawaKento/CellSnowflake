#pragma once
#include <GL/glew.h>
struct Vertex;

class Object {
public:
	//���_�̈ʒu�̎���, ���_��, ���_�������i�[�����z��
	Object(GLint size, GLsizei vertexcount, const Vertex *vertex);

	virtual ~Object();

	void bind() const;
private:
	// �R�s�[�R���X�g���N�^�ɂ��R�s�[�֎~
	Object(const Object &o);
	// ����ɂ��R�s�[�֎~
	Object &operator=(const Object &o);

	GLuint vao;
	GLuint vbo;

};