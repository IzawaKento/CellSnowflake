#pragma once
#include "Vector.h"

enum CamProjectionType {
	ORTHOGONAL,
	PERSPECTIVE
};

class Camera {
public:
	Camera();
	~Camera();

	const Vector3 getPos() const { return position; }
	const Vector3 getDestPos() const { return destPos; }
	const CamProjectionType getCamProjectionType() const { return projectionType; }

	
private:
	CamProjectionType projectionType = ORTHOGONAL;
	Vector3 position = { 3.0f, 3.0f, 3.0f };
	Vector3 destPos = { 1.105f, 1.5f, 1.105f };
	//const Vector3 upVec = { 0.0f, 1.0f, 0.0f };
	//const Vector3 rAxis{ 0.0f, 1.0f, 0.0f };

};