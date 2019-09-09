#include "Window.h"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Window::Window(int width, int height, const char* title)
	: window(glfwCreateWindow(width, height, title, NULL, NULL))
	, scale(100.0f), location{ 0, 0 }, keyStatus(GLFW_RELEASE){

	if (window == NULL)
	{
		// ウィンドウが作成できなかった
		std::cerr << "Can't create GLFW window." << std::endl;
		//c++ main関数強制終了関数
		exit(1);
	}

	// 現在のウィンドウを処理対象にする
	glfwMakeContextCurrent(window);
	// GLEW を初期化する
	glewExperimental = GL_TRUE;		//なんかtrueにしなあかんらしい
	if (glewInit() != GLEW_OK)
	{
		// GLEW の初期化に失敗した
		std::cerr << "Can't initialize GLEW" << std::endl;
		exit(1);
	}
	// 垂直同期のタイミングを待つ
	//1: 60fps, 0: 上限なし
	glfwSwapInterval(1);

	// このインスタンスの this ポインタを記録しておく
	glfwSetWindowUserPointer(window, this);

	// ウィンドウのサイズ変更時に呼び出す処理の登録
	glfwSetWindowSizeCallback(window, Window::resize);

	// マウスホイール操作時に呼び出す処理の登録
	glfwSetScrollCallback(window, Window::wheel);

	// キーボード操作時に呼び出す処理の登録
	glfwSetKeyCallback(window, Window::keyboard);

	// 開いたウィンドウの初期設定
	Window::resize(window, width, height);
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

int Window::shouldClose() const
{
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
}

// カラーバッファを入れ替えてイベントを取り出す
void Window::swapBuffers() {
	// カラーバッファを入れ替える
	glfwSwapBuffers(window);
	// イベントを取り出す
	/*if (keyStatus == GLFW_RELEASE)
		glfwWaitEvents();
	else*/
	glfwPollEvents();

	//なんでここでやるんにゃ？Waiteventが問題らしい
	// キーボードの状態を調べる
	if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
		location[0] -= 2.0f / size[0];
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
		location[0] += 2.0f / size[0];
	if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
		location[1] -= 2.0f / size[1];
	else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
		location[1] += 2.0f / size[1];

	// マウスの左ボタンの状態を調べる
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
	{
		// マウスの左ボタンが押されていたらマウスカーソルの位置を取得する
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		// マウスカーソルの正規化デバイス座標系上での位置を求める
		location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
		location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
	}
}