//  $Id: globals.h 1025 2004-05-07 13:46:50Z rmcruz $
// 
//  SuperTux
//  Copyright (C) 2004 Bill Kendrick <bill@newbreedsoftware.com>
//                     Tobias Glaesser <tobi.web@gmx.de>
//                     Ingo Ruhnke <grumbel@gmx.de>
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef SUPERTUX_GLOBALS_H
#define SUPERTUX_GLOBALS_H

#include <string>
#include <SDL/SDL.h>
#include "text.h"
#include "menu.h"
#include "mousecursor.h"

extern std::string datadir;

struct JoystickKeymap
{
  int a_button;
  int b_button;
  int x_button;
  int y_button;
  int start_button;
  int c_button;
  int d_button;
  int z_button;

  int x_axis;
  int y_axis;
  int r_trigger;
  int l_trigger;
  int x_axis2;
  int y_axis2;
  
  int dead_zone;

  JoystickKeymap();
};

extern JoystickKeymap joystick_keymap;

extern SDL_Surface * screen;
extern Text* black_text;
extern Text* gold_text;
extern Text* silver_text;
extern Text* white_text;
extern Text* white_small_text;
extern Text* white_big_text;
extern Text* blue_text;
extern Text* red_text;
extern Text* green_text;
extern Text* yellow_nums;

extern MouseCursor * mouse_cursor;

extern bool use_gl;
extern bool use_joystick;
extern bool use_fullscreen;
extern bool debug_mode;
extern bool show_fps;

/** The number of the joystick that will be use in the game */
extern int joystick_num;
extern char* level_startup_file;
extern bool launch_leveleditor_mode;

/* SuperTux directory ($HOME/.supertux) and save directory($HOME/.supertux/save) */
extern char* st_dir;
extern char* st_save_dir;

extern float game_speed;
extern SDL_Joystick * js;

int wait_for_event(SDL_Event& event,unsigned int min_delay = 0, unsigned int max_delay = 0, bool empty_events = false);
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
#endif /* SUPERTUX_GLOBALS_H */
