#pragma once
#include "GL/glew.h"
#include "Vector.h"

class Cell {
public:
	enum CellFlags {
		ISCRYSTAL = 1,				//����snowflake�ɂ�����
		ISEDGECRYSTAL = (1 << 1),	//edge_snowflake
		ISBOUNDARY = (1 << 2),		//non_boundary
		ISEDGEBOUNDARY = (1 << 3)	//boundary
	};
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
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
private:
	Vector4 position;
	/*
	1��: isCrystal
	2��: isEdgeCrystal
	4��: isBoundary
	8��: isEdgeBoundary
	*/
	unsigned char flags = 0;	//�r�b�g���Z�ł�₱�������Ƃ��Ă݂�B���ʂɂS������

	float diffusionMass = 0.0f;
	float boundaryMass = 0.0f;
	int horizontalNeighbourNum = 0, verticalNeighbourNum = 0;
	
};