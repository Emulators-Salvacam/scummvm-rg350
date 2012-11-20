#include <stdio.h>

#include "graphics.h"
#include "os.h"

bool RetroGraphicsManager::_mouseVisible;
Graphics::Surface RetroGraphicsManager::_mouseImage;
Graphics::PixelFormat RetroGraphicsManager::_mouseFormat;
int RetroGraphicsManager::_mouseHotspotX;
int RetroGraphicsManager::_mouseHotspotY;
bool RetroGraphicsManager::_mouseDontScale;
uint32 RetroGraphicsManager::_mouseKeyColor;
RetroPalette RetroGraphicsManager::_mousePalette;
bool RetroGraphicsManager::_mousePaletteEnabled;

bool RetroGraphicsManager::showMouse(bool visible)
{
    const bool wasVisible = _mouseVisible;
    _mouseVisible = visible; 
    return wasVisible;
}

void RetroGraphicsManager::warpMouse(int x, int y)
{
    OSystem_Libretro::mouseX = x;
    OSystem_Libretro::mouseY = y;
}

void RetroGraphicsManager::setMouseCursor(const void *buf, uint w, uint h, int hotspotX, int hotspotY, uint32 keycolor, bool dontScale, const Graphics::PixelFormat *format)
{
    _mouseFormat = format ? (*format) : Graphics::PixelFormat::createFormatCLUT8();

    if(_mouseImage.w != w || _mouseImage.h != h || _mouseImage.format.bytesPerPixel != _mouseFormat.bytesPerPixel)
    {    
        _mouseImage.create(w, h, _mouseFormat);
    }

	memcpy(_mouseImage.pixels, buf, h * _mouseImage.pitch);

    _mouseHotspotX = hotspotX;
    _mouseHotspotY = hotspotY;
    _mouseKeyColor = keycolor;
    _mouseDontScale = dontScale;
}

void RetroGraphicsManager::setCursorPalette(const byte *colors, uint start, uint num)
{
    _mousePalette.set(colors, start, num);
	_mousePaletteEnabled = true;
}
