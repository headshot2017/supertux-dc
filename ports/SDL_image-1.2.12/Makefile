TARGET=libSDL_image.a

KOS_CFLAGS+= -DLOAD_JPG=YES -DLOAD_BMP=YES -DLOAD_GIF=YES -DLOAD_LBM=YES -DLOAD_PCX=YES -DLOAD_PNM=YES -DLOAD_TGA=YES -DLOAD_XCF=YES -DLOAD_XXX=YES -DLOAD_PNG=YES -I../SDLDH1.0/include -I$(KOS_PORTS)/include/png -I$(KOS_PORTS)/include/jpeg

all: $(TARGET)


include $(KOS_BASE)/Makefile.rules

OBJS = IMG.o IMG_bmp.o IMG_gif.o IMG_jpg.o IMG_lbm.o IMG_pcx.o IMG_png.o IMG_pnm.o IMG_tga.o IMG_tif.o IMG_webp.o IMG_xcf.o IMG_xpm.o IMG_xv.o IMG_xxx.o

$(TARGET): $(OBJS)
	rm -f $(KOS_BASE)/addons/lib/dreamcast/$(TARGET)
	$(KOS_AR) rcs $(KOS_BASE)/addons/lib/dreamcast/$(TARGET) $(OBJS)
	$(KOS_RANLIB) $(KOS_BASE)/addons/lib/dreamcast/$(TARGET)
	cp SDL_image.h $(KOS_BASE)/addons/include

clean:
	rm -f $(OBJS)
