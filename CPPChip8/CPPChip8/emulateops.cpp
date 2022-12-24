#include "main.h"

bool Chip8::_ZZZZ(int opcode) {
	int extractedOp = opcode & 0xF000;
	if (debugMode) {
		std::cout << "Extracted: " << extractedOp << ", POS: " << programCounter << std::endl;
	}
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
	case 0x9000:
		return Chip8::_9XY0(opcode);
	case 0xA000:
		return Chip8::_ANNN(opcode);
	case 0xB000:
		return Chip8::_BNNN(opcode);
	case 0xC000:
		return Chip8::_CXNN(opcode);
	case 0xD000:
		return Chip8::_DXYN(opcode);
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
	case(0x0001):
		return _8XY1(opcode);
	case(0x0002):
		return _8XY2(opcode);
	case(0x0003):
		return _8XY3(opcode);
	case(0x0004):
		return _8XY4(opcode);
	case(0x0005):
		return _8XY5(opcode);
	case(0x0006):
		return _8XY6(opcode);
	case(0x0007):
		return _8XY7(opcode);
	case(0x000E):
		return _8XYE(opcode);
	default:
		return false;
	}
}
bool Chip8::_EZZZ(int opcode) {
	int extractedOp = opcode & 0x00FF;
	switch (extractedOp) {
	case(0x009E): //8XY0, Vx = Vy
		return _EX9E(opcode);
	case(0x00A1):
		return _EXA1(opcode);
	default:
		return false;
	}
}
bool Chip8::_FZZZ(int opcode) {
	int extractedOp = opcode & 0x00FF;
	switch (extractedOp) {
	//case(0x0007): //Instruction NEI
	//	return _FX07(opcode);
	case(0x0007):
		return _FX07(opcode);
	case(0x000A):
		return _FX0A(opcode);
	case(0x0015):
		return _FX15(opcode);
	case(0x0018):
		return _FX18(opcode);
	case(0x001E):
		return _FX1E(opcode);
	case(0x0029):
		return _FX29(opcode);
	case (0x0055):
		return _FX55(opcode);
	case(0x0065):
		return _FX65(opcode);
	case(0x0033):
		return _FX33(opcode);

	default:
		return false;
	}
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
	displayChanged = true;
	return true;
}


//return; from subroutine
bool Chip8::_00EE(int opcode) {
	stackDepth--;
	programCounter = stack[stackDepth];//(opcode & 0x0FFF) - 2;
	return true;
}


//1NNN JUMP goto NNN
bool Chip8::_1NNN(int opcode) {
	int extractedAddress = opcode & 0x0FFF;
	programCounter = extractedAddress - 2;
	//std::cout << "Jump to: " << programCounter << std::endl;
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
	uint8_t val = (opcode & 0x00FF);
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

	//std::cout << "BFORE" << registers[reg] << std::endl;

	registers[reg] += (uint8_t)val;

	//std::cout << "Reg: " << reg << ",Val: " << (int)registers[reg] << std::endl;

	return true;
}


//Assign Operators Vx = Vy
bool Chip8::_8XY0(int opcode) {

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	registers[reg1] = registers[reg2];

	return true;
}

//BitOp

bool Chip8::_8XY1(int opcode) {//Vx = Vx | Vy

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	registers[reg1] = registers[reg1] | registers[reg2];

	return true;
}


bool Chip8::_8XY2(int opcode) {//Vx = Vx & Vy

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	registers[reg1] = registers[reg1] & registers[reg2];

	return true;
}

bool Chip8::_8XY3(int opcode) { //Vx = Vx ^ Vy

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	registers[reg1] = registers[reg1] ^ registers[reg2];

	return true;
}



//Math
bool Chip8::_8XY4(int opcode) {

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	uint8_t before = registers[reg1];
	registers[reg1] += registers[reg2];

	if (before > registers[reg1]) {
		//Carry occured
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}

	return true;

}

bool Chip8::_8XY5(int opcode) {

	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	uint8_t before = registers[reg1];
	registers[reg1] -= registers[reg2];

	if (before < registers[reg1]) {
		//Carry occured
		registers[0xF] = 0;
	}
	else {
		registers[0xF] = 1;
	}

	return true;

}

//BitOp BitShifting

//8Xy6 Vx >>= 1
bool Chip8::_8XY6(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;
	int flag = (registers[reg] % 2 != 0);
	registers[reg] = registers[reg] >> 1;
	registers[0xF] = flag;
	return true;
}


//Vx = Vy - Vx
bool Chip8::_8XY7(int opcode) {
	int regX = (opcode & 0x0F00) >> 8;
	int regY = (opcode & 0x00F0) >> 4;
	int flag = 1;
	if (registers[regY] < registers[regX]) {
		flag = 0;
	}
	registers[regX] = registers[regY] - registers[regX];
	registers[0xF] = flag;

	return true;
}

