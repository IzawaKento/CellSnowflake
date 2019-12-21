#pragma once
#include "Vector.h"

struct Tetrahedra {
	//四面体各頂点情報の配列
	alignas(16) Vector4 positions[4];
	alignas(16) GLint isCrystals[4];
};