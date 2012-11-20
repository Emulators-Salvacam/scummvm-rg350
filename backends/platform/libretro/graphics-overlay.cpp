#include <stdio.h>

#include "graphics.h"
#include "os.h"

bool RetroGraphicsManager::_overlayVisible;    
Graphics::Surface RetroGraphicsManager::_overlay;

// OVERLAY
int16 RetroGraphicsManager::getOverlayWidth()
{
    return 640;
}

int16 RetroGraphicsManager::getOverlayHeight()
{
    return 480;
}

Graphics::PixelFormat RetroGraphicsManager::getOverlayFormat() const
{
    return Graphics::PixelFormat(2, 5, 5, 5, 1, 10, 5, 0, 15);
}

void RetroGraphicsManager::showOverlay()
{
    _overlayVisible = true;
}

void RetroGraphicsManager::hideOverlay()
{
    _overlayVisible = false;
}

void RetroGraphicsManager::clearOverlay()
{
    _overlay.fillRect(Common::Rect(_overlay.w, _overlay.h), 0);
}

void RetroGraphicsManager::grabOverlay(void *buf, int pitch)
{
	const byte *src = (byte *)_overlay.pixels;
	byte *dst = (byte *)buf;

	for (int i = 0; i < 480; i++, dst += pitch, src += 640 * 2)
	{
		memcpy(dst, src, 640 * 2);
	}
}

void RetroGraphicsManager::copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h)
{
	const byte *src = (const byte *)buf;
	byte *dst = (byte *)_overlay.pixels + y * _overlay.pitch + x * _overlay.format.bytesPerPixel;
	
	for (int i = 0; i < h; i++)
	{
		memcpy(dst, src, w * _overlay.format.bytesPerPixel);
		src += pitch;
		dst += _overlay.pitch;
	}
}
