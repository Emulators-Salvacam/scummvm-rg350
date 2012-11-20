srcdir = ../../../..
retrodir = backends/platform/libretro/
VPATH = $(srcdir)

#DEFINES = -D__DC__ -DNONSTANDARD_PORT -DUSE_MAD -DUSE_ZLIB -DDISABLE_DEFAULT_SAVEFILEMANAGER -DDISABLE_TEXT_CONSOLE -DDISABLE_COMMAND_LINE -DUSE_RGB_COLOR

CC=clang -g -O3
CXX=clang++ -g -O3
LD=clang++ -g -dynamiclib -O3
DEFINES=-D__LIBRETRO__ -DNONSTANDARD_PORT -DUSE_RGB_COLOR
INCLUDES= -I$(srcdir)/$(retrodir) -I. -I$(srcdir) -I$(srcdir)/engines
EXECUTABLE = libretro
DEPDIR = .deps
MKDIR = mkdir -p
RM = rm -f
RM_REC = rm -rf
AR = ar cru
RANLIB = ranlib
HAVE_GCC3 = true
USE_RGB_COLOR = true

ENABLED=STATIC_PLUGIN
ENABLE_SCUMM = $(ENABLED)
#ENABLE_SCUMM_7_8 = $(ENABLED)
#ENABLE_HE = $(ENABLED)
ENABLE_AGI = $(ENABLED)
ENABLE_AGOS = $(ENABLED)
#ENABLE_CINE = $(ENABLED)
#ENABLE_CRUISE = $(ENABLED)
#ENABLE_DRASCULA = $(ENABLED)
#ENABLE_GOB = $(ENABLED)
#ENABLE_GROOVIE = $(ENABLED)
#ENABLE_IHNM = $(ENABLED)
#ENABLE_KYRA = $(ENABLED)
#ENABLE_LURE = $(ENABLED)
#ENABLE_M4 = $(ENABLED)
#ENABLE_MADE = $(ENABLED)
#ENABLE_PARALLACTION = $(ENABLED)
#ENABLE_QUEEN = $(ENABLED)
#ENABLE_SAGA = $(ENABLED)
ENABLE_SKY = $(ENABLED)
#ENABLE_SWORD1 = $(ENABLED)
#ENABLE_SWORD2 = $(ENABLED)
#ENABLE_TOUCHE = $(ENABLED)
#ENABLE_TUCKER = $(ENABLED)

OBJS :=	$(retrodir)libretro.o \
        $(retrodir)os.o \
        $(retrodir)graphics.o \
        $(retrodir)graphics-mouse.o \
        $(retrodir)graphics-overlay.o \
        $(retrodir)graphics-game.o \
        $(retrodir)libco/libco.o \
        backends/fs/posix/posix-fs.o \
        backends/fs/posix/posix-fs-factory.o

MODULE_DIRS += $(retrodir) $(retrodir)libco

BACKEND := libretro

include $(srcdir)/Makefile.common
	