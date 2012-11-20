#ifndef BACKENDS_LIBRETRO_GRAPHICS_H
#define BACKENDS_LIBRETRO_GRAPHICS_H

#include <stdio.h>

#include "os.h"

#include "graphics/surface.h"
#include "backends/graphics/graphics.h"

class RetroGraphicsManager : public GraphicsManager {
    static Graphics::Surface _screenData;
    
    static bool _overlayVisible;    
    static uint16 _overlay[640 * 480];


public:
    RetroGraphicsManager();
	virtual ~RetroGraphicsManager();

    // VIDEO FEATURES
	bool hasFeature(OSystem::Feature f);
	void setFeatureState(OSystem::Feature f, bool enable);
	bool getFeatureState(OSystem::Feature f);

    // VIDEO MODE
	const OSystem::GraphicsMode *getSupportedGraphicsModes() const;
	
	int getDefaultGraphicsMode() const;
	bool setGraphicsMode(int mode);
	void resetGraphicsScale();
	int getGraphicsMode() const;
		
	// GAME VIDEO
	void initSize(uint width, uint height, const Graphics::PixelFormat *format = NULL);	
	virtual int getScreenChangeID() const;

	void beginGFXTransaction();
	OSystem::TransactionError endGFXTransaction();

	int16 getHeight();
	int16 getWidth();

	void setPalette(const byte *colors, uint start, uint num);
	void grabPalette(byte *colors, uint start, uint num);
	
	void copyRectToScreen(const void *buf, int pitch, int x, int y, int w, int h);

	Graphics::Surface *lockScreen();
	
	void unlockScreen();
	void fillScreen(uint32 col);
	
	void updateScreen();
	
    // ???
	void setShakePos(int shakeOffset);
	void setFocusRectangle(const Common::Rect& rect);
	void clearFocusRectangle();

    // OVERLAY
	void showOverlay();
	void hideOverlay();
	void clearOverlay();
	void grabOverlay(void *buf, int pitch);	
	void copyRectToOverlay(const void *buf, int pitch, int x, int y, int w, int h);

	Graphics::PixelFormat getOverlayFormat() const;
	int16 getOverlayHeight();
	int16 getOverlayWidth();



    // MOUSE
	bool showMouse(bool visible);
	void warpMouse(int x, int y);
	void setMouseCursor(const void *buf, uint w, uint h, int hotspotX, int hotspotY, uint32 keycolor, bool dontScale = false, const Graphics::PixelFormat *format = NULL);
	void setCursorPalette(const byte *colors, uint start, uint num);
	
	uint16* getScreen();
};

#endif
