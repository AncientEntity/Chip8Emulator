#pragma once

class Chip8 {
public:
	Chip8();
	~Chip8();

	void Init();
	int Tick(double timePast);
	void SetInput(int* keyStateArray) { keyStates = keyStateArray; }

	uint8_t* GetDisplayBuffer();
	int GetCurrentInstruction(int offset);

	void LoadROM(const char* fileName);

	void OverrideMemory(int index, int8_t val); //For easy memory injection for debugging opcodes.
	void DebugOpCodeTest();						//Injects memory with a few opcodes to test.
	void SetDebug(bool b) { debugMode = b; } 

	bool displayChanged = false;

private:
	uint8_t memory[4096];
	uint8_t registers[16];
	int addressRegister;
	int stack[16];
	int stackDepth = 0;
	int *keyStates;

	int16_t iRegister = 0;
	int programCounter = 0x200;

	int16_t opcode = 0;

	//int soundTimer = 0;
	double soundTimerRaw = 0;
	//int delayTimer = 0;
	double delayTimerRaw = 0;


	bool debugMode = false;


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

	bool _EX9E(int opcode);
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



	//By default for chip-8 memory 0x000 to 0x1FF is font sprites
	//Gets put into memory in Init()
	int fontSet[80] = { 0xF0, 0x90, 0x90, 0x90, 0xF0,  // Zero
						  0x20, 0x60, 0x20, 0x20, 0x70, // One
						  0xF0, 0x10, 0xF0, 0x80, 0xF0, // Two
						  0xF0, 0x10, 0xF0, 0x10, 0xF0, // Three
						  0x90, 0x90, 0xF0, 0x10, 0x10, // Four
						  0xF0, 0x80, 0xF0, 0x10, 0xF0, // Five
						  0xF0, 0x80, 0xF0, 0x90, 0xF0, // Six
						  0xF0, 0x10, 0x20, 0x40, 0x40, // Seven
						  0xF0, 0x90, 0xF0, 0x90, 0xF0, // Eight
						  0xF0, 0x90, 0xF0, 0x10, 0xF0, // Nine
						  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
						  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
						  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
						  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
						  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
						  0xF0, 0x80, 0xF0, 0x80, 0x80}; //F


	uint8_t _GetKey();
	bool _IsPressed(int c);

};

enum ChipError {
	NoError,
	UnknownOpCode,
	MemoryOutOfBounds,
};