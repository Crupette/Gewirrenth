#ifndef ROGUELIKEMATH_H
#define ROGUELIKEMATH_H

#include <random>

class RoguelikeMath {
public:
	static std::mt19937 randomEngine;

	static unsigned char* convertIntToChar(unsigned int id) {
		unsigned char* tmp = new unsigned char[4];
		tmp[0] = (unsigned char)(id & 0x000000FF);
		tmp[1] = (unsigned char)((id >> 8) & 0x000000FF);
		tmp[2] = (unsigned char)((id >> 16) & 0x000000FF);
		tmp[3] = (unsigned char)((id >> 24) & 0x000000FF);
		return tmp;
	}

	static unsigned int convertCharsToInt(unsigned char* chrs) {
		int id;
		id = (id | (unsigned int)(chrs[0] | ((unsigned int)(chrs[1]) << 8) | ((unsigned int)(chrs[2]) << 16) | ((unsigned int)(chrs[3]) << 24)));
		return id;
	}
};

#endif