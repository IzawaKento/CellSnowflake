#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window(int width, int height, const char* title);
	~Window();
	int shouldClose() const;
	void swapBuffers();
	//�E�B���h�E�T�C�Y�ύX���̏���
	static void resize(GLFWwindow *const window, int width, int height)
	{
		// �E�B���h�E�S�̂��r���[�|�[�g�ɐݒ肷��
		glViewport(0, 0, width, height);

		// ���̃C���X�^���X�� this �|�C���^�𓾂�
		Window *const
			instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

		if (instance != NULL) {
			// �J�����E�B���h�E�̃T�C�Y��ۑ�����
			instance->size[0] = static_cast<GLfloat>(width);
			instance->size[1] = static_cast<GLfloat>(height);
		}
	}
	//�}�E�X�z�C�[�����쎞�̏���
	static void wheel(GLFWwindow *window, double x, double y)
	{
		// ���̃C���X�^���X�� this �|�C���^�𓾂�
		Window *const
			instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
		if (instance != NULL)
		{
			// ���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦���X�V����
			GLfloat a(10);	//�g��{��
			instance->scale += static_cast<GLfloat>(y * a);
		}
	}
	//�L�[�{�[�h���쎞�̏��� �����͌��܂�؂��Ă���ۂ�
	static void keyboard(GLFWwindow *window, int key, int scancode,
		int action, int mods)
	{
		// ���̃C���X�^���X�� this �|�C���^�𓾂�
		Window *const
			instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
		if (instance != NULL)
		{
			// �L�[�̏�Ԃ�ۑ�����
			instance->keyStatus = action;
			//Q�������ꂽ��
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