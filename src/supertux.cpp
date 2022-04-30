//  $Id: supertux.cpp 997 2004-05-05 16:07:20Z rmcruz $
// 
//  SuperTux
//  Copyright (C) 2004 Tobias Glaesser <tobi.web@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#define SDL_MAIN_HANDLED
#include <SDL.h>
#undef main

#ifdef __DREAMCAST__
#include <kos.h>
#include <SDL_dreamcast.h>
#include <mp3/sndserver.h>
#endif

#include <sys/types.h>
#include <ctype.h>

#include "defines.h"
#include "globals.h"
#include "setup.h"
#include "intro.h"
#include "title.h"
#include "gameloop.h"
#include "leveleditor.h"
#include "screen.h"
#include "worldmap.h"
#include "resources.h"
#include "texture.h"
#include "tile.h"

int main(int argc, char ** argv)
{
  st_directory_setup();
  parseargs(argc, argv);

#ifdef __DREAMCAST__
  // reset to ODE (GDemu) when these buttons are pressed
  cont_btn_callback(0, CONT_START | CONT_A | CONT_B | CONT_X | CONT_Y, (cont_btn_callback_t)arch_reboot);

  SDL_DC_SetVideoDriver(SDL_DC_DMA_VIDEO);
  //SDL_DC_EmulateKeyboard(SDL_FALSE);
  SDL_DC_EmulateMouse(SDL_FALSE);
#endif

  st_video_setup();
  st_general_setup();

  SDL_ShowCursor(false);
  fadeout();

  st_audio_setup();
  st_joystick_setup();
  st_menu();
  loadshared();

#ifdef __DREAMCAST__
  maple_init();
  snd_stream_init();
  mp3_init();
#endif

  title();
  
  clearscreen(0, 0, 0);
  updatescreen();

  unloadshared();
  st_general_free();
  TileManager::destroy_instance();
#ifdef DEBUG
  Surface::debug_check();
#endif
  st_shutdown();
  
  return 0;
}
