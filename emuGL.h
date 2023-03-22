#ifndef EMUGL_H
#define EMUGL_H

#include <string>
#include <vector>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "color.h"

namespace emuGL
{
    struct Point
    {
        Point() : x{}, y{} {}
        Point(int x, int y) : x(x), y(y) {}

        Point(const Point& other) : x(other.x), y(other.y) {}
        Point operator=(const Point& other) { return {other}; }

        int x;
        int y;
    };

    class Window;

    class Shape
    {
    friend class Window;

    public:
        void add(Point p) { m_points.push_back(p); }
        void setBorderColor(const Color& color) { m_borderColor = color; }
        const Color& borderColor() const { return m_borderColor; }

        void setFillColor(const Color& color) { m_fillColor = color; }
        const Color& fillColor() const { return m_fillColor; }

        Shape(const Shape&) = delete;
        Shape operator=(const Shape&) = delete;

        Point point(int i) const { return m_points.at(i); }

    protected:
        Shape() {};
        Shape(std::initializer_list<Point> points) { for (const Point& p : points) add(p); }

        virtual void drawShape(SDL_Renderer* renderer) const;


    private:
        std::vector<Point> m_points;
        Color m_borderColor {Colors::black};
        Color m_fillColor {Colors::transparent};

        void draw(const Window* win) const;
    };


    class Rect : public Shape
    {
    public:
        Rect(int width, int height, Point pos)
            :   m_w(width),
                m_h(height),
                Shape{pos}
        { m_rect = new SDL_Rect{point(0).x, point(0).y, m_w, m_h}; }

        ~Rect() { delete m_rect; }

    protected:
        void drawShape(SDL_Renderer* renderer) const
        {
            if (borderColor() != Colors::transparent)
            {
                SDL_SetRenderDrawColor(renderer,
                    borderColor().red(), borderColor().green(),
                    borderColor().blue(), borderColor().alpha());
                
                SDL_RenderDrawRect(renderer, m_rect);
            }

            if (fillColor() != Colors::transparent)
            {
                SDL_SetRenderDrawColor(renderer,
                    fillColor().red(), fillColor().green(),
                    fillColor().blue(), fillColor().alpha());
                
                SDL_RenderFillRect(renderer, m_rect);
            }
        }

    private:
        int m_w;
        int m_h;

        SDL_Rect* m_rect {};
    };

    enum class KeyScanCode
    {
        k0, k1, k2, k3, k4, k5, k6, k7, k8, k9, // top 1-9, 0 keys
        kQ, kW, kE, kR, kT, kY, kU, kI, kO, kP,
        kA, kS, kD, kF, kG, kH, kJ, kK, kL,
        kZ, kX, kC, kV, kB, kN, kM,
        kSPACE,
        kENTER
    };

    enum class KeyEvent { Pressed, Released };

    struct KeyInput
    {
        KeyEvent type;
        KeyScanCode keyCode;
    };


    class InputHandler
    {
    public:

        bool quitIntent() const
        {
            SDL_PumpEvents();
            return SDL_HasEvent(SDL_QUIT) == SDL_TRUE;
        }

        /**
         * @returns nullptr if no key input in queue, a ptr to KeyInput type otherwise
        */
        const KeyInput* nextKeyInput(); // TODO: change ptr return to object return

        const KeyInput* waitForKeyPress(); // TODO: change ptr return to object return
        

        ~InputHandler()
        {
            // delete m_sdlEvent;
            delete m_currKeyInput;
        }

    private:
        // SDL_Event m_sdlEvent;
        KeyInput* m_currKeyInput {};

        bool isKeyboardEvent(const SDL_Event* event)
        {
            return SDL_KEYDOWN == event->type || event->type == SDL_KEYUP;
        }

        KeyScanCode scanCodeFromSdlEvent(const SDL_Event& sdlEvent);
        KeyInput* keyInputFromSdlEvent(const SDL_Event& sdlEvent);
        
    };
}

#endif /* EMUGL_H */
