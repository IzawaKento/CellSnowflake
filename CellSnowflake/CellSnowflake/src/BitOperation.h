#pragma once
namespace BitOperation {
	//flagID��2�i���̒l
	//flags�̃r�b�g��fragID�Ԗځi0����j��1�Ȃ�true
	bool isFlag(unsigned char flags, unsigned flagID) {
		return ((flags & flagID) != 0);
	}
	void setFlagTrue(unsigned char* flags, unsigned flagID) {
		*flags |= flagID;
	}
	void setFlagFalse(unsigned char* flags, unsigned flagID) {
		*flags &= ~flagID;
	}
}

