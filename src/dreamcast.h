#ifndef SUPERTUX_DREAMCAST_H
#define SUPERTUX_DREAMCAST_H

#ifdef __DREAMCAST__

#include <stdio.h>
#include <kos.h>

vmu_pkg_t loadFromVMU(FILE* f);
void saveToVMU(FILE* f, const char* data, const char* longdesc);

#endif // __DREAMCAST__
#endif // SUPERTUX_DREAMCAST_H

