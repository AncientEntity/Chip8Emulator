#include "main.h"

Chip8::Chip8() { Init(); }
Chip8::~Chip8()
{
	delete[] memory;
	delete[] registers;
	delete[] stack;
}

void Chip8::Init() {
	for (int i = 0; i < 4096; i++) { memory[i] = 0; }
	for (int i = 0; i < 16; i++) { registers[i] = 0; stack[i] = 0; }
	addressRegister = 0;

	std::cout << "Chip8 Initialized" << std::endl;
}

int Chip8::Tick() {

	opcode = (memory[programCounter] << 8) + memory[programCounter + 1];
	std::cout << std::hex << opcode << std::endl;

	bool instructResult = _ZZZZ(opcode); //Opcode Top Level Filter

	programCounter += 2;
	if (programCounter >= 4096) {
		return -1;
	}
	if (instructResult) { return 0; }
	else { return -2; }
}

void Chip8::DebugOpCodeTest() {

	//Top Level Filter Check
	//OverrideMemory(0x200, 0x02); //Test _0ZZZ Filter
	//OverrideMemory(0x201, 0x03);

	OverrideMemory(0x200, 0x80); //Test _8ZZZ Filter
	OverrideMemory(0x201, 0x00);

	OverrideMemory(0x202, 0x39); //Test 3XNN
	OverrideMemory(0x203, 0x00);

	OverrideMemory(0x204, 0xF0); 
	OverrideMemory(0x205, 0x00);



	//Jump to start opcode
	OverrideMemory(0x206, 0x02); //Jump to 0x200 (aka beginning)
	OverrideMemory(0x207, 0x00);
}

void Chip8::OverrideMemory(int index, int8_t val) {
	memory[index] = val;
}

int Chip8::GetCurrentInstruction(int offset) {
	return (memory[programCounter + offset * 2] << 8) + memory[programCounter + 1 + offset * 2];
}

int** Chip8::CreateArray(int x, int y) {
	int** arr = new int* [x];
	for (int i = 0; i < x; i++) {
		arr[i] = new int[y];
		for (int j = 0; j < y; j++) {
			arr[i][j] = 0;
		}
	}

	return arr;
}

void Chip8::DestroyArray(int** arr, int x, int y) {
	for (int i = 0; i < x; i++) {
		delete arr[x];
	}
	delete[] arr;
}