WRC=wrc
CC=wcl386
CFLAGS=-Ilmikmod/include/ -bt=nt -l=nt_win -fe=w3mikmod.exe -oeatxh -q
LINKFILE=w3mikmod.lnk

all: w3mikmod.exe

w3mikmod.exe:
	!cd ./lmikmod/win32/ && wmake -f Makefile.wat target=static
	$(WRC) -q -zm -bt=nt -r -fo=w3mikmod.res w3mikmod.rc
	$(CC) $(CFLAGS) @$(LINKFILE) winmm.lib ./lmikmod/win32/*.obj w3mikmod.c w3mikmod.res

libmikmod: .symbolic
	!cd ./lmikmod/win32/ && wmake -f Makefile.wat target=static

clean: .symbolic
	rm -f w3mikmod.exe
	rm -f w3mikmod.obj
	rm -f w3mikmod.o
	rm -f w3mikmod.map
	rm -f w3mikmod.err
	rm -f w3mikmod.res
	!cd ./lmikmod/win32/ && wmake -f Makefile.wat distclean
