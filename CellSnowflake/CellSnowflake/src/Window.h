#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window(int width, int height, const char* title);
	~Window();
	int shouldClose() const;
	void swapBuffers();
	//ウィンドウサイズ変更時の処理
	static void resize(GLFWwindow *const window, int width, int height)
	{
		// ウィンドウ全体をビューポートに設定する
		glViewport(0, 0, width, height);

		// このインスタンスの this ポインタを得る
		Window *const
			instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

		if (instance != NULL) {
			// 開いたウィンドウのサイズを保存する
			instance->size[0] = static_cast<GLfloat>(width);
			instance->size[1] = static_cast<GLfloat>(height);
		}
	}
	//マウスホイール操作時の処理
	static void wheel(GLFWwindow *window, double x, double y)
	{
		// このインスタンスの this ポインタを得る
		Window *const
			instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
		if (instance != NULL)
		{
			// ワールド座標系に対するデバイス座標系の拡大率を更新する
			GLfloat a(10);	//拡大倍率
			instance->scale += static_cast<GLfloat>(y * a);
		}
	}
	//キーボード操作時の処理 引数は決まり切ってるっぽい
	static void keyboard(GLFWwindow *window, int key, int scancode,
		int action, int mods)
	{
		// このインスタンスの this ポインタを得る
		Window *const
			instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
		if (instance != NULL)
		{
			// キーの状態を保存する
			instance->keyStatus = action;
			//Qが押されたら
			if (key == GLFW_KEY_Q && action == GLFW_PRESS) {

			}
		}
	}

	const GLfloat* getSize() const { return size; }
	GLfloat getScale() const { return scale; }
	const GLfloat* getLocation() const { return location; }

private:
	GLFWwindow *const window;

	GLfloat size[2];

	GLfloat scale;

	GLfloat location[2];

	int keyStatus;


};