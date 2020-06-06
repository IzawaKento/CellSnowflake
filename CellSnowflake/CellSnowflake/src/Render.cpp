#include "Render.h"

#include <iostream>
#include <algorithm>

#include "GL/glew.h"

#include "Vector.h"
#include "Camera.h"
#include "Window.h"
#include "Matrix.h"
#include "Program.h"


Render::Render(Window* win, Camera* camera)
: mainCamera(camera)
, window(win)
, windowSize{ window->getSize()[0], window->getSize()[1] }
, windowLocation{ window->getLocation()[0], window->getLocation()[1] }
, position{ windowLocation[0], windowLocation[1], 0.0f }
, translation(new Matrix(Matrix::translate(position)))
, rotation(new Matrix(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()*0.1f), rAxis)))
, model(new Matrix(*translation))
, view(new Matrix(Matrix::lookat(mainCamera->getPos(), mainCamera->getDestPos(), upVec)))
, modelview(new Matrix(*view * *model))
, scrollScale(6.0f)
, windowScale(window->getScale() * scrollScale)
, w(windowSize[0] / windowScale)
, h(windowSize[1] / windowScale)
, projection(new Matrix(Matrix::orthogonal(-w, w, -h, h, 1.0f, 10.0f)))
, mCellVFProgObj(Program::loadProgramObj("src\\point.vert", "src\\point.geom", "src\\point.frag"))
, mModelviewLoc(glGetUniformLocation(mCellVFProgObj, "modelview"))
, mProjectionLoc(glGetUniformLocation(mCellVFProgObj, "projection")){
	
}

Render::~Render() {
	delete translation;
	delete rotation;
	delete model;
	delete view;
	delete modelview;
	delete projection;
}

//未使用
bool Render::initGLFW() {
	// GLFW を初期化する
	if (glfwInit() == GL_FALSE) {
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}
	atexit(glfwTerminate);

	// OpenGL のバージョンを指定する
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Core Profile を選択する
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	return 0;
}
//2つやしシンプルなほうがいいかな
void Render::setWindowSize(){
	windowSize[0] = window->getSize()[0];
	windowSize[1] = window->getSize()[1];
}

void Render::setWindowLocation() {
	windowLocation[0] = window->getLocation()[0];
	windowLocation[1] = window->getLocation()[1];
}

void Render::calcCoordTransformMatrix() {
	setWindowSize();
	setWindowLocation();
	position[0] = windowLocation[0];
	position[1] = windowLocation[1];
	*translation = Matrix::translate(position);
	//*rotation = Matrix::rotate(static_cast<GLfloat>(glfwGetTime() * 0.1f), rAxis);
	*model = *translation; // * r
	*view = Matrix::lookat(mainCamera->getPos(), mainCamera->getDestPos(), upVec);
	*modelview = *view * *model;
	windowScale = window->getScale() * scrollScale;
	w = windowSize[0] / windowScale;
	h = windowSize[1] / windowScale;
	*projection = Matrix::orthogonal(-w, w, -h, h, 1.0f, 10.0f);
}

// uniform 変数に値を設定する
void Render::setUniforms() {
	glUniformMatrix4fv(mModelviewLoc, 1, GL_FALSE, modelview->data());
	glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, projection->data());
}
