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
 
#ifndef BACKENDS_LIBRETRO_OS_H
#define BACKENDS_LIBRETRO_OS_H

 
#define FORBIDDEN_SYMBOL_ALLOW_ALL

#include <unistd.h>
#include <sys/time.h>
#include <list>

#include "backends/modular-backend.h"
#include "base/main.h"

#include "backends/saves/default/default-saves.h"
#include "backends/timer/default/default-timer.h"
#include "backends/mutex/null/null-mutex.h"
#include "backends/events/default/default-events.h"
#include "backends/fs/posix/posix-fs-factory.h"

#include "audio/mixer_intern.h"
#include "common/scummsys.h"
#include "common/events.h"
#include "libretro.h"

//

class RetroGraphicsManager;

//

class OSystem_Libretro : public ModularBackend, public Common::EventSource
{
    public:
        static int32 mouseX;
        static int32 mouseY;
        static bool mouseDown;
    
    
        OSystem_Libretro();
        virtual ~OSystem_Libretro();
    
        virtual void initBackend();
    
        virtual Common::EventSource *getDefaultEventSource();
        
        virtual uint32 getMillis();
        virtual void delayMillis(uint msecs);	
        virtual void getTimeAndDate(TimeDate &t) const;
        virtual void logMessage(LogMessageType::Type type, const char *message);
        
        //
        
        RetroGraphicsManager* getRetroGraphics();
        
        // EVENTS
        static std::list<Common::Event> events;
        
        virtual bool pollEvent(Common::Event &event);
        void processMouse(retro_input_state_t aCallback);
        void processKeyboard(retro_input_state_t aCallback);
};

#endif
