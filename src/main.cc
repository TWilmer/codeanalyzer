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
#include "slice.h"
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

#include "baobab/baobab-chart.h"
#include "baobab/baobab-ringschart.h"

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
#define GLADE_START binary_codeanalyzer_glade_start
#define GLADE_END binary_codeanalyzer_glade_end
#else
#define GLADE_START _binary_codeanalyzer_glade_start
#define GLADE_END _binary_codeanalyzer_glade_end
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
Gtk::TreeView* theDebugView=0;

Gtk::Label *theLocation;
void doOpen()
{



   Gtk::FileChooserDialog dialog(_("Please choose a file"),
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*main_win);

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:
    Gtk::FileFilter filter_any;
    filter_any.set_name(_("Any files"));
    filter_any.add_pattern("*");
    dialog.add_filter(filter_any);


    Gtk::FileFilter filter_text;
    filter_text.set_name(_("Application File"));
    filter_text.add_mime_type("application/x-executable");
    dialog.add_filter(filter_text);

    Gtk::FileFilter filter_obj;
    filter_obj.set_name(_("Object File"));
    filter_obj.add_mime_type("application/x-object");
    dialog.add_filter(filter_obj);



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
Gtk::AboutDialog dialog;
dialog.set_transient_for(*main_win);
dialog.set_copyright(about->get_copyright());
dialog.set_license(about->get_license());
dialog.set_authors(about->get_authors());
dialog.set_title(about->get_title());
dialog.set_program_name(about->get_program_name());
dialog.run();

/*
   about->show();
   about->raise();
   about->present();*/

}
void doAboutClose(int a)
{
   about->hide();

}
Gtk::ProgressBar *theProgressBar;
GtkWidget * rings_chart;
Gtk::Alignment *ringAlignment=0;
GtkTreeStore *model;
void onFinished()
{


   theSymbolsView->set_model(theParser->getSymbols());
   theSymbolsView->remove_all_columns();
   theSymbolsView->append_column(_("Size"), theParser->mColumns.size);
   theSymbolsView->append_column(_("Source"), theParser->mColumns.stab);
   theSymbolsView->append_column(_("Symbol"), theParser->mColumns.symbol);
   theProgressBar->set_fraction(1);

theDebugView->set_model(theParser->getDistribution());
theDebugView->remove_all_columns();
theDebugView->append_column("Symbol", theParser->mColumns.symbol);
theDebugView->append_column("Size", theParser->mColumns.size);
theDebugView->append_column("Percentage", theParser->mColumns.percentage);


   baobab_chart_freeze_updates(rings_chart);
   model=theParser->getDistribution()->gobj();
   baobab_chart_set_model_with_columns (rings_chart,
                                        GTK_TREE_MODEL (model),
                                        theParser->mColumns.csymbol.index()    ,
                                        theParser->mColumns.csizeShown.index(),
                                        theParser->mColumns.cstab.index(),
                                        theParser->mColumns.percentage.index(),
                                        theParser->mColumns.valid.index(),
                                        NULL);

   baobab_chart_freeze_updates(rings_chart);
   baobab_chart_thaw_updates(rings_chart);
   baobab_chart_set_max_depth(rings_chart, theParser->getDepth());

}


void onProgress(int a)
{
   if(a<100)
   {
      printf("Done: %d\n",a);
      double f=(double)a/100;
      printf("Data %f",f);


      theProgressBar->set_fraction(f);
   }
}
extern "C"
{
   void onChart_item_activated (BaobabChart *chart, GtkTreeIter *iter);
   gboolean on_chart_button_release (BaobabChart *chart, GdkEventButton *event,
                                          gpointer data);
};



void onChart_item_activated (BaobabChart *chart, GtkTreeIter *iter)
{

   GtkTreePath *path;

       path = gtk_tree_model_get_path (GTK_TREE_MODEL (model), iter);

       baobab_chart_set_root(rings_chart,path);
       theDebugView->expand_to_path(Gtk::TreePath(path));
       theDebugView->scroll_to_row(Gtk::TreePath(path));



       Glib::ustring location="";

       Glib::RefPtr<Gtk::TreeStore> aModel=theParser->getDistribution();
       Gtk::TreePath aPath(path);
       do
       {
          location=location.compose("%1/%2",aModel->get_iter(aPath)->get_value(theParser->mColumns.symbol),location);
       }
       while(aPath.up());


       theLocation->set_text(location);

       gtk_tree_path_free (path);


}
gboolean onChart_button_release (BaobabChart *chart, GdkEventButton *event,
                                       gpointer data)
{
   printf("Item ac button\n;");
}



/* tree model columns (_H_ are hidden) */
enum
{
   COL_DIR_NAME,
   COL_H_PARSENAME,
   COL_H_PERC,
   COL_DIR_SIZE,
   COL_H_ELEMENTS,
   NUM_TREE_COLUMNS
};

int
main (int argc, char *argv[])
{
   g_type_init();
   if(!Glib::thread_supported()) Glib::thread_init();

   Glib::RefPtr< Gio::File > localeDeRel=    Gio::File::create_for_commandline_arg("../share/locale/de/LC_MESSAGES/codeanalyzer.mo");
printf("%s", localeDeRel->get_path().c_str());
   if(localeDeRel->query_exists ())
   {
      printf("Using relative path\n");
      bindtextdomain (GETTEXT_PACKAGE, "../share/locale");
   }else{
      bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
      printf("Using absolute path %s\n",PACKAGE_LOCALE_DIR);
   }


bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
textdomain (GETTEXT_PACKAGE);



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
	      about->set_transient_for (*main_win);



	      builder->get_widget("progressbar",theProgressBar);
	      builder->get_widget("symbolsView",theSymbolsView);
	      builder->get_widget("debugView",theDebugView);

         builder->get_widget("location",theLocation);



 Glib::RefPtr<Gtk::IconFactory> factory = Gtk::IconFactory::create();

   std::list<Glib::RefPtr<Gdk::Pixbuf> > iconList;

   for (int i = 0; i < sizeof(sIconData) / sizeof(sIconData[0]); i++)
   {

      const IconData &icon_data = sIconData[i];

      iconList.push_back(Gdk::Pixbuf::create_from_inline(icon_data.length, icon_data.data));

   }

   Gtk::Window::set_default_icon_list(iconList);



   builder->get_widget("ringchartAlignment",ringAlignment);



 rings_chart = (GtkWidget *) baobab_ringschart_new ();




   baobab_chart_set_max_depth (rings_chart, 20);

   g_signal_connect (rings_chart, "item_activated",
               G_CALLBACK (onChart_item_activated), NULL);


   gtk_widget_show (rings_chart);
   gtk_container_add(GTK_CONTAINER(ringAlignment->gobj()), rings_chart);
   gtk_widget_show_all(rings_chart);


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
