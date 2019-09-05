#include "Cell.h"

#include "GL/glew.h"
#include "Vector.h"
#include "BitOperation.h"

void Cell::SetPosition(GLfloat x, GLfloat y, GLfloat z) {
	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = 1.0f;
}
/*
void Cell::SetFlagTrue(unsigned flagID) {
	BitOperation::setFlagTrue(&flags, flagID);
}
void Cell::SetFlagFalse(unsigned flagID) {
	BitOperation::setFlagFalse(&flags, flagID);
}
*/



