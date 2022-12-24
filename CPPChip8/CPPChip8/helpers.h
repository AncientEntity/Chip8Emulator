#pragma once
#include <cstdint>

class HelperFunctions {

public:
	static uint8_t KeyToValue(char key) {
		switch (key) {
		case(48):
			return 0;
		case(49):
			return 1;
		case(50):
			return 2;
		case(51):
			return 3;
		case(52):
			return 4;
		case(53):
			return 5;
		case(54):
			return 6;
		case(55):
			return 7;
		case(56):
			return 8;
		case(57):
			return 9;
		case(97):
			return 0xA;
		case(98):
			return 0xB;
		case(99):
			return 0xC;
		case(100):
			return 0xD;
		case(101):
			return 0xE;
		case(102):
			return 0xF;
		}
		return 0xFF;
	}

};