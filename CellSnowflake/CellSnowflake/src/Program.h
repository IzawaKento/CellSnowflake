#pragma once
#include <vector>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
namespace Program {
	// シェーダオブジェクトのコンパイル結果を表示する
	// shader: シェーダオブジェクト名
	// str: コンパイルエラーが発生した場所を示す文字列
	extern GLboolean printShaderInfoLog(GLuint shader, const char *str);

	// プログラムオブジェクトのリンク結果を表示する
	// program: プログラムオブジェクト名
	extern GLboolean printProgramInfoLog(GLuint program);

	//プログラムオブジェクトを作成する
	extern GLuint createProgramObj(const char *vsrc, const char *gsrc, const char *fsrc);

	//シェーダのソースファイルを読み込んだメモリを返す
	extern bool readShaderSource(const char *name, std::vector<GLchar> &buffer);

	extern GLuint loadProgramObj(const char* vert, const char* geom, const char* frag);

	//コンピュートシェーダー用
	extern GLuint createCompProgramObj(const char* csrc);
	extern GLuint loadCompProgramObj(const char* comp);

}