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

// ���ʑ̂̒��_�̈ʒu
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



//���[��
const float pi = 3.1415926535f;
//���[��
// �����C�Z���̏����g�U����
const float rho = 0.1f;

const int gridNumX = 50;
const int gridNumY = 3;
const int gridNumZ = 50;

const float cellSizeX = 0.05f;
const float cellSizeZ = cellSizeX * sin(60.0f * pi / 180.0f);		//��3 / 2
const float cellSizeY = cellSizeX;

//�Ă���
Vertex TestCellVertex[gridNumX * gridNumY * gridNumZ];

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


	Window window(640, 480, "Test");
	
	//���[�N�O���[�v���ŋN���\�ȃX���b�h�̐�	1536�@���ȁI�I
	GLint workgroupInvocations = 0;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroupInvocations);
	std::cout << "WORK_GROUP_INVOCATIONS " << workgroupInvocations << std::endl;
	//�V�F�A�[�h�������̍��v�T�C�Y	49152�H
	GLint sharedMemorySize = 0;
	glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &sharedMemorySize);
	std::cout << "SHARED_MEMORY_SIZE " << sharedMemorySize << std::endl;


	// �w�i�F���w�肷��
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//// �V�F�[�_�̃\�[�X�t�@�C����ǂݍ���Ńv���O�����I�u�W�F�N�g���쐬����
	//Program program("src\\point.vert", "src\\point.frag");
	GLuint vertfragProgramObj = Program::loadProgramObj("src\\point.vert", "src\\point.frag");

	// uniform �ϐ��̏ꏊ���擾����
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

	//FPS�\���p
	double previousTime = glfwGetTime();
	int frameCount = 0;

	// �E�B���h�E���J���Ă���ԌJ��Ԃ�
	while (window.shouldClose() == GL_FALSE)
	{
		//���v���C
		if (glfwGetTime() > 30.0f)
		{
			std::cout << "RePlay" << std::endl;
			cellularAutomata.initialize();
			glfwSetTime(0.0);
		}

		// �E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT);

		
		//FPS�\��
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0)
		{
			std::cout << "fps:" << frameCount << std::endl;

			frameCount = 0;
			previousTime = currentTime;
		}
		

		// �V�F�[�_�v���O�����̎g�p�J�n
		//glUseProgram(vertfragProgramObj);

		// �g��k���̕ϊ��s������߂�
		const GLfloat *const size(window.getSize());
		
		// ���s�ړ��̕ϊ��s������߂�
		const GLfloat *const position(window.getLocation());
		const Vector3 pos{ position[0], position[1], 0.0 };
		const Matrix translation(Matrix::translate(pos));
		// ���f���ϊ��s������߂� scaling���Ă���translate�Ȃ̂ł����Ɗg�傷��
		const Vector3 rAxis{ 0.0f, 1.0f, 0.0f };
		const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()),	//�񂷗p
			rAxis));
		const Matrix model(translation);
		// �r���[�ϊ��s������߂�

		//���ϐ�
		/*Vector3 eyePos{ 3.0f, 4.0f, 3.0f };
		Vector3 destPos{ 3.0f, 0.0f, 3.1f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };*/
		Vector3 eyePos{ 4.0f, 3.0f, 4.0f };
		Vector3 destPos{ 0.0f, 0.0f, 0.0f };
		Vector3 upVec{ 0.0f, 1.0f, 0.0f };
		const Matrix view(Matrix::lookat(eyePos, destPos, upVec));
		// ���f���r���[�ϊ��s������߂�
		const Matrix modelview(view * model);
		// ���𓊉e�ϊ��s������߂�
		const GLfloat fovy(window.getScale() * 0.01f);
		const GLfloat aspect(size[0] / size[1]);
		const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

		// uniform �ϐ��ɒl��ݒ肷��
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());

		// �}�`��`�悷��
		//shape->draw();
		//�Z���I�[�g�}�g������
		cellularAutomata.DispatchCompute(gridNumX, gridNumY, gridNumZ);

		cellularAutomata.drawCell(gridNumX*gridNumY*gridNumZ, vertfragProgramObj);
		
		// �J���[�o�b�t�@�����ւ���
		window.swapBuffers();
	}
}