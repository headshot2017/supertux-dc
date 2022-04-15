#include "dreamcast.h"
#include "setup.h"

vmu_pkg_t loadFromVMU(FILE* f)
{
	fseek(f, 0, SEEK_END);
	size_t fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* data = new char[fsize+1];
	fread(data, fsize, 1, f);
	
	vmu_pkg_t pkg;
	vmu_pkg_parse((uint8*)data, &pkg);
	delete data;

	return pkg;
}

void saveToVMU(FILE* f, const char* data, const char* longdesc)
{
	vmu_pkg_t pkg;
	uint8 *pkg_out;
	int pkg_size;

	strcpy(pkg.desc_short, "SuperTux");
	strcpy(pkg.desc_long, longdesc);
	strcpy(pkg.app_id, "KOS");
	pkg.icon_cnt = 0;
	pkg.icon_anim_speed = 0;
	pkg.eyecatch_type = VMUPKG_EC_NONE;
	pkg.data_len = strlen(data);
	pkg.data = (uint8*)data;

	vmu_pkg_build(&pkg, &pkg_out, &pkg_size);

	fwrite(pkg_out, 1, pkg_size, f);
}
