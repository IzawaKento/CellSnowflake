#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Matrix.h"
#include "Shape.h"
#include "Vertex.h"
#include "Program.h"
#include "CellularAutomata.h"
#include "Cell.h"

// 八面体の頂点の位置
constexpr Vertex octahedronVertex[] =
{
 { 0.0f, 1.0f, 0.0f, 1.0f },
 { -1.0f, 0.0f, 0.0f, 1.0f },
 { 0.0f, -1.0f, 0.0f, 1.0f },
 { 1.0f, 0.0f, 0.0f, 1.0f },
 { 0.0f, 1.0f, 0.0f, 1.0f },
 { 0.0f, 0.0f, 1.0f, 1.0f },
 { 0.0f, -1.0f, 0.0f, 1.0f },
 { 0.0f, 0.0f, -1.0f, 1.0f },
 { -1.0f, 0.0f, 0.0f, 1.0f },
 { 0.0f, 0.0f, 1.0f, 1.0f },
 { 1.0f, 0.0f, 0.0f, 1.0f },
 { 0.0f, 0.0f, -1.0f, 1.0f }
};



//うーん
const float pi = 3.1415926535f;
//うーん
// 水蒸気セルの初期拡散質量
const float rho = 0.1f;

const int gridNumX = 50;
const int gridNumY = 3;
const int gridNumZ = 50;

const float cellSizeX = 0.05f;
const float cellSizeZ = cellSizeX * sin(60.0f * pi / 180.0f);		//√3 / 2
const float cellSizeY = cellSizeX;

//てすと
Vertex TestCellVertex[gridNumX * gridNumY * gridNumZ];

int main() {
	// GLFW を初期化する
	if (glfwInit() == GL_FALSE) {
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}
	atexit(glfwTerminate);

	// OpenGL のバージョンを指定する
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Core Profile を選択する
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	/*
	GLint workgroupCount;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &workgroupCount);
	std::cout << "WORK_GROUP_COUNT" << workgroupCount << std::endl;
	*/


	Window window(640, 480, "Test");
	
	//ワークグループ内で起動可能なスレッドの数	1536　少な！！
	GLint workgroupInvocations = 0;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroupInvocations);
	std::cout << "WORK_GROUP_INVOCATIONS " << workgroupInvocations << std::endl;
	//シェアードメモリの合計サイズ	49152？
	GLint sharedMemorySize = 0;
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &sharedMemorySize);
	std::cout << "SHARED_MEMORY_SIZE " << sharedMemorySize << std::endl;


	// 背景色を指定する
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
	//Program program("src\\point.vert", "src\\point.frag");
	GLuint vertfragProgramObj = Program::loadProgramObj("src\\point.vert", "src\\point.frag");

	// uniform 変数の場所を取得する
	const GLint modelviewLoc(glGetUniformLocation(vertfragProgramObj, "modelview"));
	const GLint projectionLoc(glGetUniformLocation(vertfragProgramObj, "projection"));

	CellularAutomata cellularAutomata(rho, gridNumX, gridNumY, gridNumZ, cellSizeX, cellSizeZ, cellSizeY);
	
	for (int k = 0; k < gridNumY; ++k) {
		for (int j = 0; j < gridNumZ; ++j) {
			for (int i = 0; i < gridNumX; ++i) {
				int num = i + gridNumX * j + k * gridNumX * gridNumZ;
				TestCellVertex[num].position = cellularAutomata.GetCells()[num].position;
			}
		}
	}
	//std::unique_ptr<const Shape> shape(new Shape(3, 12, octahedronVertex));
	std::unique_ptr<const Shape> shape(new Shape(4, gridNumX*gridNumY*gridNumZ, TestCellVertex));

	//FPS表示用
	double previousTime = glfwGetTime();
	int frameCount = 0;

	// ウィンドウが開いている間繰り返す
	while (window.shouldClose() == GL_FALSE)
	{
		//リプレイ
		if (glfwGetTime() > 30.0f)
		{
			std::cout << "RePlay" << std::endl;
			cellularAutomata.initialize();
			glfwSetTime(0.0);
		}

		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		
		//FPS表示
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0)
		{
			std::cout << "fps:" << frameCount << std::endl;

			frameCount = 0;
			previousTime = currentTime;
		}
		

		// シェーダプログラムの使用開始
		//glUseProgram(vertfragProgramObj);

		// 拡大縮小の変換行列を求める
		const GLfloat *const size(window.getSize());
		
		// 平行移動の変換行列を求める
		const GLfloat *const position(window.getLocation());
		const Vector3 pos{ position[0], position[1], 0.0 };
		const Matrix translation(Matrix::translate(pos));
		// モデル変換行列を求める scalingしてからtranslateなのでちゃんと拡大する
		const Vector3 rAxis{ 0.0f, 1.0f, 0.0f };
		const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()),	//回す用
			rAxis));
		const Matrix model(translation);
		// ビュー変換行列を求める

		//仮変数
		/*Vector3 eyePos{ 3.0f, 4.0f, 3.0f };
		Vector3 destPos{ 3.0f, 0.0f, 3.1f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };*/
		Vector3 eyePos{ 4.0f, 3.0f, 4.0f };
		Vector3 destPos{ 0.0f, 0.0f, 0.0f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };
		const Matrix view(Matrix::lookat(eyePos, destPos, upVec));
		// モデルビュー変換行列を求める
		const Matrix modelview(view * model);
		// 直交投影変換行列を求める
		const GLfloat fovy(window.getScale() * 0.01f);
		const GLfloat aspect(size[0] / size[1]);
		const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

		// uniform 変数に値を設定する
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());

		// 図形を描画する
		//shape->draw();
		//セルオートマトン処理
		cellularAutomata.DispatchCompute(gridNumX, gridNumY, gridNumZ);

		cellularAutomata.drawCell(gridNumX*gridNumY*gridNumZ, vertfragProgramObj);
		
		// カラーバッファを入れ替える
		window.swapBuffers();
	}
}