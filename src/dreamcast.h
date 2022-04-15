#ifndef SUPERTUX_DREAMCAST_H
#define SUPERTUX_DREAMCAST_H

#include <stdio.h>
#include <kos.h>

vmu_pkg_t loadFromVMU(FILE* f);
void saveToVMU(FILE* f, const char* data, const char* longdesc);

#endif

