#ifndef BACKENDS_LIBRETRO_PALETTE_H
#define BACKENDS_LIBRETRO_PALETTE_H

struct RetroPalette
{
    byte _colors[256 * 3];
    
    void set(const byte *colors, uint start, uint num)
    {
        assert(colors && (start + num) <= 256);
        memcpy(_colors + start * 3, colors, num * 3);
    }
    
    void get(byte* colors, uint start, uint num)
    {
        assert(colors && (start + num) <= 256);
        memcpy(colors, _colors + start * 3, num * 3);    
    }
    
    void getColor(uint aIndex, byte& aR, byte& aG, byte& aB) const
    {
        if(aIndex < 256)
        {
            aR = _colors[aIndex * 3 + 0];
            aG = _colors[aIndex * 3 + 1];
            aB = _colors[aIndex * 3 + 2];
        }
    }
};

#endif