//8Xy6 Vx <<= 1
bool Chip8::_8XYE(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;
	int flag = ((registers[reg] >> 7) % 2 != 0);;// >> 7;
	registers[reg] = registers[reg] << 1;
	registers[0xF] = flag;
	return true;
}



//9XY0 Cond
bool Chip8::_9XY0(int opcode) {
	int reg1 = (opcode & 0x0F00) >> 8;
	int reg2 = (opcode & 0x00F0) >> 4;

	if (registers[reg1] != registers[reg2]) {
		programCounter += 2;
	}
	return true;
}

//ANNN I = NNN
bool Chip8::_ANNN(int opcode) {
	
	iRegister = opcode & 0x0FFF;

	return true;
}

//BNNN PC = V0 + NNN
bool Chip8::_BNNN(int opcode) {

	programCounter = (opcode & 0x0FFF) + registers[0];

	return true;
}

//Rand Vx = rand() & NN
bool Chip8::_CXNN(int opcode) {

	int reg = (opcode & 0x0F00) >> 8;
	int val = opcode && 0x00FF;

	registers[reg] = (rand() % 256) & val;

	return true;
}

//Display draw(Vx,Vy,N)
bool Chip8::_DXYN(int opcode) {
	uint8_t x = registers[(opcode & 0x0F00) >> 8];
	uint8_t y = registers[(opcode & 0x00F0) >> 4];
	uint8_t num = (opcode & 0x000F);		  //Height of Sprite

	int bufferPos = 0xF00 + (x / 8) + (y * 8); //Pos in memory[]
	int iPos = iRegister;				  //Copy of iRegister

	registers[0xF] = 0;


	int bitOffset = x % 8;

	for (int h = 0; h < num; h++) {

		uint8_t newVal = memory[bufferPos];
		uint8_t mask = 0b10000000 >> bitOffset;
		uint8_t spriteMask = 0b10000000;

		if ((memory[bufferPos] & (memory[iPos] >> bitOffset)) || (memory[bufferPos + 1] & (memory[iPos] << (8 - bitOffset)))) {
			registers[0xF] = 1;
		}


		memory[bufferPos] ^= memory[iPos] >> bitOffset;
		memory[bufferPos + 1] ^= memory[iPos] << (8 - bitOffset);

		bufferPos += 8;
		iPos += 1;
	}
	displayChanged = true;
	return true;

}

//KeyOp if key() == Vx
bool Chip8::_EX9E(int opcode) {

	int reg = (opcode & 0x0F00) >> 8;
	if (_IsPressed(registers[reg])) {
		programCounter += 2;
	}

	return true;
}

//KeyOp if key() != Vx
bool Chip8::_EXA1(int opcode) {

	int reg = (opcode & 0x0F00) >> 8;
	if (!_IsPressed(registers[reg])) {
		programCounter += 2;
	}

	return true;
}

//KeyOp await key() then save to Vx
bool Chip8::_FX0A(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	uint8_t key = 0xFF;
	while (key == 0xFF) {
		key = _GetKey();
	}

	registers[reg] = key;
	//std::cout << "Awaited: " << key << std::endl;

	return true;
}

//I += VX
bool Chip8::_FX1E(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	iRegister += registers[reg];

	return true;
}

//MEM I = Sprite_ADDR(VX)
bool Chip8::_FX29(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	//self.gpio[self.vx] * 5) & 0x0FFF

	iRegister = registers[reg] * 5;

	return true;
}


//Reg Dump
bool Chip8::_FX55(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	for (int i = 0; i < reg + 1; i++) {
		memory[iRegister + i] = registers[i];
	}


	return true;
}


//Reg Load
bool Chip8::_FX65(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	for (int i = 0; i < reg + 1; i++) {
		registers[i] = memory[iRegister + i];
	}


	return true;
}

//Vx = get_delay()
bool Chip8::_FX07(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	registers[reg] = (int)delayTimerRaw;

	return true;
}

//delay_timer(Vx)
bool Chip8::_FX15(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	delayTimerRaw = (double)registers[reg];

	return true;
}

//sound_timer(Vx)
bool Chip8::_FX18(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	soundTimerRaw = (double)registers[reg];

	return true;
}

//BCD
bool Chip8::_FX33(int opcode) {
	int reg = (opcode & 0x0F00) >> 8;

	int regValue = registers[reg];

	memory[iRegister] = regValue / 100;
	memory[iRegister + 1] = (regValue % 100) / 10;
	memory[iRegister + 2] = (regValue % 10);


	return true;
}


