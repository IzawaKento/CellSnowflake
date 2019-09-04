#pragma once
#include "GL/glew.h"
#include "Vector.h"

struct Cell {
	Vector4 position;
	/*
	1桁: isCrystal
	2桁: isEdgeCrystal
	4桁: isBoundary
	8桁: isEdgeBoundary
	*/
	
	/*
	unsigned flags = 0;	//ビット演算でややこしいことしてみる。無駄に４つもある

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