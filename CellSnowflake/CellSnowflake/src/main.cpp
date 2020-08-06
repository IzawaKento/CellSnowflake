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
#include "Vertex.h"
#include "Program.h"
#include "CellularAutomata.h"
#include "Cell.h"
#include "MarchingTetrahedra.h"
#include "DebugLog.h"
#include "Camera.h"
#include "Render.h"

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

	Window window(960, 720, "CellSnowFlake");

	DebugLog::create();
	DebugLog::getInstance().printMachineInfo();

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

	Camera mainCamera;

	Render basicRender(&window, &mainCamera);

	CellularAutomata cellularAutomata(basicRender.cellVFProgObj(), 70, 90, 70);
	//パラメータ用
	const GLint rhoLoc = glGetUniformLocation(cellularAutomata.getDiffusion1ComProgObj(), "rho");

	MarchingTetrahedra marchingTetrahedra(&cellularAutomata);
	
	// ウィンドウが開いている間繰り返す
	while (window.shouldClose() == GL_FALSE)
	{
		
		//リプレイ(未完成、不具合あり)
		if (window.isPressingReplayKey()/*glfwGetTime() > 30.0f*/)
		{
			std::cout << "RePlay" << std::endl;
			cellularAutomata.initialize();
			glfwSetTime(0.0);

			//previousTime = glfwGetTime();
		}
		
		//1フレームデバッグ
		//なんか無理やり止める感じ
		//std::this_thread::sleep_for(std::chrono::milliseconds(250));

		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);
		
		//FPS表示
		DebugLog::getInstance().displayFPS();
		
		//座標変換行列を求める
		basicRender.calcCoordTransformMatrix();

		//uniform変数を設定する
		basicRender.setUniforms();
		cellularAutomata.setUniforms(window.getRho());
		// 図形を描画する
		
		if (window.isReady()) {
			//セルオートマトン処理
			cellularAutomata.DispatchCompute();

			if (window.isMC()) {
				marchingTetrahedra.dispatchCompute();
				marchingTetrahedra.drawMesh();
			}
			else {
				cellularAutomata.drawCell();
			}
		}
		
		// カラーバッファを入れ替える
		window.swapBuffers();

		
	}
}