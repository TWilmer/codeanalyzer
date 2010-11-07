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

#include "FileParser.hpp"

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

FileParser *theParser;
Gtk::Window* main_win = 0;

Gtk::TreeView* theSymbolsView=0;
void doOpen()
{
   Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*main_win);

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    Gtk::FileFilter filter_text;
    filter_text.set_name("Application File");
    filter_text.add_mime_type("application/x-executable");
    dialog.add_filter(filter_text);

    filter_text.set_name("Object File");
    filter_text.add_mime_type("application/x-object");
    dialog.add_filter(filter_text);


    Gtk::FileFilter filter_any;
    filter_any.set_name("Any files");
    filter_any.add_pattern("*");
    dialog.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
      case(Gtk::RESPONSE_OK):
      {
        std::cout << "Open clicked." << std::endl;

        //Notice that this is a std::string, not a Glib::ustring.
        std::string filename = dialog.get_filename();
        std::cout << "File selected: " <<  filename << std::endl;

        theParser->launch(filename);
        break;
      }
      case(Gtk::RESPONSE_CANCEL):
      {
        std::cout << "Cancel clicked." << std::endl;
        break;
      }
      default:
      {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
      }
    }

printf("Open\n");
}
void doEnd()
{
   Gtk::Main::quit();
}

Gtk::AboutDialog *about;
void doHelp()
{
   about->show();
}
void doAboutClose(int a)
{
   about->hide();
}

void onFinished()
{
   theSymbolsView->set_model(theParser->getSymbols());
   theSymbolsView->append_column("Size", theParser->mColumns.size);
   theSymbolsView->append_column("S", theParser->mColumns.stab);
   theSymbolsView->append_column("Symbol", theParser->mColumns.symbol);
}

Gtk::ProgressBar *theProgressBar;
void onProgress(int a)
{
   printf("Done: %d\n",a);
   double f=(double)a/100;
   printf("Data %f",f);
 theProgressBar->set_fraction(f);
}

int
main (int argc, char *argv[])
{
   if(!Glib::thread_supported()) Glib::thread_init();

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

	builder->get_widget("main_window", main_win);

	Gtk::MenuItem* open=0;
	builder->get_widget("menu_open",open);
	open->signal_activate().connect( sigc::ptr_fun(doOpen));

	Gtk::MenuItem* end=0;
	   builder->get_widget("menu_end",end);
	   end->signal_activate().connect( sigc::ptr_fun(doEnd));

	   Gtk::MenuItem* help=0;
	      builder->get_widget("menu_help",help);
	      help->signal_activate().connect( sigc::ptr_fun(doHelp));


	      builder->get_widget("aboutdialog",about);
	      about->signal_response().connect(sigc::ptr_fun(doAboutClose));


	      builder->get_widget("progressbar",theProgressBar);
	      builder->get_widget("symbolsView",theSymbolsView);


 Glib::RefPtr<Gtk::IconFactory> factory = Gtk::IconFactory::create();

   std::list<Glib::RefPtr<Gdk::Pixbuf> > iconList;

   for (int i = 0; i < sizeof(sIconData) / sizeof(sIconData[0]); i++)
   {

      const IconData &icon_data = sIconData[i];

      iconList.push_back(Gdk::Pixbuf::create_from_inline(icon_data.length, icon_data.data));

   }

   Gtk::Window::set_default_icon_list(iconList);

   theParser=new FileParser(1);

   theParser->signal_finished().connect(
            sigc::ptr_fun(onFinished));

   theParser->signal_progress().connect(
            sigc::ptr_fun(onProgress));

	if (main_win)
	{
		kit.run(*main_win);
	}
	return 0;
}
