- Data Sheet: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
- Blogpost: https://tobiasvl.github.io/blog/write-a-chip-8-emulator
- ROMS: https://github.com/loktar00/chip8/tree/master/roms
- Testing: https://github.com/corax89/chip8-test-rom

## Models:
### Dumb component:
- chip8 components

### Memory:
- is Dumb Component
- Read/Write to/from memory

### Emulator
- Maps Chip8 Instruction to method/function calls
- this 'has' a memory, CPU, inputHandler, Display etc.
- contains config data like: display scale, 

### CPU
- is Dumb Component
- Reads/Writes to/from registers
- Arithmetic & Logical Operations

### Display
- is Dumb Component
- contains screen buffer
- basically chip8 display functionality

### Keyboard
- is Dumb Component
- contains key press buffer


## Drivers
These take chip 8 dumb components & interface them with host system

### Display Driver
- takes a chip8 display & uses emuGL to render a window

### Keyboard Driver
- provides physical key presses on host, to chip8 keyboard, in a way that it can understand
