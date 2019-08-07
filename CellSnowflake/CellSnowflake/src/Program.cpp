#include "Program.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// �V�F�[�_�I�u�W�F�N�g�̃R���p�C�����ʂ�\������
// shader: �V�F�[�_�I�u�W�F�N�g��
// str: �R���p�C���G���[�����������ꏊ������������
GLboolean Program::printShaderInfoLog(GLuint shader, const char *str)
{
	// �R���p�C�����ʂ��擾����
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;
	// �V�F�[�_�̃R���p�C�����̃��O�̒������擾����
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
		// �V�F�[�_�̃R���p�C�����̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

// �v���O�����I�u�W�F�N�g�̃����N���ʂ�\������
	// program: �v���O�����I�u�W�F�N�g��
GLboolean Program::printProgramInfoLog(GLuint program)
{
	// �����N���ʂ��擾����
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;
	// �V�F�[�_�̃����N���̃��O�̒������擾����
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
		// �V�F�[�_�̃����N���̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

//�v���O�����I�u�W�F�N�g���쐬����
GLuint Program::createProgramObj(const char *vsrc, const char *fsrc) {
	// ��̃v���O�����I�u�W�F�N�g���쐬����
	const GLuint program(glCreateProgram());

	if (vsrc != NULL)
	{
		// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
		//�߂�l�͍쐬���ꂽ�V�F�[�_�I�u�W�F�N�g�̃n���h�� (���ʖ�) 
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		//			shader, count, **string, *length
		/*
		glShaderSource() �̈��� string �ɓn���z��̊e�v�f���I�[�Ƀk������
		('\0') �����ʏ�̕�����Ȃ�, ���� length �� 0 (NULL) �ɂ��܂�.
		*/
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);
		// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if (printShaderInfoLog(vobj, "vertex shader"))
			glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}
	if (fsrc != NULL)
	{
		// �t���O�����g�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);
		// �t���O�����g�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if (printShaderInfoLog(fobj, "fragment shader"))
			glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}
	
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	//�t���O�����g�V�F�[�_�[����̏o�͐���w��
	glBindFragDataLocation(program, 0, "fragment");
	// �v���O�����I�u�W�F�N�g�������N����
	glLinkProgram(program);
	// �쐬�����v���O�����I�u�W�F�N�g��Ԃ�
	if (printProgramInfoLog(program))
		return program;

	// �v���O�����I�u�W�F�N�g���쐬�ł��Ȃ���� 0 ��Ԃ�
	glDeleteProgram(program);
	return 0;
}

//�V�F�[�_�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ�
bool Program::readShaderSource(const char *name, std::vector<GLchar> &buffer) {
	// �t�@�C������ NULL ������
	if (name == NULL) return false;
	// �\�[�X�t�@�C�����J��   //���Ԃ�o�C�i���t�@�C�����Ӗ�
	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		// �J���Ȃ�����
		std::cerr << "Error: Can't open source file: " << name << std::endl;
		return false;
	}
	// �t�@�C���̖����Ɉړ������݈ʒu�i���t�@�C���T�C�Y�j�𓾂�
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());
	// �t�@�C���T�C�Y�̃��������m��
	buffer.resize(length + 1);
	// �t�@�C����擪����ǂݍ���
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';
	if (file.fail())
	{
		// ���܂��ǂݍ��߂Ȃ�����
		std::cerr << "Error: Could not read souce file: " << name << std::endl;
		file.close();
		return false;
	}
	std::cout << buffer.data() << std::endl;
	// �ǂݍ��ݐ���
	file.close();
	return true;
}

GLuint Program::loadProgramObj(const char* vert, const char* frag) {
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));
	// �v���O�����I�u�W�F�N�g���쐬����
	return vstat && fstat ? createProgramObj(vsrc.data(), fsrc.data()) : 0;
}

//�v���O�����I�u�W�F�N�g���쐬����
GLuint Program::createCompProgramObj(const char *csrc) {
	// ��̃v���O�����I�u�W�F�N�g���쐬����
	const GLuint program(glCreateProgram());

	if (csrc != NULL)
	{
		// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
		//�߂�l�͍쐬���ꂽ�V�F�[�_�I�u�W�F�N�g�̃n���h�� (���ʖ�) 
		const GLuint cobj(glCreateShader(GL_VERTEX_SHADER));
		//			shader, count, **string, *length
		/*
		glShaderSource() �̈��� string �ɓn���z��̊e�v�f���I�[�Ƀk������
		('\0') �����ʏ�̕�����Ȃ�, ���� length �� 0 (NULL) �ɂ��܂�.
		*/
		glShaderSource(cobj, 1, &csrc, NULL);
		glCompileShader(cobj);
		// �o�[�e�b�N�X�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if (printShaderInfoLog(cobj, "compute shader"))
			glAttachShader(program, cobj);
		glDeleteShader(cobj);
	}
	// �v���O�����I�u�W�F�N�g�������N����
	glLinkProgram(program);
	// �쐬�����v���O�����I�u�W�F�N�g��Ԃ�
	if (printProgramInfoLog(program))
		return program;

	// �v���O�����I�u�W�F�N�g���쐬�ł��Ȃ���� 0 ��Ԃ�
	glDeleteProgram(program);
	return 0;
}

GLuint Program::loadCompProgramObj(const char* comp) {
	std::vector<GLchar> csrc;
	const bool cstat(readShaderSource(comp, csrc));
	// �v���O�����I�u�W�F�N�g���쐬����
	return cstat ? createCompProgramObj(csrc.data()) : 0;
}

