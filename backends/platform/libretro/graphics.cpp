#include <stdio.h>

#include "graphics.h"
#include "os.h"

Graphics::Surface RetroGraphicsManager::_screenData;

bool RetroGraphicsManager::_overlayVisible;    
uint16 RetroGraphicsManager::_overlay[640 * 480];


RetroGraphicsManager::RetroGraphicsManager() { }
RetroGraphicsManager::~RetroGraphicsManager()	{ _screenData.free(); }

// VIDEO FEATURES
bool RetroGraphicsManager::hasFeature(OSystem::Feature f) { return false;	}
void RetroGraphicsManager::setFeatureState(OSystem::Feature f, bool enable) {	}
bool RetroGraphicsManager::getFeatureState(OSystem::Feature f) { return false; }

// VIDEO MODE
const OSystem::GraphicsMode *RetroGraphicsManager::getSupportedGraphicsModes() const
{
    static const OSystem::GraphicsMode s_noGraphicsModes[] = { {0, 0, 0} };
    return s_noGraphicsModes;
}

int RetroGraphicsManager::getDefaultGraphicsMode() const { return 0; }
bool RetroGraphicsManager::setGraphicsMode(int mode) { return true; }
void RetroGraphicsManager::resetGraphicsScale() { }
int RetroGraphicsManager::getGraphicsMode() const	{ return 0;	}
		
// GAME VIDEO
void RetroGraphicsManager::initSize(uint width, uint height, const Graphics::PixelFormat *format)
{
    _screenData.create(width, height, Graphics::PixelFormat());
}

int RetroGraphicsManager::getScreenChangeID() const { return 0; }

void RetroGraphicsManager::beginGFXTransaction() { }
OSystem::TransactionError RetroGraphicsManager::endGFXTransaction() { return OSystem::kTransactionSuccess; }

int16 RetroGraphicsManager::getHeight() { return _screenData.w; }
int16 RetroGraphicsManager::getWidth() { return _screenData.h; }

void RetroGraphicsManager::setPalette(const byte *colors, uint start, uint num) { }
void RetroGraphicsManager::grabPalette(byte *colors, uint start, uint num) { }

void RetroGraphicsManager::copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h) { }

Graphics::Surface *RetroGraphicsManager::lockScreen() { return &_screenData; }

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

// OVERLAY

void RetroGraphicsManager::showOverlay() { _overlayVisible = true; }
void RetroGraphicsManager::hideOverlay() { _overlayVisible = false; }

Graphics::PixelFormat RetroGraphicsManager::getOverlayFormat() const { return Graphics::PixelFormat(2, 5, 5, 5, 1, 10, 5, 0, 15); }
int16 RetroGraphicsManager::getOverlayHeight() { return 480; }
int16 RetroGraphicsManager::getOverlayWidth() { return 640; }

void RetroGraphicsManager::clearOverlay() { memset(_overlay, 0, sizeof(_overlay)); }

void RetroGraphicsManager::grabOverlay(void *buf, int pitch)
{
    
}

void RetroGraphicsManager::copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h)
{
    const byte *src = (const byte *)buf;

    // Clip the coordinates
    if (x < 0)
    {
        w += x;
        src -= x * 2;
        x = 0;
    }

    if (y < 0)
    {
        h += y;
        src -= y * pitch;
        y = 0;
    }

    w = (w > 640 - x) ? 640 - x : w;
    h = (h > 480 - y) ? 640 - y : h;

    if(w > 0 || h > 0)
    {
        byte *dst = (byte *)&_overlay[y * 640];
        for (int i = 0; i < h; i++, src += pitch, dst += 640 * 2)
        {
            memcpy(dst + x * 2, src, w * 2);
        }
    }
}

// MOUSE
bool RetroGraphicsManager::showMouse(bool visible) { return !visible; }
void RetroGraphicsManager::warpMouse(int x, int y) {}
void RetroGraphicsManager::setMouseCursor(const void *buf, uint w, uint h, int hotspotX, int hotspotY, uint32 keycolor, bool dontScale, const Graphics::PixelFormat *format) {}
void RetroGraphicsManager::setCursorPalette(const byte *colors, uint start, uint num) {}

uint16* RetroGraphicsManager::getScreen()
{
    static uint16 screen[640 * 480];
    
    memcpy(screen, _overlay, sizeof(_overlay));

    // Draw Mouse
    const int32 mouseX = OSystem_Libretro::mouseX;
    const int32 mouseY = OSystem_Libretro::mouseY;
    
    if(mouseY > 0 && mouseX > 0 && mouseX < 640 && mouseY < 480)
    {
        screen[mouseY * 640 + mouseX] = 0x7FFF;
    }
    
    return screen;
}
