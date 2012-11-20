#include <stdio.h>

#include "graphics.h"
#include "os.h"

bool RetroGraphicsManager::_overlayVisible;    
uint16 RetroGraphicsManager::_overlay[640 * 480];

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
    memset(_overlay, 0, sizeof(_overlay));
}

void RetroGraphicsManager::grabOverlay(void *buf, int pitch)
{
	const byte *src = (byte *)_overlay;
	byte *dst = (byte *)buf;

	for (int i = 0; i < 480; i++, dst += pitch, src += 640 * 2)
	{
		memcpy(dst, src, 640 * 2);
	}
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
