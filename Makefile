#
# Basic KallistiOS skeleton / test program
# Copyright (C)2001-2004 Dan Potter
#   

# Put the filename of the output binary here
TARGET = supertux.elf

CXXFLAGS += -O3 -D__DREAMCAST__ -DNOOPENGL -Iports/SDL-dreamhal/include

# List all of your C files here, but change the extension to ".o"
# Include "romdisk.o" if you want a rom disk.
OBJS = src/badguy.o src/bitmask.o src/button.o src/collision.o src/configfile.o src/dreamcast.o src/gameloop.o src/gameobjs.o src/globals.o src/high_scores.o src/intro.o src/level.o src/leveleditor.o src/lispreader.o src/menu.o src/mousecursor.o src/music_manager.o src/musicref.o src/particlesystem.o src/physic.o src/player.o src/resources.o src/scene.o src/screen.o src/setup.o src/sound.o src/special.o src/sprite.o src/sprite_manager.o src/supertux.o src/text.o src/texture.o src/tile.o src/timer.o src/title.o src/type.o src/world.o src/worldmap.o

# If you define this, the Makefile.rules will create a romdisk.o for you
# from the named dir.
#KOS_ROMDISK_DIR = romdisk

# The rm-elf step is to remove the target before building, to force the
# re-creation of the rom disk.
all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	-rm -f $(TARGET) $(OBJS) romdisk.*

rm-elf:
	-rm -f $(TARGET) romdisk.*

$(TARGET): $(OBJS)
	kos-c++ -o $(TARGET) $(OBJS) -g -L"ports/SDL-dreamhal/Precompiled libs" -lSDL_mixer_126 -lSDL_image -lSDLDH -lmodplug -lvorbis -logg -ljpeg -lpng -lz -lm

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist:
#	rm -f $(OBJS) romdisk.o romdisk.img
	$(KOS_STRIP) $(TARGET)
