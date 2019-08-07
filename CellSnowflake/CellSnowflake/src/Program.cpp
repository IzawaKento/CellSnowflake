#include "Program.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// シェーダオブジェクトのコンパイル結果を表示する
// shader: シェーダオブジェクト名
// str: コンパイルエラーが発生した場所を示す文字列
GLboolean Program::printShaderInfoLog(GLuint shader, const char *str)
{
	// コンパイル結果を取得する
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;
	// シェーダのコンパイル時のログの長さを取得する
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
		// シェーダのコンパイル時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

// プログラムオブジェクトのリンク結果を表示する
	// program: プログラムオブジェクト名
GLboolean Program::printProgramInfoLog(GLuint program)
{
	// リンク結果を取得する
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;
	// シェーダのリンク時のログの長さを取得する
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
		// シェーダのリンク時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

//プログラムオブジェクトを作成する
GLuint Program::createProgramObj(const char *vsrc, const char *fsrc) {
	// 空のプログラムオブジェクトを作成する
	const GLuint program(glCreateProgram());

	if (vsrc != NULL)
	{
		// バーテックスシェーダのシェーダオブジェクトを作成する
		//戻り値は作成されたシェーダオブジェクトのハンドル (識別名) 
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		//			shader, count, **string, *length
		/*
		glShaderSource() の引数 string に渡す配列の各要素が終端にヌル文字
		('\0') をもつ通常の文字列なら, 引数 length を 0 (NULL) にします.
		*/
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);
		// バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(vobj, "vertex shader"))
			glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}
	if (fsrc != NULL)
	{
		// フラグメントシェーダのシェーダオブジェクトを作成する
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);
		// フラグメントシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(fobj, "fragment shader"))
			glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}
	
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	//フラグメントシェーダーからの出力先を指定
	glBindFragDataLocation(program, 0, "fragment");
	// プログラムオブジェクトをリンクする
	glLinkProgram(program);
	// 作成したプログラムオブジェクトを返す
	if (printProgramInfoLog(program))
		return program;

	// プログラムオブジェクトが作成できなければ 0 を返す
	glDeleteProgram(program);
	return 0;
}

//シェーダのソースファイルを読み込んだメモリを返す
bool Program::readShaderSource(const char *name, std::vector<GLchar> &buffer) {
	// ファイル名が NULL だった
	if (name == NULL) return false;
	// ソースファイルを開く   //たぶんバイナリファイルを意味
	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		// 開けなかった
		std::cerr << "Error: Can't open source file: " << name << std::endl;
		return false;
	}
	// ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());
	// ファイルサイズのメモリを確保
	buffer.resize(length + 1);
	// ファイルを先頭から読み込む
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';
	if (file.fail())
	{
		// うまく読み込めなかった
		std::cerr << "Error: Could not read souce file: " << name << std::endl;
		file.close();
		return false;
	}
	std::cout << buffer.data() << std::endl;
	// 読み込み成功
	file.close();
	return true;
}

GLuint Program::loadProgramObj(const char* vert, const char* frag) {
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));
	// プログラムオブジェクトを作成する
	return vstat && fstat ? createProgramObj(vsrc.data(), fsrc.data()) : 0;
}

//プログラムオブジェクトを作成する
GLuint Program::createCompProgramObj(const char *csrc) {
	// 空のプログラムオブジェクトを作成する
	const GLuint program(glCreateProgram());

	if (csrc != NULL)
	{
		// バーテックスシェーダのシェーダオブジェクトを作成する
		//戻り値は作成されたシェーダオブジェクトのハンドル (識別名) 
		const GLuint cobj(glCreateShader(GL_VERTEX_SHADER));
		//			shader, count, **string, *length
		/*
		glShaderSource() の引数 string に渡す配列の各要素が終端にヌル文字
		('\0') をもつ通常の文字列なら, 引数 length を 0 (NULL) にします.
		*/
		glShaderSource(cobj, 1, &csrc, NULL);
		glCompileShader(cobj);
		// バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(cobj, "compute shader"))
			glAttachShader(program, cobj);
		glDeleteShader(cobj);
	}
	// プログラムオブジェクトをリンクする
	glLinkProgram(program);
	// 作成したプログラムオブジェクトを返す
	if (printProgramInfoLog(program))
		return program;

	// プログラムオブジェクトが作成できなければ 0 を返す
	glDeleteProgram(program);
	return 0;
}

GLuint Program::loadCompProgramObj(const char* comp) {
	std::vector<GLchar> csrc;
	const bool cstat(readShaderSource(comp, csrc));
	// プログラムオブジェクトを作成する
	return cstat ? createCompProgramObj(csrc.data()) : 0;
}

