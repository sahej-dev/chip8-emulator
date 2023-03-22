#include <stdexcept>
#include <iostream>

#include <SDL2/SDL.h>

#include "emuGL.h"
#include "window.h"

namespace emuGL
{

    void Shape::drawShape(SDL_Renderer* renderer) const
    {
        if (borderColor() != Colors::transparent)
        {
            for (uint i = 1; i < m_points.size(); i++)
                SDL_RenderDrawLine(renderer, point(i - 1).x, point(i - 1).y, point(i).x, point(i).y);
        }

    }

    void Shape::draw(const Window* win) const
    {
        // storing previous color
        uint8_t r, g, b, a;
        SDL_GetRenderDrawColor(win->m_renderer, &r, &g, &b, &a);
        
        drawShape(win->m_renderer);

        // restoring previoius color
        SDL_SetRenderDrawColor(win->m_renderer, r, g, b, a);
    }

    const KeyInput* InputHandler::nextKeyInput()
    {
        SDL_Event sdlEvent;
        bool haveAnEvent {};
        do
        {
            haveAnEvent = SDL_PollEvent(&sdlEvent);
        }
        while (haveAnEvent && !isKeyboardEvent(&sdlEvent));
        
        if (m_currKeyInput) delete m_currKeyInput;

        if (haveAnEvent)
            m_currKeyInput = keyInputFromSdlEvent(sdlEvent);
        else
            m_currKeyInput = nullptr;

        return m_currKeyInput;
    }

    const KeyInput* InputHandler::waitForKeyPress()
    {
        bool foundKeyDownEvent = false;
        SDL_Event pressedEvent;
        SDL_Event upEvent;
        // while (!foundKeyDownEvent)
        // {
        //     if (!SDL_WaitEvent(&m_sdlEvent))
        //         throw std::runtime_error(SDL_GetError());

        //     // if (m_sdlEvent != nullptr)
        //     // {
        //         // std::cout << "not null\n";
        //         foundKeyDownEvent = isKeyboardEvent(&m_sdlEvent) && m_sdlEvent.key.type == SDL_KEYDOWN;
        //         std::cout << foundKeyDownEvent << " is found key down event\n";
        //     // }
        // }

        while (
            !SDL_PollEvent(&pressedEvent) || 
            !(isKeyboardEvent(&pressedEvent) && pressedEvent.key.type == SDL_KEYDOWN)
        );

        KeyInput* pressedKeyInput = keyInputFromSdlEvent(pressedEvent);

        while (
            !SDL_PollEvent(&upEvent) || 
            !(
                isKeyboardEvent(&upEvent) && upEvent.key.type == SDL_KEYUP &&
                keyInputFromSdlEvent(upEvent)->keyCode == pressedKeyInput->keyCode
            )
        );
        

        if (m_currKeyInput) delete m_currKeyInput;

        m_currKeyInput = keyInputFromSdlEvent(upEvent);
        return m_currKeyInput;
    }

    KeyScanCode InputHandler::scanCodeFromSdlEvent(const SDL_Event& sdlEvent)
    {
        // if (!m_sdlEvent) throw std::runtime_error("ERROR: Requested scan code from null event!");
        if (!isKeyboardEvent(&sdlEvent)) throw std::runtime_error("ERROR: Requested scan code for non keyboard event!");

        switch (sdlEvent.key.keysym.scancode)
        {
            case SDL_SCANCODE_1: return KeyScanCode::k1;
            case SDL_SCANCODE_2: return KeyScanCode::k2;
            case SDL_SCANCODE_3: return KeyScanCode::k3;
            case SDL_SCANCODE_4: return KeyScanCode::k4;
            case SDL_SCANCODE_5: return KeyScanCode::k5;
            case SDL_SCANCODE_6: return KeyScanCode::k6;
            case SDL_SCANCODE_7: return KeyScanCode::k7;
            case SDL_SCANCODE_8: return KeyScanCode::k8;
            case SDL_SCANCODE_9: return KeyScanCode::k9;
            case SDL_SCANCODE_0: return KeyScanCode::k0;
            case SDL_SCANCODE_Q: return KeyScanCode::kQ;
            case SDL_SCANCODE_W: return KeyScanCode::kW;
            case SDL_SCANCODE_E: return KeyScanCode::kE;
            case SDL_SCANCODE_R: return KeyScanCode::kR;
            case SDL_SCANCODE_T: return KeyScanCode::kT;
            case SDL_SCANCODE_Y: return KeyScanCode::kY;
            case SDL_SCANCODE_U: return KeyScanCode::kU;
            case SDL_SCANCODE_I: return KeyScanCode::kI;
            case SDL_SCANCODE_O: return KeyScanCode::kO;
            case SDL_SCANCODE_P: return KeyScanCode::kP;
            case SDL_SCANCODE_A: return KeyScanCode::kA;
            case SDL_SCANCODE_S: return KeyScanCode::kS;
            case SDL_SCANCODE_D: return KeyScanCode::kD;
            case SDL_SCANCODE_F: return KeyScanCode::kF;
            case SDL_SCANCODE_G: return KeyScanCode::kG;
            case SDL_SCANCODE_H: return KeyScanCode::kH;
            case SDL_SCANCODE_J: return KeyScanCode::kJ;
            case SDL_SCANCODE_K: return KeyScanCode::kK;
            case SDL_SCANCODE_L: return KeyScanCode::kL;
            case SDL_SCANCODE_Z: return KeyScanCode::kZ;
            case SDL_SCANCODE_X: return KeyScanCode::kX;
            case SDL_SCANCODE_C: return KeyScanCode::kC;
            case SDL_SCANCODE_V: return KeyScanCode::kV;
            case SDL_SCANCODE_B: return KeyScanCode::kB;
            case SDL_SCANCODE_N: return KeyScanCode::kN;
            case SDL_SCANCODE_M: return KeyScanCode::kM;
            case SDL_SCANCODE_SPACE: return KeyScanCode::kSPACE;
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_KP_ENTER: return KeyScanCode::kENTER;

        default:
            std::cout << sdlEvent.key.keysym.sym << std::endl;
            throw std::runtime_error("ERROR: Unimplemented key event occurred.");
            break;
        }
    }

    KeyInput* InputHandler::keyInputFromSdlEvent(const SDL_Event& sdlEvent)
    {
        return new KeyInput{
            sdlEvent.key.type == SDL_KEYDOWN ? KeyEvent::Pressed : KeyEvent::Released,
            scanCodeFromSdlEvent(sdlEvent)
        }; 
    }
}
