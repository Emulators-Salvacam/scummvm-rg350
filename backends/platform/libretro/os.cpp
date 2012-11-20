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

RetroGraphicsManager* OSystem_Libretro::getRetroGraphics()
{
    return (RetroGraphicsManager*)_graphicsManager;
}

// EVENTS
std::list<Common::Event> OSystem_Libretro::events;

bool OSystem_Libretro::pollEvent(Common::Event &event)
{
    if(!events.empty())
    {
        event = events.front();
        events.pop_front();
        return true;
    }

    return false;
}

void OSystem_Libretro::processMouse(retro_input_state_t aCallback)
{
    const int16_t x = aCallback(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
    const int16_t y = aCallback(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
    
    if(x || y)
    {
        mouseX += x;
        mouseX = (mouseX < 0) ? 0 : mouseX;
        mouseX = (mouseX >= getRetroGraphics()->getCurrentWidth()) ? getRetroGraphics()->getCurrentWidth() : mouseX;

        mouseY += y;
        mouseY = (mouseY < 0) ? 0 : mouseY;
        mouseY = (mouseY >= getRetroGraphics()->getCurrentHeight()) ? getRetroGraphics()->getCurrentHeight() : mouseY;
        
        Common::Event ev;
        ev.type = Common::EVENT_MOUSEMOVE;
        ev.mouse.x = mouseX;
        ev.mouse.y = mouseY;
        events.push_back(ev); 
    }

    const bool down = aCallback(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
    if(down != mouseDown)
    {
        mouseDown = down;
        
        Common::Event ev;
        ev.type = down ? Common::EVENT_LBUTTONDOWN : Common::EVENT_LBUTTONUP;
        ev.mouse.x = mouseX;
        ev.mouse.y = mouseY;
        events.push_back(ev);
    }
}

void OSystem_Libretro::processKeyboard(retro_input_state_t aCallback)
{
    static bool states[RETROK_LAST];
    
    for(int i = 0; i != RETROK_LAST; i ++)
    {
        const bool down = aCallback(0, RETRO_DEVICE_KEYBOARD, 0, i);
        
        if(states[i] != down)
        {
            states[i] = down;
            
            Common::Event ev;
            ev.type = down ? Common::EVENT_KEYDOWN : Common::EVENT_KEYUP;
            ev.kbd.keycode = (Common::KeyCode)i;
            ev.kbd.flags = 0;
            ev.kbd.ascii = 0;
            events.push_back(ev);
        }
    }
}
