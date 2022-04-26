#ifdef __DREAMCAST__

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


uint32 lastPressed[4] = {0, 0, 0, 0};
uint32 btns[] = {
	CONT_A,
	CONT_B,
	CONT_X,
	CONT_Y,
	CONT_START,
	CONT_DPAD_UP,
	CONT_DPAD_DOWN,
	CONT_DPAD_LEFT,
	CONT_DPAD_RIGHT,
};
uint32 btnSize = 9;

// check if a button has been pressed (NOT when held down, useful for menus or something)
uint32 getPressed(int port)
{
	maple_device_t *cont = maple_enum_type(port, MAPLE_FUNC_CONTROLLER);
	cont_state_t *state;
	uint32 pressed = lastPressed[port];
	bool update = false;

	if (cont)
	{
		state = (cont_state_t *)maple_dev_status(cont);

		if (state)
		{
			for (uint32 i=0; i<btnSize; i++)
			{
				if (state->buttons & btns[i] && !(lastPressed[port] & btns[i]))
				{
					pressed |= btns[i]; // this button has been pressed, add it
					update = true;
				}
				else if (state->buttons & btns[i] && lastPressed[port] & btns[i])
					pressed &= ~btns[i]; // this button was already pressed, ignore it
				else if (!(state->buttons & btns[i]) && lastPressed[port] & btns[i])
				{
					pressed &= ~btns[i]; // button has been released
					update = true;
				}
			}

			if (update)
				lastPressed[port] = pressed;
		}
	}

	return pressed;
}

#endif // __DREAMCAST__

