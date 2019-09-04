#pragma once
#include "GL/glew.h"
#include "Vector.h"

struct Cell {
	Vector4 position;
	/*
	1��: isCrystal
	2��: isEdgeCrystal
	4��: isBoundary
	8��: isEdgeBoundary
	*/
	
	/*
	unsigned flags = 0;	//�r�b�g���Z�ł�₱�������Ƃ��Ă݂�B���ʂɂS������

	GLfloat diffusionMass = 0.0f;
	GLfloat boundaryMass = 0.0f;
	GLint horizontalNeighbourNum = 0;
	GLint verticalNeighbourNum = 0;
	*/

	
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	/*
	void SetFlagTrue(unsigned flagID);
	void SetFlagFalse(unsigned flagID);

	void SetDiffusionMass(float value)
	{
		diffusionMass = value;
	}
	void SetBoundaryMass(float value)
	{
		boundaryMass = value;
	}

	float GetDiffusionMass() { return diffusionMass; }
	float GetBoundaryMass() { return boundaryMass; }

	//test
	Vector4 GetPos() { return position; }
	*/
	
};