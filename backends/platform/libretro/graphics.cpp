#include <stdio.h>

#include "graphics.h"
#include "os.h"

Graphics::Surface RetroGraphicsManager::_gameScreen;
byte RetroGraphicsManager::_gamePalette[256 * 3];



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
    return Graphics::PixelFormat(2, 5, 5, 5, 1, 10, 5, 0, 15);
}

Common::List<Graphics::PixelFormat> RetroGraphicsManager::getSupportedFormats() const
{
    Common::List<Graphics::PixelFormat> result;
    result.push_back(Graphics::PixelFormat(2, 5, 5, 5, 1, 10, 5, 0, 15));
    return result;
}


int RetroGraphicsManager::getDefaultGraphicsMode() const { return 0; }
bool RetroGraphicsManager::setGraphicsMode(int mode) { return true; }
void RetroGraphicsManager::resetGraphicsScale() { }
int RetroGraphicsManager::getGraphicsMode() const	{ return 0;	}
		
// GAME VIDEO
void RetroGraphicsManager::initSize(uint width, uint height, const Graphics::PixelFormat *format)
{
    _gameScreen.create(width, height, Graphics::PixelFormat());
}

int RetroGraphicsManager::getScreenChangeID() const { return 0; }

void RetroGraphicsManager::beginGFXTransaction() { }
OSystem::TransactionError RetroGraphicsManager::endGFXTransaction() { return OSystem::kTransactionSuccess; }

int16 RetroGraphicsManager::getHeight() { return _gameScreen.w; }
int16 RetroGraphicsManager::getWidth() { return _gameScreen.h; }

void RetroGraphicsManager::setPalette(const byte *colors, uint start, uint num)
{ 
	memcpy(_gamePalette + start * 3, colors, num * 3);
}

void RetroGraphicsManager::grabPalette(byte *colors, uint start, uint num) { }

void RetroGraphicsManager::copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h) { }

Graphics::Surface *RetroGraphicsManager::lockScreen() { return &_gameScreen; }

void RetroGraphicsManager::unlockScreen() { }
void RetroGraphicsManager::fillScreen(uint32 col) { }

void RetroGraphicsManager::updateScreen()
{
    extern void retro_leave_thread();
    retro_leave_thread();
}

// ???
void RetroGraphicsManager::setShakePos(int shakeOffset) {}
void RetroGraphicsManager::setFocusRectangle(const Common::Rect& rect) {}
void RetroGraphicsManager::clearFocusRectangle() {}

template<typename INPUT, typename OUTPUT>
void blit(Graphics::Surface& aOut, Graphics::Surface& aIn, int aX, int aY, const byte* aColors, uint32 aKeyColor)
{
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
                    r = aColors[val * 3 + 0];
                    g = aColors[val * 3 + 1];
                    b = aColors[val * 3 + 2];
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
    
    memcpy(screen.pixels, _overlay, sizeof(_overlay));

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
