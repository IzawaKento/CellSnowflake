#pragma once
#include <vector>
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
namespace Program {
	// �V�F�[�_�I�u�W�F�N�g�̃R���p�C�����ʂ�\������
	// shader: �V�F�[�_�I�u�W�F�N�g��
	// str: �R���p�C���G���[�����������ꏊ������������
	extern GLboolean printShaderInfoLog(GLuint shader, const char *str);

	// �v���O�����I�u�W�F�N�g�̃����N���ʂ�\������
	// program: �v���O�����I�u�W�F�N�g��
	extern GLboolean printProgramInfoLog(GLuint program);

	//�v���O�����I�u�W�F�N�g���쐬����
	extern GLuint createProgramObj(const char *vsrc, const char *gsrc, const char *fsrc);

	//�V�F�[�_�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ�
	extern bool readShaderSource(const char *name, std::vector<GLchar> &buffer);

	extern GLuint loadProgramObj(const char* vert, const char* geom, const char* frag);

	//�R���s���[�g�V�F�[�_�[�p
	extern GLuint createCompProgramObj(const char* csrc);
	extern GLuint loadCompProgramObj(const char* comp);

}