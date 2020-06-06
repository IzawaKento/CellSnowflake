#pragma once
#include "Vector.h"

class Camera;
class Window;
class Matrix;

class Render {
public:
	Render(Window*, Camera*);
	~Render();
	//未使用
	bool initGLFW();
	void drawMarchingCubes();
	void drawCells();

	void setWindowLocation();
	void setWindowSize();
	void calcCoordTransformMatrix();
	void setUniforms();
	
	GLuint cellVFProgObj() { return mCellVFProgObj; }
	GLint modelviewLoc() { return mModelviewLoc; }
	GLint projectionLoc() { return mProjectionLoc; }
	
private:
	Camera* mainCamera;
	Window* const window;
	GLfloat windowSize[2];
	GLfloat windowLocation[2];
	Matrix* const translation;
	Vector3 position;
	const Vector3 upVec = { 0.0f, 1.0f, 0.0f };
	const Vector3 rAxis = { 0.0f, 1.0f, 0.0f };
	Matrix* const rotation;
	Matrix* const model;
	Matrix* const view;
	Matrix* const modelview;
	GLfloat scrollScale;
	GLfloat windowScale, w, h;
	Matrix* const projection;

	//とりあえずセル表示プログラムオブジェクトを用意
	GLuint mCellVFProgObj;
	const GLint mModelviewLoc;
	const GLint mProjectionLoc;
};
