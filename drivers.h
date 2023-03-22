#ifndef DRIVERS_H
#define DRIVERS_H

#include <vector>

#include "emuGL.h"
#include "window.h"
#include "color.h"
#include "theme.h"

namespace drivers
{
    class Display
    {
    public:

        Display(uint16_t scale, uint16_t chip8Width, uint16_t chip8Height)
        {
            // initialising rects
            for (int i = 0; i < chip8Height; i++)
                for (int j = 0; j < chip8Width; j++)
                    m_pixels.push_back(new emuGL::Rect{scale, scale, {j * scale, i * scale}});

            m_window = new emuGL::Window
            {
                "Chip 8",
                chip8Width * scale,
                chip8Height * scale,
                emuGL::Colors::white
            };
            // attaching rects to window
            for (auto& pixel : m_pixels)
                m_window->attach(*pixel);
        }

        ~Display()
        {
            for (auto& pixel : m_pixels)
                delete pixel;

            delete m_window;
        }

        Display() = delete;
        Display(const Display&) = delete;
        Display& operator=(const Display&) = delete;

        void updateDisplay(chip8::Display* chip8Display)
        {
            const std::vector< std::vector<bool> >& screenBuffer = chip8Display->screenBuffer();
            // updating pixels
            for (uint8_t i = 0; i < screenBuffer.size(); i++)
                for (uint8_t j = 0; j < screenBuffer[i].size(); j++)
                {
                    if (screenBuffer[i][j])
                        m_pixels.at(i * screenBuffer[i].size() + j)->setFillColor(theme::foregroundColor);
                    else
                        m_pixels.at(i * screenBuffer[i].size() + j)->setFillColor(theme::backgroundColor);
                }
            
            m_window->update();
        }

    private:
        emuGL::Window* m_window;
        std::vector<emuGL::Rect*> m_pixels;
    };

    class Input
    {
    public:
        ~Input() { delete m_inputHandler; }

        bool shouldQuit() { return m_inputHandler->quitIntent(); }
        uint8_t waitKeyPress(chip8::Keypad* chip8Keypad)
        {
            int chip8Key = 16; // 16 if no (or invalid) key pressed

            while (chip8Key == 16)
            {
                emuGL::KeyInput key = *(m_inputHandler->waitForKeyPress());

                chip8Key = chip8KeyFromKeyScanCode(key.keyCode);

                if (chip8Key != 16 && chip8Keypad->isPressed(chip8Key))
                    chip8Key = 16;
            }

            return static_cast<uint8_t>(chip8Key);
        }

        void updateKeyStates(chip8::Keypad* chip8KeyPad)
        {
            while (true)
            {
                const emuGL::KeyInput* nextKeyInput = m_inputHandler->nextKeyInput();
                if (nextKeyInput == nullptr) return;
                
                int chip8Key = chip8KeyFromKeyScanCode(nextKeyInput->keyCode);
                if (chip8Key != 16)
                {
                    const uint8_t chip8Key8Bit = static_cast<uint8_t>(chip8Key);
                    nextKeyInput->type == emuGL::KeyEvent::Pressed ?
                        chip8KeyPad->pressKey(chip8Key8Bit) :
                        chip8KeyPad->releaseKey(chip8Key8Bit);
                }
            }
            
        }

    private:
        emuGL::InputHandler* m_inputHandler { new emuGL::InputHandler{}};
        /**
         * @returns 16 if non-chip8 key pressed, otherwise chip8 key pressed
        */
        int chip8KeyFromKeyScanCode(const emuGL::KeyScanCode& keyScanCode)
        {
            int chip8Key;
            switch (keyScanCode)
            {
                case emuGL::KeyScanCode::k1: chip8Key = 0x1; break;
                case emuGL::KeyScanCode::k2: chip8Key = 0x2; break;
                case emuGL::KeyScanCode::k3: chip8Key = 0x3; break;
                case emuGL::KeyScanCode::kQ: chip8Key = 0x4; break;
                case emuGL::KeyScanCode::kW: chip8Key = 0x5; break;
                case emuGL::KeyScanCode::kE: chip8Key = 0x6; break;
                case emuGL::KeyScanCode::kA: chip8Key = 0x7; break;
                case emuGL::KeyScanCode::kS: chip8Key = 0x8; break;
                case emuGL::KeyScanCode::kD: chip8Key = 0x9; break;
                case emuGL::KeyScanCode::kZ: chip8Key = 0xA; break;
                case emuGL::KeyScanCode::kX: chip8Key = 0x0; break;
                case emuGL::KeyScanCode::kC: chip8Key = 0xB; break;
                case emuGL::KeyScanCode::k4: chip8Key = 0xC; break;
                case emuGL::KeyScanCode::kR: chip8Key = 0xD; break;
                case emuGL::KeyScanCode::kF: chip8Key = 0xE; break;
                case emuGL::KeyScanCode::kV: chip8Key = 0xF; break;

            default:
                chip8Key = 16;
                break;
            }

            return chip8Key;
        }
    };
}

#endif /* DRIVERS_H */
