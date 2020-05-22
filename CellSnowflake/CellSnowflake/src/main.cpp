#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Matrix.h"
#include "Shape.h"
#include "Vertex.h"
#include "Program.h"
#include "CellularAutomata.h"
#include "Cell.h"
#include "MarchingTetrahedra.h"
#include "DebugLog.h"

//精度より速度重視で
const float pi = 3.141592653589f;

// 水蒸気セルの初期拡散質量
const float initRho = 0.2f;

const int gridNumX = 70;
const int gridNumY = 100;
const int gridNumZ = 70;

const float cellSizeX = 0.03f;
const float cellSizeZ = cellSizeX * sin(60.0f * pi / 180.0f);		//√3 / 2
const float cellSizeY = cellSizeX *1.10013f;						//c : a = 1.10013 : 1

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

	Window window(720, 960, "CellSnowFlake");
	//シングルトン
	DebugLog::create();
	DebugLog::get_instance().printMachineInfo();

	// 背景色を指定する
	glClearColor(0.15f, 0.13f, 0.14f, 1.0f);

	glPointSize(2.0);
	//いみない
	///* アルファテスト開始 */
	//glEnable(GL_ALPHA_TEST);
	///* アルファテストの比較関数 */
	//glAlphaFunc(GL_ALWAYS, 0.5);

	// 背面カリングを有効にする
	
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	// デプスバッファを有効にする(なぜか描画されなくなる)
	/*glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);*/

	//// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
	GLuint vertfragProgramObj = Program::loadProgramObj("src\\point.vert", "src\\point.geom", "src\\point.frag");
	
	// uniform 変数の場所を取得する
	const GLint modelviewLoc(glGetUniformLocation(vertfragProgramObj, "modelview"));
	const GLint projectionLoc(glGetUniformLocation(vertfragProgramObj, "projection"));

	CellularAutomata cellularAutomata(initRho,
		gridNumX, gridNumY, gridNumZ, cellSizeX, cellSizeZ, cellSizeY, vertfragProgramObj);
	//パラメータ用
	//const GLint rhoLoc = glGetUniformLocation(cellularAutomata.getDiffusion1ComProgObj(), "rho");

	MarchingTetrahedra marchingTetrahedra(gridNumX,
		gridNumY, gridNumZ, &cellularAutomata);

	//FPS表示用
	double previousTime = glfwGetTime();
	int frameCount = 0;
	int stepCount = 0;
	// ウィンドウが開いている間繰り返す
	while (window.shouldClose() == GL_FALSE)
	{
		
		//リプレイ
		if (glfwGetTime() > 30.0f)
		{
			std::cout << "RePlay" << std::endl;
			cellularAutomata.initialize();
			glfwSetTime(0.0);

			previousTime = glfwGetTime();
		}
		
		//1フレームデバッグ
		//なんか無理やり止める感じ
		//std::this_thread::sleep_for(std::chrono::milliseconds(250));

		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		
		//FPS表示
		double currentTime = glfwGetTime();
		frameCount++;
		stepCount++;
		if (currentTime - previousTime >= 1.0)
		{
			std::cout << "fps:" << frameCount << std::endl;
			std::cout << "t = " << stepCount << std::endl;
			frameCount = 0;
			previousTime = currentTime;
		}

		// 拡大縮小の変換行列を求める
		const GLfloat *const size(window.getSize());
		
		// 平行移動の変換行列を求める
		const GLfloat *const position(window.getLocation());
		const Vector3 pos{ position[0], position[1], 0.0 };
		const Matrix translation(Matrix::translate(pos));
		// モデル変換行列を求める scalingしてからtranslateなのでちゃんと拡大する
		const Vector3 rAxis{ 0.0f, 1.0f, 0.0f };
		const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()*0.1),	//回す用
			rAxis));
		const Matrix model(translation);
		// ビュー変換行列を求める
		/*Vector3 eyePos{ 3.0f, 4.0f, 3.0f };
		Vector3 destPos{ 3.0f, 0.0f, 3.1f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };*/
		Vector3 eyePos{ 3.0f, 3.0f, 3.0f };
		Vector3 destPos{ 1.105f, 1.5f, 1.105f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };
		const Matrix view(Matrix::lookat(eyePos, destPos, upVec));
		// モデルビュー変換行列を求める
		const Matrix modelview(view * model);
		// 透視投影変換行列を求める
		//const GLfloat fovy(window.getScale() * 0.01f);
		//const GLfloat aspect(size[0] / size[1]);
		//const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));
		//直交投影
		const GLfloat scale(window.getScale() * 6.0f);
		const GLfloat w(size[0] / scale), h(size[1] / scale);
		const Matrix projection(Matrix::orthogonal(-w, w, -h, h, 1.0f, 10.0f));

		// uniform 変数に値を設定する
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
		//glUniform1f(rhoLoc, 1.0f);

		// 図形を描画する

		//セルオートマトン処理
		cellularAutomata.DispatchCompute(gridNumX, gridNumY, gridNumZ);
		if (window.isMC()) {
			marchingTetrahedra.dispatchCompute();
			marchingTetrahedra.drawMesh();
		}
		else {
			cellularAutomata.drawCell(gridNumX * gridNumY * gridNumZ, vertfragProgramObj);
		}
		
		// カラーバッファを入れ替える
		window.swapBuffers();

		
	}
}