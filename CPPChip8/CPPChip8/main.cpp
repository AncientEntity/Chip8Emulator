// CPPChip8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"

static const char* fileName = "ROMS\\randomnumber.ch8";


int main(int argc, char* argv[])
{
    Chip8 *chip = new Chip8();
    chip->SetDebug(false);
    //chip->DebugOpCodeTest();
    chip->LoadROM(fileName);
    //chip->OverrideMemory(0x200, 0xF0);
    //chip->OverrideMemory(0x201, 0x0A);
    //chip->OverrideMemory(0x202, 0x12);
    //chip->OverrideMemory(0x203, 0x00);

    Display* display = new Display();
    display->Init();
    
    chip->SetInput(&display->keyStates[0]);

    bool running = true;

    double tickDelta = 0;
    uint32_t lastTickStart = SDL_GetTicks();

    int i = 0;

    while (running) {
        int eventResult = display->HandleEvents();
        if (eventResult == 1) { running = 0; }
        int tickResult = chip->Tick(tickDelta);
        switch(tickResult) {
        case (-1):
            std::cout << "CHIP-8 ERROR Memory Out Of Bounds Exception" << std::endl;
            running = 0;
        
        case(-2):
            std::cout << "CHIP-8 ERROR Instruction Failed: " << std::hex << chip->GetCurrentInstruction(-1) << std::endl;
            break;
        default:
            break;
        }

        if (chip->displayChanged) {
            display->Render(chip->GetDisplayBuffer());
            chip->displayChanged = false;
            //std::cout << "Display Updated" << std::endl;
        }

        uint32_t newTime = SDL_GetTicks();
        tickDelta = (newTime - lastTickStart) / 1000.0;
        lastTickStart = newTime;

        //std::cin.get();
        //SDL_Delay(100);

        i += 1;
    }

    display->Close();
    delete chip;
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
