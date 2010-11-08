/*
 * FileParser.cc
 *
 *  Created on: 05.11.2010
 *      Author: thorsten
 */

#include "FileParser.hpp"
#include <stdio.h>
#include <iostream>
#include <bfd.h>
#include "elf-bfd.h"
#define HAVE_DECL_BASENAME 1
#include "demangle.h"

FileParser::FileParser(int id) :
   thread_(0), id_(id), progress_(0), mFile("")
{
   // Connect to the cross-thread signal.
   signal_increment_.connect(sigc::mem_fun(*this, &FileParser::progress_increment));
}

FileParser::~FileParser()
{
   // It is an error if the thread is still running at this point.
   g_return_if_fail(thread_ == 0);
}

int FileParser::id() const
{
   return id_;
}

void FileParser::launch(const std::string &filename)
{
   // Create a joinable thread.
   progress_ = 0;
   mFile = filename;
   thread_ = Glib::Thread::create(sigc::mem_fun(*this, &FileParser::thread_function), true);
}

void FileParser::join()
{
   thread_->join();
   thread_ = 0;
}

bool FileParser::unfinished() const
{
   return (progress_ < ITERATIONS);
}

sigc::signal<void>& FileParser::signal_finished()
{
   return signal_finished_;
}
sigc::signal<void, int>& FileParser::signal_progress()
{
   return signal_progress_;
}

void FileParser::progress_increment()
{
   ++progress_;
   std::cout << "Thread " << id_ << ": " << progress_ << '%' << std::endl;

   if (progress_ >= ITERATIONS)
      signal_finished_();
   signal_progress_(progress_);
}

void show_globals(asymbol *sym, bfd *abfd, asymbol **syms,Glib::ustring &file) {
   const char *filename;
   const char *funcname;
   unsigned int line;
   if (sym->flags & BSF_GLOBAL) {

      if (bfd_find_nearest_line(abfd, sym->section, syms, sym->value,
            &filename, &funcname, &line)) {
         if(filename!=0)
         {
            file="ALL/";
            file.append(filename);
            return;
      //      printf("  in func %s- %s: %d\n", funcname, filename, line);
         }
      }
   }
   file="ALL";
   return;

}


void FileParser::addEntry(const Glib::ustring &file, const Glib::ustring &symbol, int size )
{

Gtk::TreeIter iter=mSizeDistribution->get_iter("0");
Glib::ustring curName="";
printf("Inserting: %s\n", file.c_str());
/*for(Glib::ustring::iterator it=file.begin();it!=file.end();it++)
{
   if(*it=='/')
   {
     printf("Found: %s\n", curName.c_str());
     Gtk::TreeNodeChildren childs=iter->children();
     for(Gtk::TreeIter cIt=childs.begin(); cIt!=childs.end(); cIt++)
     {
        if(cIt->get_value(mColumns.stab)==curName)
        {
           break;
        }
     }

      Glib::ustring pos= iter->get_value(mColumns.stab);

      curName="";
   }else{
      curName.append(1,*it);
   }
}*/



}

void FileParser::thread_function()
{
   Glib::Rand rand;

   mSymbols = Gtk::ListStore::create(mColumns);
   mSizeDistribution=Gtk::TreeStore::create(mColumns);

   bfd *theBfd = bfd_openr(mFile.c_str(), 0);

   bfd_init();
   long storage_needed;
   asymbol **symbol_table;
   long number_of_symbols;
   long i;
   if (theBfd == 0)
   {
      printf("Could not read file\n");
      return;
   }
   bfd_check_format(theBfd, bfd_object);
   storage_needed = bfd_get_symtab_upper_bound (theBfd);

   if (storage_needed < 0)
      return;

   if (storage_needed == 0)
      return;


   symbol_table = (asymbol **) malloc(storage_needed);

   number_of_symbols = bfd_canonicalize_symtab (theBfd, symbol_table);

   if (number_of_symbols < 0)
      return;

   cplus_demangle_set_style(auto_demangling);
   Gtk::TreeModel::Row row =*(   mSizeDistribution->append());
   row[mColumns.stab]="ALL";
   row[mColumns.size]=0;

   for (i = 0; i < number_of_symbols; i++)
   {
      unsigned int size = ((elf_symbol_type *) symbol_table[i])->internal_elf_sym.st_size;
      if (size > 0)
      {
         Gtk::TreeModel::iterator iter = mSymbols->append();
         Gtk::TreeModel::Row row = *iter;

         row[mColumns.size] = size;

         unsigned int skip_first = 0;
         const char* mangled_name = symbol_table[i]->name;
         if (mangled_name[0] == '.' || mangled_name[0] == '$')
            ++skip_first;
         /*if ( mangled_name[skip_first] == '_')
          ++skip_first;*/

         int flags = DMGL_PARAMS | DMGL_ANSI | DMGL_VERBOSE;


         const char* name = bfd_demangle(theBfd, skip_first + mangled_name, 11);

         if (name == 0)
         {
            Glib::ustring cName(symbol_table[i]->name);

            row[mColumns.symbol] = cName;

         }
         else
         {
            row[mColumns.symbol] = name;
            free((void*) name);

         }


         Glib::ustring file="";
         show_globals(symbol_table[i],theBfd,symbol_table, file);
         row[mColumns.stab]=file;
      }

      signal_progress_((i * 100 / number_of_symbols));
   }

   signal_finished_();

}

