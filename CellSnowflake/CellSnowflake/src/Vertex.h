#pragma once
#include <GL/glew.h>
#include "Vector.h"

struct Vertex {
	Vector4 position;
	Vector4 color = {0.3, 0.7, 0.8, 1.0};
};