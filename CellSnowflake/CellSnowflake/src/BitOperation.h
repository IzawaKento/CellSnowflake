#pragma once
namespace BitOperation {
	//flagIDは2進数の値
	//flagsのビットのfragID番目（0から）が1ならtrue
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

