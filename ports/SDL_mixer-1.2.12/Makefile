TARGET=libSDL_mixer.a

KOS_CFLAGS+= -DWAV_MUSIC -DMODPLUG_MUSIC -DOGG_MUSIC -I$(KOS_PORTS)/include/SDL -I$(KOS_PORTS)/include/vorbis -I$(KOS_PORTS)/include/ogg -I$(KOS_PORTS)/include/modplug

all: $(TARGET)


include $(KOS_BASE)/Makefile.rules

OBJS = dynamic_flac.o dynamic_fluidsynth.o dynamic_mod.o dynamic_mp3.o dynamic_ogg.o effect_position.o effect_stereoreverse.o effects_internal.o fluidsynth.o load_aiff.o load_flac.o load_ogg.o load_voc.o mixer.o music.o music_cmd.o music_flac.o music_mad.o music_mod.o music_modplug.o music_ogg.o wavestream.o

$(TARGET): $(OBJS)
	rm -f $(KOS_BASE)/addons/lib/dreamcast/$(TARGET)
	$(KOS_AR) rcs $(KOS_BASE)/addons/lib/dreamcast/$(TARGET) $(OBJS)
	$(KOS_RANLIB) $(KOS_BASE)/addons/lib/dreamcast/$(TARGET)
	cp SDL_mixer.h $(KOS_BASE)/addons/include

clean:
	rm -f $(OBJS)