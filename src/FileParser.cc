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
//#include "demangle.h"

template<class A>
A min(const A a, const A b)
{
      return a<b ? a : b;
}

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
   if(progress_==102)
      return;
   printf("Progress: %d", progress_);
   if (progress_ == 101)
   {

      signal_finished_();
      progress_=102;
   }
   signal_progress_(progress_);
}

void show_globals(asymbol *sym, bfd *abfd, asymbol **syms, Glib::ustring &file)
{
   const char *filename;
   const char *funcname;
   unsigned int line;
   if (sym->flags & BSF_GLOBAL)
   {

      if (bfd_find_nearest_line(abfd, sym->section, syms, sym->value,
            &filename, &funcname, &line))
      {
         if (filename != 0)
         {
            file = sym->section->name;
            file.append("/");

            file.append(filename);
            return;
            //      printf("  in func %s- %s: %d\n", funcname, filename, line);
         }
      }
   }
   file = "UNKOWN";
   return;

}

void FileParser::addEntry(const Glib::ustring &file, const Glib::ustring &symbol, int size)
{

   Gtk::TreeIter iter = mSizeDistribution->get_iter("0");
   Glib::ustring curName = "";
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

void FileParser::fillPercentages()
{
   typedef Gtk::TreeModel::Children type_children; //minimise code length.

    type_children cur=mSizeDistribution->children();
    fillPercentages(cur,0);


}
void FileParser::fillPercentages(const Gtk::TreeModel::Children &cur, int d)
{
   if(d+1>mDepth)
      mDepth=d+1;
   unsigned int size=0;
   for(Gtk::TreeModel::Children::iterator iter = cur.begin();
         iter != cur.end(); ++iter)
   {
      Gtk::TreeModel::Row row = *iter;
      fillPercentages(row.children(),d+1);
      size+=row->get_value(mColumns.size);


   }



   for(Gtk::TreeModel::Children::iterator iter = cur.begin();
         iter != cur.end(); ++iter)
   {
      Gtk::TreeModel::Row row = *iter;
      unsigned int curSize=row->get_value(mColumns.size);

      double p;
      if(size==0)
      {
         curSize=42;
         p=1;
      }
      else
         p=(double)100*curSize/size;
      row->set_value(mColumns.percentage,p);
      row->set_value(mColumns.sizeShown,Glib::ustring(" SIZE"));
      row->set_value(mColumns.valid,1);


      Glib::ustring *size;
      size=new Glib::ustring();

   *size=   size->compose("%1 Bytes", curSize);

      Glib::ustring *name;
      name=new Glib::ustring();
      *name=row->get_value(mColumns.symbol);
      row->set_value(mColumns.csizeShown,(gchar*)size->c_str());
      row->set_value(mColumns.cstab,(gchar*)name->c_str());
      row->set_value(mColumns.csymbol,(gchar*)name->c_str());

   }

}

void FileParser::addSymbolToTree(Glib::ustring &name, unsigned int size)
{
   printf("Adding %s\n", name.c_str());


   typedef Gtk::TreeModel::Children type_children; //minimise code length.

   type_children cur=mSizeDistribution->children();
   cur=cur.begin()->children();
   bool done=false;
   size_t start=0;
   do
   {


      size_t end=min(name.find("\\",start),name.find("/",start));

      if(end==-1)
      {
         done=true;
         Glib::ustring curName=name.substr(start,end-start);

         printf(" File: %s\n", curName.c_str());
         Gtk::TreeModel::iterator nextBase;
         nextBase = mSizeDistribution->insert(cur.end());


         nextBase->set_value(mColumns.symbol,curName);
         nextBase->set_value(mColumns.size,size);

      }else{
         Glib::ustring curName=name.substr(start,end-start);
         printf(" Sub %s\n", curName.c_str());

         bool subFound=false;
         type_children::iterator nextBase;
         for(type_children::iterator iter = cur.begin();
               iter != cur.end(); ++iter)
         {
            Gtk::TreeModel::Row row = *iter;
            if(row.get_value(mColumns.symbol)==curName)
            {
               nextBase=iter;
               subFound=true;
               break;
            }

         }
         if(!subFound)
         {
            nextBase = mSizeDistribution->insert(cur.end());
            nextBase->set_value(mColumns.symbol,curName);
            nextBase->set_value(mColumns.size,0u);
         }
         nextBase->set_value(mColumns.size,(unsigned int)size+nextBase->get_value(mColumns.size));

         cur=nextBase->children();


         do
         {

            end++;
         }while(name[end]=='/' || name[end]=='\\');
         start=end;
         printf("%d %d",start,name.size());
      }

   }while(!done);

    cur=mSizeDistribution->children();
   cur.begin()->set_value(mColumns.size, size+cur.begin()->get_value(mColumns.size));
}

void FileParser::thread_function()
{
   Glib::Rand rand;

   mSymbols = Gtk::ListStore::create(mColumns);
   mSizeDistribution = Gtk::TreeStore::create(mColumns);

   bfd *theBfd = bfd_openr(mFile.c_str(), 0);

   bfd_init();
   mDepth=0;
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

   Gtk::TreeModel::Row row = *(mSizeDistribution->append());
   row[mColumns.stab] = "ALL";
   row[mColumns.size] = 0;
   int oldProgress=progress_;
   mSizeDistribution->clear();
   mSizeDistribution->append();
   typedef Gtk::TreeModel::Children type_children; //minimise code length.

   type_children cur=mSizeDistribution->children();

   cur.begin()->set_value(mColumns.symbol,Glib::ustring("Total"));

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


         const char* name = bfd_demangle(theBfd, skip_first + mangled_name, 11);

         Glib::ustring symbolName;
         if (name == 0)
         {
            Glib::ustring cName(symbol_table[i]->name);
            symbolName=cName;
            row[mColumns.symbol] = symbolName;

         }
         else
         {
            symbolName=name;
            row[mColumns.symbol] = symbolName;
            free((void*) name);

         }
         Glib::ustring tmp(symbolName);
         symbolName="";
         for(Glib::ustring::iterator it=tmp.begin();
               it!=tmp.end();
               it++)
         {
            symbolName.append(1, *it);
            if(*it=='&')
            {
               symbolName.append("amp;");
            }
         }

         Glib::ustring file = "";
         show_globals(symbol_table[i], theBfd, symbol_table, file);
         row[mColumns.stab] = file;


         Glib::ustring tree=file.compose("%1/%2", file,symbolName);
         addSymbolToTree(tree, size);

         progress_ = ((i * 100 / number_of_symbols));
         printf("%d, %d, %d\n", i, number_of_symbols, progress_);
         if(progress_!=oldProgress)
         {
            oldProgress=progress_;
            if(oldProgress<100)
               signal_increment_();
         }
      }


   }

   fillPercentages();

   progress_ = 101;
   signal_increment_();

}
FileParser &FileParser::self()
{
   return *this;
}

