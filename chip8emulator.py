import random, pygame, time
DO_LOGGING = True


class CHIP8:
    def __init__(self):
        self.memory = [0] * 4096
        self.gpio = [0] * 16
        self.display_buffer = [0]*32*64
        self.keyInputs = [0]*16

        self.shouldDraw = False

        self.soundTimer = 0
        self.delayTimer = 0
        self.hiddenDelayTimer = 0
        self.hiddenSoundTimer = 0
        self.lastTickTime = time.time()

        self.index = 0 #I Register
        self.stack = []

        self.programCounter = 0x200

        self.opcode = 0

        self.funcMap = {
            0x0000: self._0ZZZ,
            0x00E0: self._00E0,
            0x00EE: self._00EE,
            0x1000: self._1NNN,
            0x2000: self._2NNN,
            0x3000: self._3XNN,
            0x4000: self._4XNN,
            0x5000: self._5XY0,
            0x6000: self._6XNN,
            0x7000: self._7XNN,
            0x8000: self._8ZZZ,
            0x8001: self._8XY1,
            0x8002: self._8XY2,
            0x8003: self._8XY3,
            0x8004: self._8XY4,
            0x8005: self._8XY5,
            0x8006: self._8XY6,
            0x8007: self._8XY7,
            0x800E: self._8XYE,
            0x9000: self._9XY0,
            0xA000: self._ANNN,
            0xB000: self._BNNN,
            0xC000: self._CXNN,
            0xD000: self._DXYN,
            0xE000: self._EZZZ,
            0xE09E: self._EX9E,
            0xE0A1: self._EXA1,
            0xF000: self._FZZZ,
            0xF007: self._FX07,
            0xF00A: self._FX0A,
            0xF015: self._FX15,
            0xF018: self._FX18,
            0xF01E: self._FX1E,
            0xF029: self._FX29,
            0xF033: self._FX33,
            0xF055: self._FX55,
            0xF065: self._FX65,
        }

        self.vx = 0
        self.vy = 0

        self._font_set = [0xF0, 0x90, 0x90, 0x90, 0xF0, # Zero
                          0x20, 0x60, 0x20, 0x20, 0x70, # One
                          0xF0, 0x10, 0xF0, 0x80, 0xF0, # Two
                          0xF0, 0x10, 0xF0, 0x10, 0xF0, # Three
                          0x90, 0x90, 0xF0, 0x10, 0x10, # Four
                          0xF0, 0x80, 0xF0, 0x10, 0xF0, # Five
                          0xF0, 0x80, 0xF0, 0x90, 0xF0, # Six
                          0xF0, 0x10, 0x20, 0x40, 0x40, # Seven
                          0xF0, 0x90, 0xF0, 0x90, 0xF0, # Eight
                          0xF0, 0x90, 0xF0, 0x10, 0xF0, # Nine
                          0xF0, 0x90, 0xF0, 0x90, 0x90, # A
                          0xE0, 0x90, 0xE0, 0x90, 0xE0, # B
                          0xF0, 0x80, 0x80, 0x80, 0xF0, # C
                          0xE0, 0x90, 0x90, 0x90, 0xE0, # D
                          0xF0, 0x80, 0xF0, 0x80, 0xF0, # E
                          0xF0, 0x80, 0xF0, 0x80, 0x80, # F
        ]
        for i in range(len(self._font_set)):
            self.memory[i] = self._font_set[i]
        self.waitingOnInput = False
        self._displayScale = 5
        self.display = pygame.display.set_mode((64*self._displayScale,32*self._displayScale))
    def dump_reg(self):
        print(f"PC: {self.programCounter} | index: {self.index} | GPIO: {self.gpio}")
    def loadRaw(self,raw):
        for key, byte in enumerate(raw):
            self.memory[0x200 + key] = byte
    def loadRom(self,romFile):
        romFile = open(romFile,"rb")



        for key, byte in enumerate(romFile.read()):
            self.memory[0x200 + key] = byte
        romFile.close()
    def emulateKeyboard(self):
        self.keyInputs = [0]*16
        keyboardInputs = pygame.key.get_pressed()
        if (keyboardInputs[pygame.K_0]):
            self.keyInputs[0] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 0
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_1]):
            self.keyInputs[1] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 1
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_2]):
            self.keyInputs[2] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 2
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_3]):
            self.keyInputs[3] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 3
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_4]):
            self.keyInputs[4] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 4
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_5]):
            self.keyInputs[5] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 5
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_6]):
            self.keyInputs[6] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 6
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_7]):
            self.keyInputs[7] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 7
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_8]):
            self.keyInputs[8] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 8
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_9]):
            self.keyInputs[9] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 9
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_a]):
            self.keyInputs[10] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 10
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_b]):
            self.keyInputs[11] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 11
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_c]):
            self.keyInputs[12] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 12
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_d]):
            self.keyInputs[13] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 13
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_e]):
            self.keyInputs[14] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 14
                self.waitingOnInput = False
        if (keyboardInputs[pygame.K_f]):
            self.keyInputs[15] = 1
            if(self.waitingOnInput):
                self.gpio[self.vx] = 15
                self.waitingOnInput = False
    def displayIndex(self,x,y):
        return (x*64)+y
    def draw(self):
        if(self.shouldDraw):
            for x in range(32):
                for y in range(64):
                    if(self.display_buffer[self.displayIndex(x,y)] == 1):
                        pygame.draw.rect(self.display,(255,255,255),(y*self._displayScale,x*self._displayScale,self._displayScale,self._displayScale))
                    else:
                        pygame.draw.rect(self.display,(0,0,0),(y*self._displayScale,x*self._displayScale,self._displayScale,self._displayScale))
            pygame.display.update()
            self.shouldDraw = False
    def tick(self):

        pygame.event.get() #Prevents pygame from imploding like a dying star.

        if(self.waitingOnInput):
            self.emulateKeyboard()
            return
        else:
            self.emulateKeyboard()

        self.opcode = (self.memory[self.programCounter] << 8) + self.memory[self.programCounter + 1]
        extracted_op = self.opcode & 0xF000

        delta = time.time() - self.lastTickTime
        self.lastTickTime = time.time()
        if(self.delayTimer > 0):
            self.hiddenDelayTimer -= delta * 60
            self.delayTimer = int(self.hiddenDelayTimer)
        if(self.soundTimer > 0):
            self.hiddenSoundTimer -= delta * 60
            self.soundTimer = int(self.hiddenSoundTimer)

        self.vx = (self.opcode & 0x0F00) >> 8
        self.vy = (self.opcode & 0x00F0) >> 4
        #print("VX: ",self.vx,"|VY: ",self.vy)

        #try:
        self.funcMap[extracted_op]()
        #except Exception as e:
        #    log("ERR: Unknown Instruction: "+str(hex(self.opcode))+str(e),True)
        self.programCounter += 2
        self.draw()

    def _0ZZZ(self):
        extracted_op = self.opcode & 0x00FF

        try:
            self.funcMap[extracted_op]()
        except Exception as e:
            log("ERR: Unknown Instruction: "+str(hex(self.opcode))+str(e),True)
    def _00E0(self): #Display: Clear Display
        log("Clearing Screen")
        self.display_buffer = [0]*32*64
        self.shouldDraw = True
    def _00EE(self): #Flow: Return from a subroutine
        self.programCounter = self.stack.pop()
    def _1NNN(self): #Flow: go to NNN
        log("Jumping to address NNN")
        self.programCounter = (self.opcode & 0x0FFF) - 2
    def _2NNN(self):
        log("Calls subroutine at NNN")
        self.stack.append(self.programCounter)
        self.programCounter = (self.opcode & 0x0FFF) - 2
    def _3XNN(self):
        log("Skip next instruction if Vx == NN")
        if self.gpio[self.vx] == self.opcode & 0x00FF:
            self.programCounter += 2
    def _4XNN(self):
        log("IF VX is not equal to NN skip next instruciton")
        if self.gpio[self.vx] != self.opcode & 0x00FF:
            self.programCounter += 2
    def _5XY0(self):
        log("If VX is equal to VY skip next instruction")
        if(self.gpio[self.vx] == self.gpio[self.vy]):
            self.programCounter += 2
    def _6XNN(self):
        log("Set VX to NN")
        self.gpio[self.vx] = self.opcode & 0x00FF
    def _7XNN(self):
        log("Adds NN to VX")
        self.gpio[self.vx] = (self.gpio[self.vx] + (self.opcode & 0x00FF)) & 0x00FF
    def _8ZZZ(self):
        extracted_opcode = self.opcode & 0xF00F
        if(extracted_opcode == 0x8000):
            self._8XY0(); return;

        try:
            self.funcMap[extracted_opcode]()
        except Exception as e:
            log("ERR: Unknown Instruction: " + str(hex(self.opcode)) + str(e),True)
    def _8XY0(self):
        log("Set VX to the value of VY")
        self.gpio[self.vx] = self.gpio[self.vy]
    def _8XY1(self):
        log("Sets VX to VX or VY Bitwise")
        self.gpio[self.vx] = self.gpio[self.vx] | self.gpio[self.vy]
    def _8XY2(self):
        log("Sets VX to VX and VY Bitwise")
        self.gpio[self.vx] = self.gpio[self.vx] & self.gpio[self.vy]
    def _8XY3(self):
        log("Sets VX to VX xor VY Bitwise")
        self.gpio[self.vx] = self.gpio[self.vx] ^ self.gpio[self.vy]
    def _8XY4(self):
        log("Adds VY to VX, set VF = Carry")
        if(self.gpio[self.vx] + self.gpio[self.vy] >> 8 != 0):
            self.gpio[0xF] = 1
        else:
            self.gpio[0xF] = 0
        self.gpio[self.vx] = (self.gpio[self.vx] + self.gpio[self.vy]) & 0xFF
    def _8XY5(self):
        log("Subtract VY from VX, VF = Not BORROW")
        if(self.gpio[self.vx] < self.gpio[self.vy]):
            self.gpio[0xF] = 0
        else:
            self.gpio[0xF] = 1
        self.gpio[self.vx] = (self.gpio[self.vx] - self.gpio[self.vy]) & 0xFF
    def _8XY6(self):
        log("Store LSB of VX in VF then shift VX to right by 1")
        self.gpio[0xF] = self.gpio[self.vx] & 0x0001
        self.gpio[self.vx] = (self.gpio[self.vx] >> 1) & 0x00FF
    def _8XY7(self):
        log("Set VX to VY minus VX. VF = NOT BORROW")
        if(self.gpio[self.vy] < self.gpio[self.vx]):
            self.gpio[0xF] = 0
        else:
            self.gpio[0xF] = 1
        self.gpio[self.vx] = (self.gpio[self.vy] - self.gpio[self.vx]) & 0xFF
    def _8XYE(self):
        log("Store MSB of VX in VF then shift VX to right by 1")
        self.gpio[0xF] = (self.gpio[self.vx] & 0x00F0) >> 7
        self.gpio[self.vx] = (self.gpio[self.vx] << 1) & 0xFF
    def _9XY0(self):
        log("Skips next instruction if VX != VY")
        if(self.gpio[self.vx] != self.gpio[self.vy]):
            self.programCounter += 2
    def _ANNN(self):
        log("Set I to address NNN")
        self.index = self.opcode & 0xFFF
    def _BNNN(self):
        log("Jumps to the address NNN plus V0")
        self.programCounter = (self.opcode & 0x0FFF) + (self.gpio[0] & 0xFF) - 2
    def _CXNN(self):
        log("Setting VX to random() & NN")
        self.gpio[self.vx] = random.randint(0,255) & (self.opcode & 0xFF)
    def _DXYN(self):
        log("Draw a sprite")
        self.gpio[0xf] = 0
        x = self.gpio[self.vx] & 0xff
        y = self.gpio[self.vy] & 0xff
        height = self.opcode & 0x000f
        row = 0
        while row < height:
            curr_row = self.memory[row + self.index]
            pixel_offset = 0
            while pixel_offset < 8:
                loc = x + pixel_offset + ((y + row) * 64)
                pixel_offset += 1
                if (y + row) >= 32 or (x + pixel_offset - 1) >= 64:
                    # ignore pixels outside the screen
                    continue
                mask = 1 << 8-pixel_offset
                curr_pixel = (curr_row & mask) >> (8-pixel_offset)
                self.display_buffer[loc] ^= curr_pixel
                if self.display_buffer[loc] == 0:
                    self.gpio[0xf] = 1
                else:
                    self.gpio[0xf] = 0
            row += 1
        self.shouldDraw = True
    def _EZZZ(self):
        extracted_opcode = self.opcode & 0xF0FF
        try:
            self.funcMap[extracted_opcode]()
        except Exception as e:
            log("ERR: Unknown Instruction: " + str(hex(self.opcode)) + str(e), True)
    def _EX9E(self):
        log("Skip next instruction if key stored in VX is pressed")
        if(self.keyInputs[self.gpio[self.vx]] == 1):
            self.programCounter += 2
    def _EXA1(self):
        log("Skip next instruction if key stored in VX is not pressed")
        if(self.keyInputs[self.gpio[self.vx]] == 0):
            self.programCounter += 2
    def _FZZZ(self):
        extracted_opcode = self.opcode & 0xF0FF
        try:
            self.funcMap[extracted_opcode]()
        except Exception as e:
            log("ERR: Unknown Instruction: " + str(hex(self.opcode)) + str(e), True)
    def _FX07(self):
        log("VX = Delay Timer")
        self.gpio[self.vx] = self.delayTimer
    def _FX0A(self):
        log("Awaiting for input then putting it into Vx")
        self.waitingOnInput = True
    def _FX15(self):
        log("Sets the delay timer to VX")
        self.delayTimer = self.gpio[self.vx]
        self.hiddenDelayTimer = self.gpio[self.vx]
    def _FX18(self):
        log("Sets the sound timer to VX")
        self.soundTimer = self.gpio[self.vx]
        self.hiddenSoundTimer = self.gpio[self.vx]
    def _FX1E(self):
        log("Adds VX to I")
        self.index = (self.index + self.gpio[self.vx]) & 0x0FFF
    def _FX29(self):
        log("Sets I to sprite address for character Vx")
        self.index = (self.gpio[self.vx] * 5) & 0x0FFF
    def _FX33(self):
        self.memory[self.index] = self.gpio[self.vx] // 100
        self.memory[self.index+1] = (self.gpio[self.vx] % 100) // 10
        self.memory[self.index+2] = self.gpio[self.vx] % 10
    def _FX55(self):
        for i in range(self.vx+1):
            self.memory[self.index+i] = self.gpio[i]
    def _FX65(self):
        for i in range(self.vx+1):
            self.gpio[i] = self.memory[self.index+i]
def log(text, force=False):
    if(DO_LOGGING or force):
        print("C8: "+text)

DO_LOGGING = False
pygame.init()
c = CHIP8()
c.loadRom("ROMS/tictactoe.ch8")
while True:
    c.tick()
