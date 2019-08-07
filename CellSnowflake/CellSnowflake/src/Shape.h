#pragma once
#include <memory>
#include <GL/glew.h>

class Object;
struct Vertex;

class Shape {
public:
	// size: ���_�̈ʒu�̎���
	// vertexcount: ���_�̐�
	// vertex: ���_�������i�[�����z��
	Shape(GLint size, GLsizei vertexcount, const Vertex* vertex);

	void draw() const;
	virtual void execute() const;

protected:
	const GLsizei vertexCount;

private:
	std::shared_ptr<const Object> object;

};