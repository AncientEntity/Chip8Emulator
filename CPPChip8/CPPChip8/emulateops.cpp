#include "main.h"

bool Chip8::_ZZZZ(int opcode) {
	int extractedOp = opcode & 0xF000;
	std::cout << "Extracted: " << extractedOp << ", POS: " << programCounter << std::endl;
	switch (extractedOp) {
		//Top Level Cases
	case 0x0000:
		return Chip8::_0ZZZ(opcode);
	case 0x8000:
		return Chip8::_8ZZZ(opcode);
	case 0xE000:
		return Chip8::_EZZZ(opcode);
	case 0xF000:
		return Chip8::_FZZZ(opcode);

	//Actual OpCodes:
	case 0x1000: //1NNN JUMP
		return Chip8::_1NNN(opcode);
	case 0x2000: //1NNN JUMP
		return Chip8::_2NNN(opcode);
	case 0x3000: //1NNN JUMP
		return Chip8::_3XNN(opcode);
	case 0x4000: //1NNN JUMP
		return Chip8::_4XNN(opcode);
	case 0x5000:
		return Chip8::_5XY0(opcode);
	case 0x6000:
		return Chip8::_6XNN(opcode);
	case 0x7000:
		return Chip8::_7XNN(opcode);
	default:
		return false;
	}
}

//Filters
bool Chip8::_0ZZZ(int opcode) {
	int extractedOp = opcode & 0x0EEE;
	switch (extractedOp) {
	case (0x00E0):
		return _00E0(opcode);
	case (0x00EE):
		return _00EE(opcode);
	default:
		return _0NNN(opcode);
	}
}
bool Chip8::_8ZZZ(int opcode) {
	int extractedOp = opcode & 0x000F;
	switch (extractedOp) {
	case(0x0000): //8XY0, Vx = Vy
		return _8XY0(opcode);
	default:
		return false;
	}
}
bool Chip8::_EZZZ(int opcode) {
	return true;
}
bool Chip8::_FZZZ(int opcode) {
	return true;
}

//Actual Opcodes

//0ZZZ
bool Chip8::_0NNN(int opcode) {
	if (stackDepth == 16) {
		std::cout << "Routine Depth Limit Exceeded (16)" << std::endl;
		return false;
	}
	stack[stackDepth] = programCounter;
	stackDepth++;

	int extractedAddress = opcode & 0x0FFF;
	programCounter = extractedAddress - 2;

	//std::cout << "Routine Called At: " << extractedAddress << ", Depth: " << stackDepth << std::endl;

	return true;
}
bool Chip8::_00E0(int opcode) {

	//Memory 0xF00 to 0xFFF is the display buffer.
	for (int i = 0xF00; i < 0xFFF; i++) {
		memory[i] = 0;
	}

	return true;
}
bool Chip8::_00EE(int opcode) {
	
	programCounter = (opcode & 0x0FFF) - 2;
	
	return true;
}


//1NNN JUMP goto NNN
bool Chip8::_1NNN(int opcode) {
	int extractedAddress = opcode & 0x0FFF;
	programCounter = extractedAddress - 2;
	std::cout << "Jump to: " << programCounter << std::endl;
	return true;
}

//2NNN Subrountine
bool Chip8::_2NNN(int opcode) {
	return _0NNN(opcode);
}

//3XNN if Vx == NN skip next instruction
bool Chip8::_3XNN(int opcode) {
	int registerNumber = (opcode & 0x0F00) >> 8;
	int val = (opcode & 0x00FF);
	
	if (registers[registerNumber] == val) {
		programCounter += 2;
	}
	return true;
}

//3XNN if Vx != NN skip next instruction
bool Chip8::_4XNN(int opcode) {
	int registerNumber = (opcode & 0x0F00) >> 8;
	int val = (opcode & 0x00FF);
	if (registers[registerNumber] != val) {
		programCounter += 2;
	}
	return true;
}

//3XNN if Vx != NN skip next instruction
bool Chip8::_5XY0(int opcode) {
	int registerNumber1 = (opcode & 0x0F00) >> 8;
	int registerNumber2 = (opcode & 0x00F0) >> 4;
	if (registers[registerNumber1] == registers[registerNumber2]) {
		programCounter += 2;
	}
	return true;
}

//Const Operators
//6XNN Vx = NN
bool Chip8::_6XNN(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;
	int val = (opcode & 0x00FF);

	registers[reg] = val;

	return true;
}

//7XNN Vx += NN
bool Chip8::_7XNN(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;
	int val = (opcode & 0x00FF);

	registers[reg] += val;

	//std::cout << "Reg: " << reg << ",Val: " << (int)registers[reg] << std::endl;

	return true;
}

//Assign Operators Vx = Vy
bool Chip8::_8XY0(int opcode) {

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	registers[1] = registers[2];

	return true;
}

//Math
bool Chip8::_8XY4(int opcode) {

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	uint8_t before = registers[reg1];
	registers[reg1] += registers[reg2];

	if (before <= registers[reg1]) {
		//Carry occured
		registers[16] = 1;
	}
	else {
		registers[16] = 0;
	}

}

//Math
bool Chip8::_8XY5(int opcode) {

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	uint8_t before = registers[reg1];
	registers[reg1] -= registers[reg2];

	if (before >= registers[reg1]) {
		//Carry occured
		registers[16] = 0;
	}
	else {
		registers[16] = 1;
	}

}