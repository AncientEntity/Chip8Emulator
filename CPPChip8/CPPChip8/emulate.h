#pragma once

class Chip8 {
public:
	Chip8();
	~Chip8();

	void Init();
	int Tick();

	int8_t* GetDisplayBuffer();
	int GetCurrentInstruction(int offset);

	void OverrideMemory(int index, int8_t val); //For easy memory injection for debugging opcodes.
	void DebugOpCodeTest();						//Injects memory with a few opcodes to test.
private:
	int8_t memory[4096];
	uint8_t registers[16];
	int addressRegister;
	int stack[16];
	int stackDepth = 0;

	int iRegister = 0;
	int programCounter = 0x200;

	int16_t opcode = 0;

	int soundTimer = 0;
	int delayTimer = 0;


	int** CreateArray(int x, int y);
	void DestroyArray(int** arr, int x, int y);

	//Opcode Filter Functions
	bool _ZZZZ(int opcode); //Top Level
	bool _0ZZZ(int opcode);
	bool _8ZZZ(int opcode);
	bool _EZZZ(int opcode);
	bool _FZZZ(int opcode);

	//Actual Opcode Functions
	bool _0NNN(int opcode);
	bool _00E0(int opcode);
	bool _00EE(int opcode);

	bool _1NNN(int opcode);
	bool _2NNN(int opcode);
	bool _3XNN(int opcode);
	bool _4XNN(int opcode);
	bool _5XY0(int opcode);
	bool _6XNN(int opcode);
	bool _7XNN(int opcode);

	bool _8XY0(int opcode);
	bool _8XY1(int opcode);
	bool _8XY2(int opcode);
	bool _8XY3(int opcode);
	bool _8XY4(int opcode);
	bool _8XY5(int opcode);
	bool _8XY6(int opcode);
	bool _8XY7(int opcode);
	bool _8XYE(int opcode);
	bool _9XY0(int opcode);

	bool _ANNN(int opcode);
	bool _BNNN(int opcode);
	bool _CXNN(int opcode);
	bool _DXYN(int opcode);

	bool _EX8E(int opcode);
	bool _EXA1(int opcode);

	bool _FX07(int opcode);
	bool _FX0A(int opcode);
	bool _FX15(int opcode);
	bool _FX18(int opcode);
	bool _FX1E(int opcode);
	bool _FX29(int opcode);
	bool _FX33(int opcode);
	bool _FX55(int opcode);
	bool _FX65(int opcode);




};

enum ChipError {
	NoError,
	UnknownOpCode,
	MemoryOutOfBounds,
};