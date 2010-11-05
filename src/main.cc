/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) Thorsten Wilmer 2010 <thorsten.wilmer@gmail.com>
 * 
 * codeanalyzer is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * codeanalyzer is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtkmm.h>
#include <iostream>

#include <fcntl.h>
#include <string.h>
#ifdef ENABLE_NLS
#  include <libintl.h>
#endif

#include <vector>
#include <iostream>
#include <fstream>

#include <list>

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>


namespace icon
{
#include "icon.h"
}
;

using Glib::ustring;


/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/codeanalyzer/ui/codeanalyzer.ui" */

extern "C"
{
#ifdef GLIBMM_WIN32
#define GLADE_START binary_codeanalyzer_ui_start
#define GLADE_END binary_codeanalyzer_ui_end
#else
#define GLADE_START _binary_codeanalyzer_ui_start
#define GLADE_END _binary_codeanalyzer_ui_end
#endif
   extern char GLADE_START;
   extern char GLADE_END;
}

struct IconData
{
      const guint8 *data;
      unsigned int length;
      int size;
};

static const IconData sIconData[] =
   {
      { icon::icon64, sizeof(icon::icon64), 64 },
      { icon::icon16, sizeof(icon::icon16), 16 },
      { icon::icon32, sizeof(icon::icon32), 32 },

   };

   
int
main (int argc, char *argv[])
{
	Gtk::Main kit(argc, argv);

	int binary_gtk_pinmaster_glade_size = &GLADE_END - &GLADE_START;

	//Load the Glade file and instiate its widgets:
	Glib::RefPtr<Gtk::Builder> builder;
	try
	{
		builder = Gtk::Builder::create_from_string(ustring(&GLADE_START, binary_gtk_pinmaster_glade_size));

	}
	catch (const Glib::FileError & ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	Gtk::Window* main_win = 0;
	builder->get_widget("main_window", main_win);

 Glib::RefPtr<Gtk::IconFactory> factory = Gtk::IconFactory::create();

   std::list<Glib::RefPtr<Gdk::Pixbuf> > iconList;

   for (int i = 0; i < sizeof(sIconData) / sizeof(sIconData[0]); i++)
   {

      const IconData &icon_data = sIconData[i];

      iconList.push_back(Gdk::Pixbuf::create_from_inline(icon_data.length, icon_data.data));

   }

   Gtk::Window::set_default_icon_list(iconList);

	if (main_win)
	{
		kit.run(*main_win);
	}
	return 0;
}
