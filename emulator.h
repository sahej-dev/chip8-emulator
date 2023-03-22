#ifndef EMULATOR_H
#define EMULATOR_H

#include <iostream>
#include <fstream>
#include <ios>
#include <chrono>

#include "chip8.h"
#include "window.h"
#include "drivers.h"

namespace chip8
{
    class Emulator
    {
    public:
        Emulator(uint16_t scale, std::string romPath)
            :   m_romStream{romPath, std::ios::in | std::ios::binary},
                m_chip8Memory(new chip8::Memory{m_romStream}),
                m_chip8Cpu(new chip8::Cpu{}),
                m_chip8Keypad(new chip8::Keypad{}),
                m_chip8Display(new chip8::Display{}),
                m_displayDriver(new drivers::Display{scale, m_chip8Display->width(), m_chip8Display->height()}),
                m_inputDriver(new drivers::Input{})
        { }

        ~Emulator()
        {
            delete m_chip8Display;
            delete m_chip8Memory;
            delete m_chip8Cpu;
            delete m_chip8Keypad;

            delete m_displayDriver;
            delete m_inputDriver;
        }

        void run()
        {
            std::chrono::time_point<std::chrono::steady_clock> fpsTimer { std::chrono::steady_clock::now() };
	        std::chrono::duration<int32_t, std::ratio<1, 60>> FPS {};

            uint16_t pc = m_chip8Memory->romStartAddress();
            m_chip8Cpu->writePC(pc);

            while (!m_inputDriver->shouldQuit())
            {
                FPS = std::chrono::duration_cast<std::chrono::duration<int32_t, std::ratio<1, 60>>>(
                    std::chrono::steady_clock::now() - fpsTimer
                );

                if (FPS.count() >= 1)
                {
                    fpsTimer = std::chrono::steady_clock::now();

                    const uint8_t dt = m_chip8Cpu->delayTimer();
                    if (dt > 0)
                        m_chip8Cpu->setDelayTimer(dt - 1);

                    const uint8_t st = m_chip8Cpu->soundTimer();
                    if (st > 0)
                        m_chip8Cpu->setSoundTimer(st - 1);
                }

                pc = m_chip8Cpu->readPC();

                uint16_t instruction = m_chip8Memory->read(pc);
                instruction <<= 8;
                instruction |= m_chip8Memory->read(pc + 1);

                std::cout << std::hex << pc << " : " <<  instruction << '\n';

                execute(instruction);

                m_inputDriver->updateKeyStates(m_chip8Keypad);
                m_displayDriver->updateDisplay(m_chip8Display);

            }
        }
        
    private:
        std::ifstream m_romStream;

        chip8::Display* m_chip8Display {};
        chip8::Memory* m_chip8Memory {};
        chip8::Cpu* m_chip8Cpu {};
        chip8::Keypad* m_chip8Keypad {};

        drivers::Display* m_displayDriver {};
        drivers::Input* m_inputDriver {};

