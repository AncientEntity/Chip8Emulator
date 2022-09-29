// CPPChip8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"

int main(int argc, char* argv[])
{
    Chip8 *chip = new Chip8();
    chip->DebugOpCodeTest();

    Display* display = new Display();
    display->Init();
    
    bool running = true;
    while (running) {
        int eventResult = display->HandleEvents();
        if (eventResult == 1) { running = 0; }
        int tickResult = chip->Tick();
        if (tickResult == -1) {
            std::cout << "CHIP-8 ERROPRMemory Out Of Bounds Exception" << std::endl;
            running = 0;
        }
        SDL_Delay(500);
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