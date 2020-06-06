#pragma once
#include <iostream>
#include <GL/glew.h>
//シングルトン
class DebugLog {
public:
	DebugLog(const DebugLog&) = delete;
	DebugLog& operator=(const DebugLog&) = delete;
	DebugLog(DebugLog&&) = delete;
	DebugLog& operator=(DebugLog&&) = delete;

	static DebugLog& getInstance() {
		return *instance;
	}

	static void create() {
		if (!instance) {
			instance = new DebugLog;
		}
	}
	//解放は手でする
	static void destroy() {
		delete instance;
		instance = nullptr;
	}

	const void printMachineInfo(){
		std::cout << "WORK_GROUP_INVOCATIONS " << workgroupInvocations << std::endl;
		std::cout << "SHARED_MEMORY_SIZE " << sharedMemorySize << std::endl;
		std::cout << "MAX_GEOMETRY_OUTPUT_VERTICES " << maxGeometryOutputVertices << std::endl;
		std::cout << "MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS " << maxGeometryTotalOutputComponents << std::endl;
		std::cout << "MAX_UNIFORM_BLOCK_SIZE " << maxUniformBlockSize << std::endl;
	}

	void displayFPS() {
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
	}

private:
	//ワークグループ内で起動可能なスレッドの数 1536 or 1024
	GLint workgroupInvocations = 0;
	//シェアードメモリの合計サイズ	49152？
	GLint sharedMemorySize = 0;
	GLint maxGeometryOutputVertices = 0;
	GLint maxGeometryTotalOutputComponents = 0;
	GLint maxUniformBlockSize = 0;
	//FPS表示用
	double previousTime = glfwGetTime();
	int frameCount = 0;
	int stepCount = 0;

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