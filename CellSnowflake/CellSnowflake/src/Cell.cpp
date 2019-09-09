#include "Cell.h"

#include "GL/glew.h"
#include "Vector.h"
#include "BitOperation.h"



void Cell::SetFlagTrue(unsigned flagID) {
	BitOperation::setFlagTrue(&flags, flagID);
}
void Cell::SetFlagFalse(unsigned flagID) {
	BitOperation::setFlagFalse(&flags, flagID);
}




