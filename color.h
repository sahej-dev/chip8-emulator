#ifndef AD2C2C1A_A408_4774_9E92_D54031C5BBE9
#define AD2C2C1A_A408_4774_9E92_D54031C5BBE9

#include <stdint.h>

namespace emuGL
{
    class Color
    {
    public:
        Color() = delete;

        Color(uint8_t r, uint8_t g, uint8_t b)
            :   r(r), g(g), b(b), a(255)
        {}

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
            :   r(r), g(g), b(b), a(a)
        {}

        /**
         * @param hex
         *  uint32_t in form 0xRRGGBBAA
        */
        Color(uint32_t hex)
            :   r((hex & 0xFF000000) >> 24),
                g((hex & 0x00FF0000) >> 16),
                b((hex & 0x0000FF00) >>  8),
                a( hex & 0x000000FF)
        {}


        Color(const Color& other)
            :   Color{other.r, other.g, other.b, other.a}
        {}

        Color& operator=(const Color& other)
        {
            r = other.r;
            g = other.g;
            b = other.b;
            a = other.a;

            return *this;
        }


        uint8_t red()   const { return r; }
        uint8_t green() const { return g; }
        uint8_t blue()  const { return b; }
        uint8_t alpha() const { return a; }


        void setAlpha(uint8_t alpha) { a = alpha; }

        void makeTransparent() { a = 0; }
        void makeOpaque() { a = 255; }
        

    private:
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

    };

    class Colors
    {
    public:
        Colors() = delete;

        inline static const Color transparent {0, 0, 0, 0};

        inline static const Color hardRed       {0xFF0000ff};
        inline static const Color hardGreen     {0x00FF00ff};
        inline static const Color hardBlue      {0x0000FFff};

        inline static const Color white         {0xFFFFFFff};
        inline static const Color black         {0x000000ff};

        inline static const Color red           {0xf44336FF};
        inline static const Color green         {0x33cc66FF};
        inline static const Color blue          {0x1188ffFF};
        inline static const Color limeGreen     {0x36ff39FF};

        inline static const Color pastelRed         {0xFF8B8BFF};
        inline static const Color pastelDarkRed     {0xFF6961FF};
        inline static const Color pastelGreen       {0xAAFFA2FF};
        inline static const Color pastelDarkGreen   {0x77DD78FF};
        inline static const Color pastelBlue        {0x74BDFFFF};
        inline static const Color pastelDarkBlue    {0x50A7FFFF};

        inline static const Color pastelCream       {0xFEFFF3FF};
        inline static const Color pastelYellow      {0xFBFA93FF};
        inline static const Color pastelPurple      {0xE1BFFFFF};
        inline static const Color pastelDarkPurple  {0xFFAEF7FF};

    };

    inline bool operator==(const Color& a, const Color& b)
    {
        return  a.red()   == b.red()   &&
                a.green() == b.green() &&
                a.blue()  == b.blue()  &&
                a.alpha() == b.alpha();
    }

    inline bool operator!=(const Color& a, const Color& b) { return !(a == b); }

}

#endif /* AD2C2C1A_A408_4774_9E92_D54031C5BBE9 */
