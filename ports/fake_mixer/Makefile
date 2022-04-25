# Fake lib, can be compiled with Mikmod or Modplug support.
# Please notice that Mikmod only supports PCM Mono (no ADPCM).

TARGET = libFAKE_mixer.a

OBJS = FAKE_mixer.o mm.o

LOCAL_CLEAN = *.o *.a

KOS_CFLAGS += -I../SDL-dreamhal/include

include $(KOS_BASE)/addons/Makefile.prefab
