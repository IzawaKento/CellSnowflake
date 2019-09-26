#include "Cell.h"

#include "GL/glew.h"
#include "Vector.h"
#include "BitOperation.h"


bool Cell::isFlag(unsigned flagID) {
	return BitOperation::isFlag(flags, flagID);
}
void Cell::SetFlagTrue(unsigned flagID) {
	BitOperation::setFlagTrue(&flags, flagID);
}
void Cell::SetFlagFalse(unsigned flagID) {
	BitOperation::setFlagFalse(&flags, flagID);
}




