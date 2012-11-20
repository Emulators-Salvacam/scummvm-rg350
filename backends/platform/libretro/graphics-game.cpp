#include <stdio.h>

#include "graphics.h"
#include "os.h"

Graphics::Surface RetroGraphicsManager::_gameScreen;
byte RetroGraphicsManager::_gamePalette[256 * 3];
int RetroGraphicsManager::_screenChangeCount; 		
		
// GAME VIDEO
void RetroGraphicsManager::initSize(uint width, uint height, const Graphics::PixelFormat *format)
{
    _gameScreen.create(width, height, format ? *format : Graphics::PixelFormat::createFormatCLUT8());
}

int16 RetroGraphicsManager::getWidth()
{
    return _gameScreen.w;
}

int16 RetroGraphicsManager::getHeight()
{
    return _gameScreen.h;
}

void RetroGraphicsManager::beginGFXTransaction()
{
    //?
}

OSystem::TransactionError RetroGraphicsManager::endGFXTransaction()
{
    //?
    _screenChangeCount ++;
    return OSystem::kTransactionSuccess;
}

int RetroGraphicsManager::getScreenChangeID() const
{
    _screenChangeCount;
}


void RetroGraphicsManager::setPalette(const byte *colors, uint start, uint num)
{ 
	memcpy(_gamePalette + start * 3, colors, num * 3);
}

void RetroGraphicsManager::grabPalette(byte *colors, uint start, uint num)
{
	assert(colors && _gameScreen.format.bytesPerPixel == 1 && (start + num) < 256);
	memcpy(colors, _gamePalette + start * 3, num * 3);
}

void RetroGraphicsManager::copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h)
{
	assert(x >= 0 && x < _gameScreen.w);
	assert(y >= 0 && y < _gameScreen.h);
	assert(h > 0 && y + h <= _gameScreen.h);
	assert(w > 0 && x + w <= _gameScreen.w);

	// Copy buffer data to game screen internal buffer
	const byte *src = (const byte *)buf;
	byte *dst = (byte *)_gameScreen.pixels + y * _gameScreen.pitch + x * _gameScreen.format.bytesPerPixel;
	
	for (int i = 0; i < h; i++)
	{
		memcpy(dst, src, w * _gameScreen.format.bytesPerPixel);
		src += pitch;
		dst += _gameScreen.pitch;
	}
}

Graphics::Surface *RetroGraphicsManager::lockScreen()
{
    return &_gameScreen;
}

void RetroGraphicsManager::unlockScreen()
{
}

void RetroGraphicsManager::fillScreen(uint32 col)
{
	if(_gameScreen.format.bytesPerPixel == 1)
	{
		memset(_gameScreen.pixels, col, _gameScreen.h * _gameScreen.pitch);
	}
	else if(_gameScreen.format.bytesPerPixel == 2)
	{
		uint16 *pixels = (uint16 *)_gameScreen.pixels;
		uint16 col16 = (uint16)col;
		for (int i = 0; i < _gameScreen.w * _gameScreen.h; i++)
		{
			pixels[i] = col16;
		}
	}
	else if(_gameScreen.format.bytesPerPixel == 3)
	{
		uint8 *pixels = (uint8 *)_gameScreen.pixels;
		byte r = (col >> 16) & 0xFF;
		byte g = (col >> 8) & 0xFF;
		byte b = col & 0xFF;
		for (int i = 0; i < _gameScreen.w * _gameScreen.h; i++)
		{
			pixels[0] = r;
			pixels[1] = g;
			pixels[2] = b;
			pixels += 3;
		}
	}
	else if (_gameScreen.format.bytesPerPixel == 4)
	{
		uint32 *pixels = (uint32 *)_gameScreen.pixels;
		for (int i = 0; i < _gameScreen.w * _gameScreen.h; i++)
		{
			pixels[i] = col;
		}
	}
}

void RetroGraphicsManager::updateScreen()
{
    extern void retro_leave_thread();
    retro_leave_thread();
}
