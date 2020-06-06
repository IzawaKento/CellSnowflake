#pragma once
//ビット演算
namespace BitOperation {
	//flagIDは2進数の値
	//flagsのビットのfragID番目（0から）が1ならtrue
	bool isFlag(unsigned flags, unsigned flagID) {
		return ((flags & flagID) != 0);
	}
	void setFlagTrue(unsigned* flags, unsigned flagID) {
		*flags |= flagID;
	}
	void setFlagFalse(unsigned* flags, unsigned flagID) {
		*flags &= ~flagID;
	}
}

