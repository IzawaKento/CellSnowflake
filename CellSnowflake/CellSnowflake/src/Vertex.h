#pragma once
#include <GL/glew.h>
#include "Vector.h"

struct Vertex {
	Vector4 position;
	Vector4 color = {0.3f, 0.7f, 0.8f, 0.0f};
};