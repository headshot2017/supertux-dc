//  $Id: configfile.cpp 2620 2005-06-18 12:12:10Z matzebraun $
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2004 Michael George <mike@georgetech.com>
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

#include <stdlib.h>
#include <string>
#include "configfile.h"
#include "setup.h"
#include "globals.h"
#include "lispreader.h"
#include "player.h"
#include "dreamcast.h"

#ifdef WIN32
const char * config_filename = "/st_config.dat";
#else
const char * config_filename = "/STUXCONF";
#endif

static void defaults ()
{
  /* Set defaults: */
  debug_mode = false;
  audio_device = true;

  use_fullscreen = true;
  show_fps = false;
  use_gl = false;

  use_sound = true;
  use_music = true;
}

void loadconfig(void)
{
  FILE * file = NULL;

  defaults();

  /* override defaults from config file */

  file = opendata(config_filename, "r");

  if (file == NULL)
    return;

  // Dreamcast: parse VMU data
  vmu_pkg_t pkg = loadFromVMU(file);
  fclose(file);

  file = fmemopen((char*)pkg.data, (size_t)pkg.data_len, "r");

  /* read config file */

  lisp_stream_t   stream;
  lisp_object_t * root_obj = NULL;

  lisp_stream_init_file (&stream, file);
  root_obj = lisp_read (&stream);

  if (root_obj->type == LISP_TYPE_EOF || root_obj->type == LISP_TYPE_PARSE_ERROR)
    return;

  if (strcmp(lisp_symbol(lisp_car(root_obj)), "supertux-config") != 0)
    return;

  LispReader reader(lisp_cdr(root_obj));

  reader.read_bool("fullscreen", &use_fullscreen);
  reader.read_bool("sound",      &use_sound);
  reader.read_bool("music",      &use_music);
  reader.read_bool("show_fps",   &show_fps);

  std::string video;
  reader.read_string ("video", &video);
  if (video == "opengl")
    use_gl = true;
  else
    use_gl = false;

  reader.read_int ("joystick", &joystick_num);
  if (!(joystick_num >= 0))
    use_joystick = false;
  else
    use_joystick = true;

  reader.read_int ("joystick-x", &joystick_keymap.x_axis);
  reader.read_int ("joystick-y", &joystick_keymap.y_axis);
  reader.read_int ("joystick-a", &joystick_keymap.a_button);
  reader.read_int ("joystick-b", &joystick_keymap.b_button);
  reader.read_int ("joystick-start", &joystick_keymap.start_button);
  reader.read_int ("joystick-deadzone", &joystick_keymap.dead_zone);

  reader.read_int ("keyboard-jump", &keymap.jump);
  reader.read_int ("keyboard-duck", &keymap.duck);
  reader.read_int ("keyboard-left", &keymap.left);
  reader.read_int ("keyboard-right", &keymap.right);
  reader.read_int ("keyboard-fire", &keymap.fire);

  lisp_free(root_obj);
  fclose(file);
}

void saveconfig (void)
{
  /* write settings to config file */

  FILE * config = opendata(config_filename, "w");

  if(config)
    {
      // multi-line? cry about it
      char data[4096];
      sprintf(data, "(supertux-config\n\
\t;; the following options can be set to #t or #f:\n\
\t(fullscreen %s)\n\
\t(sound      %s)\n\
\t(music      %s)\n\
\t(show_fps   %s)\n\
\n\
\t;; either \"opengl\" or \"sdl\"\n\
\t(video      \"%s\")\n\
\n\
\t;; joystick number (-1 means no joystick):\n\
\t(joystick   %d)\n\
\t(joystick-x   %d)\n\
\t(joystick-y   %d)\n\
\t(joystick-a   %d)\n\
\t(joystick-b   %d)\n\
\t(joystick-start  %d)\n\
\t(joystick-deadzone  %d)\n\
\t(keyboard-jump  %d)\n\
\t(keyboard-duck  %d)\n\
\t(keyboard-left  %d)\n\
\t(keyboard-right %d)\n\
\t(keyboard-fire  %d)\n\
)\n", use_fullscreen ? "#t" : "#f", use_sound ? "#t" : "#f", use_music ? "#t" : "#f", show_fps ? "#t" : "#f", use_gl ? "opengl" : "sdl", use_joystick ? joystick_num : -1, joystick_keymap.x_axis, joystick_keymap.y_axis, joystick_keymap.a_button, joystick_keymap.b_button, joystick_keymap.start_button, joystick_keymap.dead_zone, keymap.jump, keymap.duck, keymap.left, keymap.right, keymap.fire);

      saveToVMU(config, data, "Game configuration");

      fclose(config);
    }

}

/* EOF */
