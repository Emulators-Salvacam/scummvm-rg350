#include <stdio.h>

#include "graphics.h"
#include "os.h"


RetroGraphicsManager::RetroGraphicsManager() { }
RetroGraphicsManager::~RetroGraphicsManager()
{
    _gameScreen.free();
    _mouseImage.free();
}

// VIDEO FEATURES
bool RetroGraphicsManager::hasFeature(OSystem::Feature f)
{
	return (f == OSystem::kFeatureCursorPalette);
}

void RetroGraphicsManager::setFeatureState(OSystem::Feature f, bool enable)
{
    switch(f)
    {
        case OSystem::kFeatureCursorPalette: _mousePaletteEnabled = enable; return;
    }
}

bool RetroGraphicsManager::getFeatureState(OSystem::Feature f)
{
    switch(f)
    {
        case OSystem::kFeatureCursorPalette: return _mousePaletteEnabled;
    }

    return false;
}

// VIDEO MODE
const OSystem::GraphicsMode *RetroGraphicsManager::getSupportedGraphicsModes() const
{
    static const OSystem::GraphicsMode s_noGraphicsModes[] = { {0, 0, 0} };
    return s_noGraphicsModes;
}

Graphics::PixelFormat RetroGraphicsManager::getScreenFormat() const
{
    return _gameScreen.format;
}

Common::List<Graphics::PixelFormat> RetroGraphicsManager::getSupportedFormats() const
{
    Common::List<Graphics::PixelFormat> result;
    result.push_back(Graphics::PixelFormat(2, 5, 5, 5, 1, 10, 5, 0, 15));
    result.push_back(Graphics::PixelFormat::createFormatCLUT8());
    return result;
}


int RetroGraphicsManager::getDefaultGraphicsMode() const { return 0; }
bool RetroGraphicsManager::setGraphicsMode(int mode) { return true; }
void RetroGraphicsManager::resetGraphicsScale() { }
int RetroGraphicsManager::getGraphicsMode() const	{ return 0;	}
		

// ???
void RetroGraphicsManager::setShakePos(int shakeOffset) {}
void RetroGraphicsManager::setFocusRectangle(const Common::Rect& rect) {}
void RetroGraphicsManager::clearFocusRectangle() {}

template<typename INPUT, typename OUTPUT>
void blit(Graphics::Surface& aOut, Graphics::Surface& aIn, int aX, int aY, const RetroPalette& aColors, uint32 aKeyColor)
{
    assert(sizeof(OUTPUT) == aOut.format.bytesPerPixel && sizeof(INPUT) == aIn.format.bytesPerPixel);

    for(int i = 0; i != aIn.h; i ++)
    {
        if((i + aY) < 0 || (i + aY) >= 480)
        {
            continue;
        }
    
        INPUT* const in = (INPUT*)aIn.pixels + (i * aIn.w);
        OUTPUT* const out = (OUTPUT*)aOut.pixels + ((i + aY) * aOut.w);
        
        for(int j = 0; j != aIn.w; j ++)
        {
            if((j + aX) < 0 || (j + aX) >= 640)
            {
                continue;
            }
        
            uint8 r, g, b;

            const INPUT val = in[j];
            if(val != aKeyColor)
            {            
                if(aIn.format.bytesPerPixel == 1)
                {
                    aColors.getColor(val, r, g, b);
                }
                else
                {
                    aIn.format.colorToRGB(in[j], r, g, b);
                }
                
                out[j + aX] = aOut.format.RGBToColor(r, g, b);
            }
        }
    }
}


uint16* RetroGraphicsManager::getScreen()
{
    static Graphics::Surface screen;
    screen.create(640, 480, Graphics::PixelFormat(2, 5, 5, 5, 1, 10, 5, 0, 15));
    
    if(_overlayVisible)
    {
        memcpy(screen.pixels, _overlay, sizeof(_overlay));
    }
    else
    {
        if(_gameScreen.w && _gameScreen.h)
        {
            switch(_gameScreen.format.bytesPerPixel)
            {
                case 1: blit<uint8, uint16>(screen, _gameScreen, 0, 0, _gamePalette, 0xFFFFFFFF); break;
                case 2: blit<uint16, uint16>(screen, _gameScreen, 0, 0, _gamePalette, 0xFFFFFFFF); break;
                case 3: blit<uint8, uint16>(screen, _gameScreen, 0, 0, _gamePalette, 0xFFFFFFFF); break;
                case 4: blit<uint32, uint16>(screen, _gameScreen, 0, 0, _gamePalette, 0xFFFFFFFF); break;
            }
        }
    }

    // Draw Mouse
    if(_mouseVisible)
    {
        const int32 mouseX = OSystem_Libretro::mouseX;
        const int32 mouseY = OSystem_Libretro::mouseY;
        
        const int x = mouseX - _mouseHotspotX;
        const int y = mouseY - _mouseHotspotY;

        blit<uint8, uint16>(screen, _mouseImage, x, y, _mousePaletteEnabled ? _mousePalette : _gamePalette, _mouseKeyColor);
    }
    
    return (uint16*)screen.pixels;
}
