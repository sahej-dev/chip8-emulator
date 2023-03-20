#include <iostream>

#include "color.h"
#include "emuGL.h"
#include "window.h"
#include "chip8.h"
#include "emulator.h"

int main(int argc, char * argv[])
{
    // using namespace emuGL;

    // Window win {"Heading", 600, 400};
    // win.setBgColor(Colors::black);

    // Rect r{200, 100, {0, 0}};
    // r.setFillColor(Colors::pastelBlue);

    // win.attach(r);

    // win.show();


    // chip8::Emulator e{10, "./roms/IBM_Logo.ch8"};
    // chip8::Emulator e{10, "./roms/test_opcode.ch8"};
    chip8::Emulator e{15, "./roms/chip8-test-suite.ch8"};
    // chip8::Emulator e{10, "./roms/Brick.ch8"};
    // chip8::Emulator e{10, "./roms/Keypad_Test.ch8"};
    // chip8::Emulator e{10, "./roms/Chip8_Logo.ch8"};
    // chip8::Emulator e{10, "./roms/Chip8_Picture.ch8"};
    // chip8::Emulator e{10, "/Users/sahej/Documents/Projects/Emulators/Chip8/roms/test_opcode.ch8"};

    e.run();

}
