#pragma once
#include "Vector.h"
//�v
struct Tetrahedra {
	//�l�ʑ̊e���_���̔z��
	alignas(16) Vector4 positions[4];
	alignas(16) GLint isCrystals[4];
};