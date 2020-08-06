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
	// GLFW ������������
	if (glfwInit() == GL_FALSE) {
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}
	atexit(glfwTerminate);

	// OpenGL �̃o�[�W�������w�肷��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Core Profile ��I������
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window window(960, 720, "CellSnowFlake");

	DebugLog::create();
	DebugLog::getInstance().printMachineInfo();

	// �w�i�F���w�肷��
	glClearColor(0.15f, 0.13f, 0.14f, 1.0f);

	glPointSize(2.0);
	//���݂Ȃ�
	///* �A���t�@�e�X�g�J�n */
	//glEnable(GL_ALPHA_TEST);
	///* �A���t�@�e�X�g�̔�r�֐� */
	//glAlphaFunc(GL_ALWAYS, 0.5);

	// �w�ʃJ�����O��L���ɂ���
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	// �f�v�X�o�b�t�@��L���ɂ���(�Ȃ����`�悳��Ȃ��Ȃ�)
	/*glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);*/

	Camera mainCamera;

	Render basicRender(&window, &mainCamera);

	CellularAutomata cellularAutomata(basicRender.cellVFProgObj(), 70, 90, 70);
	//�p�����[�^�p
	const GLint rhoLoc = glGetUniformLocation(cellularAutomata.getDiffusion1ComProgObj(), "rho");

	MarchingTetrahedra marchingTetrahedra(&cellularAutomata);
	
	// �E�B���h�E���J���Ă���ԌJ��Ԃ�
	while (window.shouldClose() == GL_FALSE)
	{
		
		//���v���C(�������A�s�����)
		if (window.isPressingReplayKey()/*glfwGetTime() > 30.0f*/)
		{
			std::cout << "RePlay" << std::endl;
			cellularAutomata.initialize();
			glfwSetTime(0.0);

			//previousTime = glfwGetTime();
		}
		
		//1�t���[���f�o�b�O
		//�Ȃ񂩖������~�߂銴��
		//std::this_thread::sleep_for(std::chrono::milliseconds(250));

		// �E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT);
		
		//FPS�\��
		DebugLog::getInstance().displayFPS();
		
		//���W�ϊ��s������߂�
		basicRender.calcCoordTransformMatrix();

		//uniform�ϐ���ݒ肷��
		basicRender.setUniforms();
		cellularAutomata.setUniforms(window.getRho());
		// �}�`��`�悷��
		
		if (window.isReady()) {
			//�Z���I�[�g�}�g������
			cellularAutomata.DispatchCompute();

			if (window.isMC()) {
				marchingTetrahedra.dispatchCompute();
				marchingTetrahedra.drawMesh();
			}
			else {
				cellularAutomata.drawCell();
			}
		}
		
		// �J���[�o�b�t�@�����ւ���
		window.swapBuffers();

		
	}
}