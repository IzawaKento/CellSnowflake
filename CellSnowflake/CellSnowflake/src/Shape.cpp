#include "Shape.h"
#include <memory>
#include <GL/glew.h>
#include "Object.h"

// size: 頂点の位置の次元
	// vertexcount: 頂点の数
	// vertex: 頂点属性を格納した配列
Shape::Shape(GLint size, GLsizei vertexCount, const Vertex* vertex)
	: object(new Object(size, vertexCount, vertex))
	, vertexCount(vertexCount)
{
}

void Shape::draw() const {
	// 頂点配列オブジェクトを結合する
	object->bind();
	// 描画を実行する
	execute();
}

void Shape::execute() const {
	//描画する
	glDrawArrays(GL_POINTS, 0, vertexCount);
	//glDrawArrays(GL_LINE_STRIP, 0, vertexCount);
}