#pragma once
#include <memory>
#include <GL/glew.h>

class Object;
struct Vertex;

class Shape {
public:
	// size: 頂点の位置の次元
	// vertexcount: 頂点の数
	// vertex: 頂点属性を格納した配列
	Shape(GLint size, GLsizei vertexcount, const Vertex* vertex);

	void draw() const;
	virtual void execute() const;

protected:
	const GLsizei vertexCount;

private:
	std::shared_ptr<const Object> object;

};