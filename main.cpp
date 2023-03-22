#include <iostream>

#include "emulator.h"

const unsigned int SCALE_FACTOR = 15;

int main(int argc, char * argv[])
{

    chip8::Emulator e{SCALE_FACTOR, "./roms/Space Invaders [David Winter].ch8"};
    // chip8::Emulator e{SCALE_FACTOR, "./roms/Brick.ch8"};

    e.run();

}
