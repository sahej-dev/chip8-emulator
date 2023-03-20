#include "window.h"

#include <stdexcept>

#include <iostream>

namespace emuGL
{
 
    void Window::update()
    {
        SDL_SetRenderDrawColor(m_renderer,
                m_bgCol.red(), m_bgCol.green(), m_bgCol.blue(), m_bgCol.alpha());
        SDL_RenderClear(m_renderer);

        for (int i = 0; i < m_shapes.size(); i++)
            m_shapes[i]->draw(this);

        SDL_RenderPresent(m_renderer);
    }

    void Window::initializeSdlObjects()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            throw std::runtime_error(SDL_GetError());
        
        m_window = SDL_CreateWindow(
            m_title.c_str(),
            m_pos.x, m_pos.y,
            m_w, m_h,
            SDL_WINDOW_MAXIMIZED);
        if (!m_window) throw std::runtime_error(SDL_GetError());

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        if (!m_renderer) throw std::runtime_error(SDL_GetError());
    }
}