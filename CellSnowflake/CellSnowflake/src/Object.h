#pragma once
#include <GL/glew.h>
struct Vertex;

class Object {
public:
	//頂点の位置の次元, 頂点数, 頂点属性を格納した配列
	Object(GLint size, GLsizei vertexcount, const Vertex *vertex);

	virtual ~Object();

	void bind() const;
private:
	// コピーコンストラクタによるコピー禁止
	Object(const Object &o);
	// 代入によるコピー禁止
	Object &operator=(const Object &o);

	GLuint vao;
	GLuint vbo;

};