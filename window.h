#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <SDL2/SDL.h>

#include "color.h"
#include "emuGL.h"

namespace emuGL
{
    class Window
    {
    friend class Shape;

    public:
        Window(std::string title, int width, int height, Point position, Color bgColor)
            :   m_title(title),
                m_w(width),
                m_h(height),
                m_pos(position),
                m_bgCol(bgColor)
        { initializeSdlObjects(); }

        Window(std::string title, int width, int height, Color bgColor)
            :   Window{title, width, height, {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, bgColor}
        {}

        Window(std::string title, int width, int height, Point position)
            :   Window{title, width, height, position, Colors::white}
        {}

        Window(std::string title, int width, int height)
            :   Window{title, width, height, {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, Colors::white}
        {}

        ~Window()
        {
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);

            SDL_Quit();
        }

        Window() = delete;
        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;

        /**
         * updates a GUI window.
         * does not have a main loop, it should be used inside a main loop.
        */
        void update();

        void attach(const Shape& shape) { m_shapes.push_back(&shape); }

        const Color& bgColor() { return m_bgCol; }
        void setBgColor(const Color& color) { m_bgCol = color; }

        int height() { return m_h; }
        int width() { return m_w; }
        

    private:
        std::string m_title;
        int m_w;
        int m_h;
        Point m_pos;
        Color m_bgCol;

        std::vector<const Shape*> m_shapes;

        SDL_Window* m_window {};
        SDL_Renderer* m_renderer {};

        SDL_Event m_currEvent;

        void initializeSdlObjects();
    };
}

#endif /* WINDOW_H */
