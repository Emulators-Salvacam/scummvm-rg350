/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
 
#define FORBIDDEN_SYMBOL_ALLOW_ALL

#include <unistd.h>
#include <sys/time.h>

#include "os.h"
#include "graphics.h"

bool OSystem_Libretro::needMoveEvent;
bool OSystem_Libretro::needLeftEvent;
int32 OSystem_Libretro::mouseX;
int32 OSystem_Libretro::mouseY;
bool OSystem_Libretro::mouseDown;


OSystem_Libretro::OSystem_Libretro()
{
    _fsFactory = new POSIXFilesystemFactory();
}
	
OSystem_Libretro::~OSystem_Libretro()
{

}

void OSystem_Libretro::initBackend()
{
    _mutexManager = new NullMutexManager();
    _timerManager = new DefaultTimerManager();
    _eventManager = new DefaultEventManager(this);
    _savefileManager = new DefaultSaveFileManager();
    _graphicsManager = new RetroGraphicsManager();
    _mixer = new Audio::MixerImpl(this, 22050);

    ((Audio::MixerImpl *)_mixer)->setReady(true);
    ModularBackend::initBackend();	
}

bool OSystem_Libretro::pollEvent(Common::Event &event)
{
    if(needMoveEvent)
    {
        event.type = Common::EVENT_MOUSEMOVE;
        event.mouse.x = mouseX;
        event.mouse.y = mouseY;
        needMoveEvent = false;
        return true;
    }
    else if(needLeftEvent)
    {
        event.type = mouseDown ? Common::EVENT_LBUTTONDOWN : Common::EVENT_LBUTTONUP;
        event.mouse.x = mouseX;
        event.mouse.y = mouseY;
        needLeftEvent = false;
        return true;
    }

    return false;
}

uint32 OSystem_Libretro::getMillis()
{
    struct timeval t;
    gettimeofday(&t, 0);
    
    return (t.tv_sec * 1000) + (t.tv_usec / 1000);	
}
	
void OSystem_Libretro::delayMillis(uint msecs)
{
    usleep(msecs * 1000);
}
	
void OSystem_Libretro::getTimeAndDate(TimeDate &t) const
{

}

void OSystem_Libretro::logMessage(LogMessageType::Type type, const char *message)
{
    printf("%s\n", message);
}
	
Common::EventSource* OSystem_Libretro::getDefaultEventSource()
{
    return this;
}
	
void OSystem_Libretro::sendMouseData(int32 xMove, int32 yMove, bool down)
{
    if(down != mouseDown)
    {
        mouseDown = down;
        needLeftEvent = true;
    }
    
    if(xMove || yMove)
    {
        mouseX += xMove;
        mouseY += yMove;
        needMoveEvent = true;
    }
}

RetroGraphicsManager* OSystem_Libretro::getRetroGraphics()
{
    return (RetroGraphicsManager*)_graphicsManager;
}
