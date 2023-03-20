#include "chip8.h"

namespace chip8
{
    void checkValSize(uint16_t val, uint8_t b)
    {
        if (b > 16)
            throw std::runtime_error("ERROR: Cannot have a val having more than 16 bits");

        uint16_t maxAddr = (1 << b) - 1;
        if (val > maxAddr)
        {
            throw std::runtime_error("ERROR: Attempted to access invalid memory address on host");
        }
    }


    bool Display::attachSprite(const std::vector<uint8_t>& sprite, uint8_t x, uint8_t y)
    {
        constexpr static uint8_t spriteWidth = 8;
        bool anyErased = false;

        // start pos of sprites wrap around
        x %= m_width;
        y %= m_height;

        for (uint8_t i = 0; i < sprite.size(); i++)
        {
            if (y + i >= m_height) break;
            
            for (uint8_t j = 0; j < spriteWidth; j++)
            {   

                if (x + (spriteWidth - j - 1) >= m_width) continue;

                if (sprite[i] & (1 << j))
                {
                    if (m_screen.at(y + i).at(x + (spriteWidth - j - 1)))
                        anyErased = true;

                    m_screen.at(y + i).at(x + (spriteWidth - j - 1)) =
                        m_screen.at(y + i).at(x + (spriteWidth - j - 1)) ^ sprite[i] & (1 << j);
                }

            }
        }

        return anyErased;
    }
}
