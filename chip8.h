#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <stdexcept>

namespace chip8
{
    /**
     * @throws Runtime Exception:
     * If val cannot fit in an integer of b bits
     * or if b > 16
    **/
    void checkValSize(uint16_t val, uint8_t b);

    class Memory
    {
    public:
        inline static const uint16_t c_fontStartAddr = 0x050;

        Memory(std::ifstream& rom) : Memory(rom, c_defaultFont) { }

        Memory(std::ifstream& rom, std::vector<uint8_t> font)
            :   m_font(font)
        {
            // initialising font
            for (uint16_t i = 0; i < m_font.size(); i++)
                m_ram[i + c_fontStartAddr] = m_font[i];

            // initialising rom
            if (!rom.good())
                throw std::runtime_error("ERROR: Unable to load rom!");

            char ch {};
            for (uint16_t i = c_romStartAddr; rom.get(ch); i++)
                m_ram.at(i) = static_cast<uint8_t>(ch);
        }


        /**
         * @param addr should be a 12 bit int < (1 << 12)
         * @returns a uint8_t value at addr
        **/
        uint8_t read (int addr) { checkValSize(addr, c_addrBits); return m_ram[addr]; }

        /**
         * @param addr should be a 12 bit int
         * @param val should be a uint8_t
        **/
        void write(uint16_t addr, uint8_t val) { checkValSize(addr, c_addrBits ); m_ram[addr] = val; }


        Memory(const Memory&) = delete;
        Memory& operator=(const Memory&) = delete;

        static uint16_t romStartAddress() { return c_romStartAddr; }

    private:
        static constexpr uint k_sizeKB = 4;

        inline static const uint8_t c_addrBits =    // no. of bits in a mem addr
            static_cast<uint8_t>(ceil(log2(k_sizeKB * 1024)));

        inline static const uint16_t c_romStartAddr  = 0x200;

        inline static const std::vector<uint8_t> c_defaultFont {
            0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
            0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
            0x90, 0x90, 0xf0, 0x10, 0x10, // 4
            0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
            0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
            0xf0, 0x10, 0x20, 0x40, 0x40, // 7
            0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
            0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
            0xf0, 0x90, 0xf0, 0x90, 0x90, // A
            0xe0, 0x90, 0xe0, 0x90, 0xe0, // B
            0xf0, 0x80, 0x80, 0x80, 0xf0, // C
            0xe0, 0x90, 0x90, 0x90, 0xe0, // D
            0xf0, 0x80, 0xf0, 0x80, 0xf0, // E
            0xf0, 0x80, 0xf0, 0x80, 0x80  // F
        };

        std::vector<uint8_t> m_ram{ std::vector<uint8_t>(k_sizeKB * 1024, 0) };

        std::vector<uint8_t> m_font;
    };

    class Cpu
    {
    public:

        /**
         * @param addr should be a 4 bit int
         * @returns a uint8_t value at addr
        **/
        uint8_t readRegister(uint8_t addr) { checkValSize(addr, c_registerBits); return m_registers[addr]; }

        /**
         * @param addr should be a 4 bit int
         * @param val should be a uint8_t
        **/
        void writeRegister(uint8_t addr, uint8_t val) { checkValSize(addr, c_registerBits); m_registers[addr] = val; }

        uint16_t readI() { return m_I; }
        void writeI(uint16_t val) { m_I = val; }

        uint16_t readPC() { return m_PC; }
        void writePC(uint16_t val) { m_PC = val; }
        void incrementPC() { m_PC += 2; }

        uint8_t delayTimer() { return m_delayTimer; }
        void setDelayTimer(uint8_t val) { m_delayTimer = val; }

        uint8_t soundTimer() { return m_soundTimer; }
        void setSoundTimer(uint8_t val) { m_soundTimer = val; }

        void pushStack(uint16_t addr) { m_callStack.push(addr); }
        uint16_t peekStack() { return m_callStack.top(); }
        void popStack() { m_callStack.pop(); }


    private:
        static constexpr uint8_t k_registerCount = 16;
        static constexpr uint8_t k_iPcBits = 16;        // no. of bits in I & PC registers each
        static constexpr uint8_t k_timerBits = 8;       // no. of bits in delay & sound timers each

        inline static const uint8_t c_registerBits =    // no. of bits in a register addr
            static_cast<uint8_t>(ceil(log2(k_registerCount)));


        std::vector<uint8_t> m_registers{ std::vector<uint8_t>(k_registerCount, 0) };

        uint16_t m_I  {}; // used to store memory addrs'
        uint16_t m_PC {}; // program counter

        uint8_t m_delayTimer {}; // decrements by 1 if > 0 at 60Hz
        uint8_t m_soundTimer {}; // decrements by 1 if > 0 at 60Hz
        
        std::stack<uint16_t> m_callStack {};
    };

    class Display
    {
    public:
        Display() : Display(false) { }

        Display(bool isSuperChip)
            :   m_isSuperChip(isSuperChip),
                m_width(isSuperChip ? 128 : 64),
                m_height(isSuperChip ? 64 : 32)
        {
            // initialising bool matrix
            m_screen.resize(m_height);
            for (std::vector<bool>& row : m_screen)
                row.resize(m_width, false);
        }

        Display(const Display&) = delete;
        Display& operator=(const Display&) = delete;

        uint16_t width() { return m_width; }
        uint16_t height() { return m_height; }
        const std::vector< std::vector<bool> >& screenBuffer() { return m_screen; }

        /**
         * attaches sprite in-memory.
         * @returns
         *  true if register F is to be set to 1, false if it is to be set to 0
        */
        bool attachSprite(const std::vector<uint8_t>& sprite, uint8_t x, uint8_t y);

        void clear()
        {
            for (uint8_t i = 0; i < m_screen.size(); i++)
                for (uint8_t j = 0; j < m_screen[i].size(); j++)
                    m_screen[i][j] = false;
        }

    private:
        uint16_t m_width;
        uint16_t m_height;

        bool m_isSuperChip;

        std::vector< std::vector<bool> > m_screen;

    };

    class Keypad
    {
    public:
        void pressKey(uint8_t key) { checkValSize(key, c_keysMaxBits); m_isPressed[key] = true; }
        void releaseKey(uint8_t key) { checkValSize(key, c_keysMaxBits); m_isPressed[key] = false; }
        bool isPressed(uint8_t key) { checkValSize(key, c_keysMaxBits); return m_isPressed[key]; }

    private:
        static constexpr uint8_t k_nOfKeys = 16;
        inline static const uint8_t c_keysMaxBits =     // max no. of bits in k_nOfKeys
            static_cast<uint8_t>(ceil(log2(k_nOfKeys)));

        std::vector<bool> m_isPressed { std::vector<bool>(k_nOfKeys, false) };
    };
}

#endif /* CHIP8_H */
