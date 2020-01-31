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

// 八面体の頂点の位置
constexpr Vertex octahedronVertex[] =
{
	 { 0.0f, 1.0f, 0.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f},
	 { -1.0f, 0.0f, 0.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 0.0f, -1.0f, 0.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 1.0f, 0.0f, 0.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 0.0f, 1.0f, 0.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 0.0f, 0.0f, 1.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 0.0f, -1.0f, 0.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 0.0f, 0.0f, -1.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { -1.0f, 0.0f, 0.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 0.0f, 0.0f, 1.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 1.0f, 0.0f, 0.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f },
	 { 0.0f, 0.0f, -1.0f, 1.0f,  0.2f, 0.2f, 0.2f, 1.0f }
};



//うーん
const float pi = 3.1415926535f;
//うーん
// 水蒸気セルの初期拡散質量
const float rho = 0.1f;

const int gridNumX = 70;
const int gridNumY = 100;
const int gridNumZ = 70;

const float cellSizeX = 0.03f;
const float cellSizeZ = cellSizeX * sin(60.0f * pi / 180.0f);		//√3 / 2
const float cellSizeY = cellSizeX *1.10013f;						//c : a = 1.10013 : 1

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


	Window window(640, 480, "CellSnowFlake");
	
	//ワークグループ内で起動可能なスレッドの数	1536　少な
	GLint workgroupInvocations = 0;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroupInvocations);
	std::cout << "WORK_GROUP_INVOCATIONS " << workgroupInvocations << std::endl;
	//シェアードメモリの合計サイズ	49152？
	GLint sharedMemorySize = 0;
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &sharedMemorySize);
	std::cout << "SHARED_MEMORY_SIZE " << sharedMemorySize << std::endl;
	GLint maxGeometryOutputVertices = 0;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxGeometryOutputVertices);
	std::cout << "MAX_GEOMETRY_OUTPUT_VERTICES " << maxGeometryOutputVertices << std::endl;
	GLint maxGeometryTotalOutputComponents = 0;
	glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &maxGeometryTotalOutputComponents);
	std::cout << "MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS " << maxGeometryTotalOutputComponents << std::endl;

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
	//Program program("src\\point.vert", "src\\point.frag");
	GLuint vertfragProgramObj = Program::loadProgramObj("src\\point.vert", "src\\point.geom", "src\\point.frag");

	// uniform 変数の場所を取得する
	const GLint modelviewLoc(glGetUniformLocation(vertfragProgramObj, "modelview"));
	const GLint projectionLoc(glGetUniformLocation(vertfragProgramObj, "projection"));
	const GLint LposLoc(glGetUniformLocation(vertfragProgramObj, "Lpos"));
	const GLint LambLoc(glGetUniformLocation(vertfragProgramObj, "Lamb"));
	const GLint LdiffLoc(glGetUniformLocation(vertfragProgramObj, "Ldiff"));
	const GLint LspecLoc(glGetUniformLocation(vertfragProgramObj, "Lspec"));

	CellularAutomata cellularAutomata(rho, 
		gridNumX, gridNumY, gridNumZ, cellSizeX, cellSizeZ, cellSizeY, vertfragProgramObj);
	
	MarchingTetrahedra marchingTetrahedra(gridNumX,
		gridNumY, gridNumZ, &cellularAutomata);
	
	//const GLint modelviewLoc1(glGetUniformLocation(marchingTetrahedra.getVfProgObj(), "modelview"));
	//const GLint projectionLoc1(glGetUniformLocation(marchingTetrahedra.getVfProgObj(), "projection"));

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
	
	//光源データ
	static constexpr Vector4 Lpos = { 0.0f, 0.0f, 0.0f, 1.0f };
	static constexpr Vector4 Lamb = { 0.2f, 0.1f, 0.1f };
	static constexpr Vector4 Ldiff = { 1.0f, 0.5f, 0.5f };
	static constexpr Vector4 Lspec = { 1.0f, 0.5f, 0.5f };

	//FPS表示用
	double previousTime = glfwGetTime();
	int frameCount = 0;

	// ウィンドウが開いている間繰り返す
	while (window.shouldClose() == GL_FALSE)
	{
		//リプレイ
		if (glfwGetTime() > 400.0f)
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
		const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()*0.1),	//回す用
			rAxis));
		const Matrix model(translation);
		// ビュー変換行列を求める

		//仮変数
		/*Vector3 eyePos{ 3.0f, 4.0f, 3.0f };
		Vector3 destPos{ 3.0f, 0.0f, 3.1f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };*/
		Vector3 eyePos{ 4.0f, 4.0f, 4.0f };
		Vector3 destPos{ 1.105f, 1.5f, 1.105f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };
		const Matrix view(Matrix::lookat(eyePos, destPos, upVec));
		// モデルビュー変換行列を求める
		const Matrix modelview(view * model);
		// 直交投影変換行列を求める
		const GLfloat fovy(window.getScale() * 0.01f);
		const GLfloat aspect(size[0] / size[1]);
		const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

		//光源を回す
		const Vector3 transLit = { 0.5, 0.0, 0.0 };
		const Matrix transMatLit(Matrix::translate(transLit));
		const Matrix modelMatLit(r * transMatLit);
		Vector4 rotatedLpos = modelMatLit.multiplyVec4(Lpos);

		// uniform 変数に値を設定する
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
		glUniform4fv(LposLoc, 1, rotatedLpos.data());
		glUniform3fv(LambLoc, 1, Lamb.data());
		glUniform3fv(LdiffLoc, 1, Ldiff.data());
		glUniform3fv(LspecLoc, 1, Lspec.data());
		//マーチングキューブ
		//glUniformMatrix4fv(modelviewLoc1, 1, GL_FALSE, modelview.data());
		//glUniformMatrix4fv(projectionLoc1, 1, GL_FALSE, projection.data());

		// 図形を描画する
		//shape->draw();
		//セルオートマトン処理
		cellularAutomata.DispatchCompute(gridNumX, gridNumY, gridNumZ);
		//marchingTetrahedra.dispatchCompute();
		cellularAutomata.drawCell(gridNumX * gridNumY * gridNumZ, vertfragProgramObj);
		//marchingTetrahedra.drawMesh();
		// カラーバッファを入れ替える
		window.swapBuffers();

		
	}
}