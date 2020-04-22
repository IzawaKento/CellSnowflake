#pragma once
#include <iostream>
#include <GL/glew.h>
//�V���O���g��
class DebugLog {
public:
	DebugLog(const DebugLog&) = delete;
	DebugLog& operator=(const DebugLog&) = delete;
	DebugLog(DebugLog&&) = delete;
	DebugLog& operator=(DebugLog&&) = delete;

	static DebugLog& get_instance() {
		return *instance;
	}

	static void create() {
		if (!instance) {
			instance = new DebugLog;
		}
	}
	//����͎�ł���
	static void destroy() {
		delete instance;
		instance = nullptr;
	}

	void printMachineInfo(){
		std::cout << "WORK_GROUP_INVOCATIONS " << workgroupInvocations << std::endl;
		std::cout << "SHARED_MEMORY_SIZE " << sharedMemorySize << std::endl;
		std::cout << "MAX_GEOMETRY_OUTPUT_VERTICES " << maxGeometryOutputVertices << std::endl;
		std::cout << "MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS " << maxGeometryTotalOutputComponents << std::endl;
		std::cout << "MAX_UNIFORM_BLOCK_SIZE " << maxUniformBlockSize << std::endl;
	}
private:
	//���[�N�O���[�v���ŋN���\�ȃX���b�h�̐� 1536 or 1024
	GLint workgroupInvocations = 0;
	//�V�F�A�[�h�������̍��v�T�C�Y	49152�H
	GLint sharedMemorySize = 0;
	GLint maxGeometryOutputVertices = 0;
	GLint maxGeometryTotalOutputComponents = 0;
	GLint maxUniformBlockSize = 0;

	DebugLog() {
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroupInvocations);
		glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &sharedMemorySize);
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxGeometryOutputVertices);
		glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &maxGeometryTotalOutputComponents);
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
	}
	~DebugLog() = default;

	static DebugLog* instance;
};

DebugLog* DebugLog::instance = nullptr;