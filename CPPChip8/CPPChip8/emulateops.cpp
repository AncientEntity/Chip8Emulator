#include "main.h"

bool Chip8::_ZZZZ(int opcode) {
	int extractedOp = opcode & 0xF000;
	std::cout << "Extracted: " << extractedOp << std::endl;
	switch (extractedOp) {
		//Top Level Cases
	case 0x0000:
		return Chip8::_0ZZZ(opcode);
		break;
	case 0x8000:
		return Chip8::_8ZZZ(opcode);
		break;
	case 0xE000:
		return Chip8::_EZZZ(opcode);
		break;
	case 0xF000:
		return Chip8::_FZZZ(opcode);
		break;
		//Actual OpCodes:
	case 0x1000:
		return Chip8::_1NNN(opcode);
		break;
	default:
		return false;
	}
}

//Filters
bool Chip8::_0ZZZ(int opcode) {
	return true;
}
bool Chip8::_8ZZZ(int opcode) {
	return true;
}
bool Chip8::_EZZZ(int opcode) {
	return true;
}
bool Chip8::_FZZZ(int opcode) {
	return true;
}

//Actual Opcodes

//0ZZZ

//1NNN JUMP goto NNN
bool Chip8::_1NNN(int opcode) {
	int extractedAddress = opcode & 0x0FFF;
	programCounter = extractedAddress - 2;
	return true;
}