        void execute(const uint16_t instruction)
        {
            bool shallInc = true;
            
            switch (instruction & 0xF000)
            {
            case 0x0:
                switch (instruction)
                {
                case 0xE0:
                        m_chip8Display->clear();
                    break;
                
                case 0xEE:
                    m_chip8Cpu->writePC(m_chip8Cpu->peekStack());
                    m_chip8Cpu->popStack();
                    break;
                
                default:
                    break;
                }
                break;
            
            case 0x1000:
                m_chip8Cpu->writePC(instruction & 0x0FFF);
                shallInc = false;
                break;

            case 0x2000:
                m_chip8Cpu->pushStack(m_chip8Cpu->readPC());
                m_chip8Cpu->writePC(instruction & 0x0FFF);
                shallInc = false;
                break;

            case 0x3000:
                if (m_chip8Cpu->readRegister((instruction & 0x0F00) >> 8) == (instruction & 0xFF))
                    m_chip8Cpu->incrementPC();
                break;

            case 0x4000:
                if (m_chip8Cpu->readRegister((instruction & 0x0F00) >> 8) != (instruction & 0xFF))
                    m_chip8Cpu->incrementPC();
                break;

            case 0x5000:
                if (m_chip8Cpu->readRegister((instruction & 0x0F00) >> 8) == m_chip8Cpu->readRegister((instruction & 0xF0) >> 4))
                    m_chip8Cpu->incrementPC();
                break;

            case 0x6000:
                m_chip8Cpu->writeRegister((instruction & 0x0F00) >> 8, instruction & 0xFF);
                break;
            
            case 0x7000:
                {
                uint8_t reg = (instruction & 0xF00) >> 8;
                m_chip8Cpu->writeRegister(reg, m_chip8Cpu->readRegister(reg) + (instruction & 0xFF));
                }
                break;

            case 0x8000:
                {

                uint8_t regX = (instruction & 0x0F00) >> 8;
                uint8_t regY = (instruction & 0xF0) >> 4;

                uint8_t valX = m_chip8Cpu->readRegister(regX);
                uint8_t valY = m_chip8Cpu->readRegister(regY);

                switch (instruction & 0xF)
                {
                case 0x0:
                    m_chip8Cpu->writeRegister(regX, valY);
                    break;
                
                case 0x1:
                    m_chip8Cpu->writeRegister(regX, valX | valY);
                    break;

                case 0x2:
                    m_chip8Cpu->writeRegister(regX, valX & valY);
                    break;

                case 0x3:
                    m_chip8Cpu->writeRegister(regX, valX ^ valY);
                    break;

                case 0x4:
                    m_chip8Cpu->writeRegister(regX, valX + valY);
                    m_chip8Cpu->writeRegister(0xF, (static_cast<uint16_t>(valX) + static_cast<uint16_t>(valY)) > 255);
                    break;

                case 0x5:
                    m_chip8Cpu->writeRegister(regX, valX - valY);
                    m_chip8Cpu->writeRegister(0xF, valX > valY);
                    break;

                case 0x6:
                    m_chip8Cpu->writeRegister(regX, valX >> 1);
                    m_chip8Cpu->writeRegister(0xF, valX & 1);
                    break;

                case 0x7:
                    m_chip8Cpu->writeRegister(regX, valY - valX);
                    m_chip8Cpu->writeRegister(0xF, valY > valX);
                    break;

                case 0xE:
                    m_chip8Cpu->writeRegister(regX, valX << 1);
                    m_chip8Cpu->writeRegister(0xF, (valX & (1 << 7)) >> 7);
                    break;
                
                default:
                    break;
                }
                }
                break;
            
            case 0x9000:
                if (m_chip8Cpu->readRegister((instruction & 0x0F00) >> 8) != m_chip8Cpu->readRegister((instruction & 0xF0) >> 4))
                    m_chip8Cpu->incrementPC();
                break;
            
            case 0xA000:
                m_chip8Cpu->writeI(instruction & 0xFFF);
                break;

            case 0xB000:
                m_chip8Cpu->writePC((instruction & 0xFFF) + m_chip8Cpu->readRegister(0x0));
                break;

            case 0xC000:
                m_chip8Cpu->writeRegister((instruction & 0x0F00) >> 8, (instruction & 0xFF) & (rand() % 256));
                break;

            case 0xD000:
                {
                emuGL::Point pos
                {
                    m_chip8Cpu->readRegister((instruction & 0x0F00) >> 8),
                    m_chip8Cpu->readRegister((instruction & 0xF0) >> 4)
                };

                uint8_t n = instruction & 0xF;

                std::vector<uint8_t> sprite;
                for (uint16_t i = 0; i < n; i++)
                    sprite.push_back(m_chip8Memory->read(m_chip8Cpu->readI() + i));
                
                m_chip8Cpu->writeRegister(0xF, m_chip8Display->attachSprite(sprite, pos.x, pos.y));
                }
                break;

            case 0xE000:
                {
                uint8_t valX = m_chip8Cpu->readRegister((instruction & 0x0F00) >> 8);

                switch (instruction & 0xFF)
                {
                case 0x9E:
                    if (m_chip8Keypad->isPressed(valX))
                        m_chip8Cpu->incrementPC();
                    break;
                
                case 0xA1:
                    std::cout << "in EA1: " << (int)valX << "\n";
                    if (!m_chip8Keypad->isPressed(valX))
                    {
                        std::cout << "INSIDE IF\n";
                        m_chip8Cpu->incrementPC();
                    }
                    break;

                default:
                    break;
                }
                }
                break;

            case 0xF000:
                {
                    uint8_t reg = (instruction & 0xF00) >> 8;
                    switch (instruction & 0xFF)
                    {
                    case 0x07:
                        m_chip8Cpu->writeRegister(reg, m_chip8Cpu->delayTimer());
                        break;

                    case 0x0A:
                        m_chip8Cpu->writeRegister(reg, m_inputDriver->waitKeyPress(m_chip8Keypad));
                        break;

                    case 0x15:
                        m_chip8Cpu->setDelayTimer(m_chip8Cpu->readRegister(reg));
                        break;
                    
                    case 0x18:
                        m_chip8Cpu->setSoundTimer(m_chip8Cpu->readRegister(reg));
                        break;

                    case 0x1E:
                        m_chip8Cpu->writeI(m_chip8Cpu->readI() + m_chip8Cpu->readRegister(reg));
                        break;

                    case 0x29:
                        m_chip8Cpu->writeI(chip8::Memory::c_fontStartAddr + m_chip8Cpu->readRegister(reg));
                        break;

                    case 0x33:
                        {
                            uint8_t val = m_chip8Cpu->readRegister(reg);
                            m_chip8Memory->write(m_chip8Cpu->readI(), val / 100);
                            m_chip8Memory->write(m_chip8Cpu->readI() + 1, (val % 100) / 10);
                            m_chip8Memory->write(m_chip8Cpu->readI() + 2, val % 10);
                        }
                        break;
                    
                    case 0x55:
                        for (uint8_t i = 0; i <= reg; i++)
                            m_chip8Memory->write(m_chip8Cpu->readI() + i, m_chip8Cpu->readRegister(i));
                        break;

                    case 0x65:
                        for (uint8_t i = 0; i <= reg; i++)
                            m_chip8Cpu->writeRegister(i, m_chip8Memory->read(m_chip8Cpu->readI() + i));
                        break;

                    default:
                        break;
                    }
                }
                break;

            default:
                break;
            }

            if (shallInc)
                m_chip8Cpu->incrementPC();
        }
    };
}

#endif /* EMULATOR_H */
