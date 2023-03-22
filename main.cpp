#include <iostream>

#include "color.h"
#include "emuGL.h"
#include "window.h"
#include "chip8.h"
#include "emulator.h"

int main(int argc, char * argv[])
{

    // chip8::Emulator e{15, "./roms/chip8-test-suite.ch8"};
    chip8::Emulator e{15, "./roms/Brick.ch8"};

    e.run();

}
