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

//���x��葬�x�d����
const float pi = 3.141592653589f;

// �����C�Z���̏����g�U����
const float initRho = 0.2f;

const int gridNumX = 70;
const int gridNumY = 100;
const int gridNumZ = 70;

const float cellSizeX = 0.03f;
const float cellSizeZ = cellSizeX * sin(60.0f * pi / 180.0f);		//��3 / 2
const float cellSizeY = cellSizeX *1.10013f;						//c : a = 1.10013 : 1

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

	
	/*
	GLint workgroupCount;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &workgroupCount);
	std::cout << "WORK_GROUP_COUNT" << workgroupCount << std::endl;
	*/

	Window window(720, 960, "CellSnowFlake");
	//�V���O���g��
	DebugLog::create();
	DebugLog::get_instance().printMachineInfo();

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

	//// �V�F�[�_�̃\�[�X�t�@�C����ǂݍ���Ńv���O�����I�u�W�F�N�g���쐬����
	GLuint vertfragProgramObj = Program::loadProgramObj("src\\point.vert", "src\\point.geom", "src\\point.frag");
	
	// uniform �ϐ��̏ꏊ���擾����
	const GLint modelviewLoc(glGetUniformLocation(vertfragProgramObj, "modelview"));
	const GLint projectionLoc(glGetUniformLocation(vertfragProgramObj, "projection"));

	CellularAutomata cellularAutomata(initRho,
		gridNumX, gridNumY, gridNumZ, cellSizeX, cellSizeZ, cellSizeY, vertfragProgramObj);
	//�p�����[�^�p
	//const GLint rhoLoc = glGetUniformLocation(cellularAutomata.getDiffusion1ComProgObj(), "rho");

	MarchingTetrahedra marchingTetrahedra(gridNumX,
		gridNumY, gridNumZ, &cellularAutomata);

	//FPS�\���p
	double previousTime = glfwGetTime();
	int frameCount = 0;
	int stepCount = 0;
	// �E�B���h�E���J���Ă���ԌJ��Ԃ�
	while (window.shouldClose() == GL_FALSE)
	{
		
		//���v���C
		if (glfwGetTime() > 30.0f)
		{
			std::cout << "RePlay" << std::endl;
			cellularAutomata.initialize();
			glfwSetTime(0.0);

			previousTime = glfwGetTime();
		}
		
		//1�t���[���f�o�b�O
		//�Ȃ񂩖������~�߂銴��
		//std::this_thread::sleep_for(std::chrono::milliseconds(250));

		// �E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT);

		
		//FPS�\��
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

		// �g��k���̕ϊ��s������߂�
		const GLfloat *const size(window.getSize());
		
		// ���s�ړ��̕ϊ��s������߂�
		const GLfloat *const position(window.getLocation());
		const Vector3 pos{ position[0], position[1], 0.0 };
		const Matrix translation(Matrix::translate(pos));
		// ���f���ϊ��s������߂� scaling���Ă���translate�Ȃ̂ł����Ɗg�傷��
		const Vector3 rAxis{ 0.0f, 1.0f, 0.0f };
		const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()*0.1),	//�񂷗p
			rAxis));
		const Matrix model(translation);
		// �r���[�ϊ��s������߂�
		/*Vector3 eyePos{ 3.0f, 4.0f, 3.0f };
		Vector3 destPos{ 3.0f, 0.0f, 3.1f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };*/
		Vector3 eyePos{ 3.0f, 3.0f, 3.0f };
		Vector3 destPos{ 1.105f, 1.5f, 1.105f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };
		const Matrix view(Matrix::lookat(eyePos, destPos, upVec));
		// ���f���r���[�ϊ��s������߂�
		const Matrix modelview(view * model);
		// �������e�ϊ��s������߂�
		//const GLfloat fovy(window.getScale() * 0.01f);
		//const GLfloat aspect(size[0] / size[1]);
		//const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));
		//���𓊉e
		const GLfloat scale(window.getScale() * 6.0f);
		const GLfloat w(size[0] / scale), h(size[1] / scale);
		const Matrix projection(Matrix::orthogonal(-w, w, -h, h, 1.0f, 10.0f));

		// uniform �ϐ��ɒl��ݒ肷��
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
		//glUniform1f(rhoLoc, 1.0f);

		// �}�`��`�悷��

		//�Z���I�[�g�}�g������
		cellularAutomata.DispatchCompute(gridNumX, gridNumY, gridNumZ);
		if (window.isMC()) {
			marchingTetrahedra.dispatchCompute();
			marchingTetrahedra.drawMesh();
		}
		else {
			cellularAutomata.drawCell(gridNumX * gridNumY * gridNumZ, vertfragProgramObj);
		}
		
		// �J���[�o�b�t�@�����ւ���
		window.swapBuffers();

		
	}
}