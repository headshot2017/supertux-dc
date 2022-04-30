# Fake lib, can be compiled with Mikmod or Modplug support.
# Please notice that Mikmod only supports PCM Mono (no ADPCM).

TARGET = libFAKE_mixer.a

OBJS = FAKE_mixer.o mm.o

LOCAL_CLEAN = *.o *.a

KOS_CFLAGS += -I../SDLDH1.0/include -I../mikmod/lmikmod/include -I$(KOS_BASE)/kernel/arch/dreamcast/include/dc
# i shouldn't need to add that KOS_BASE include...
# but if it's not there, it doesn't pick up sound/sfxmgr.h

include $(KOS_BASE)/addons/Makefile.prefab
