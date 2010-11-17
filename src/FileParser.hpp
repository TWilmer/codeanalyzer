/*
 * FileParser.hpp
 *
 *  Created on: 05.11.2010
 *      Author: thorsten
 */

#ifndef FILEPARSER_HPP_
#define FILEPARSER_HPP_
#include <gtkmm.h>
class FileParser
{
public:
      class MyModelColumns : public Gtk::TreeModel::ColumnRecord
      {
      public:
        Gtk::TreeModelColumn<unsigned int> size;
        Gtk::TreeModelColumn<gdouble> percentage;
        Gtk::TreeModelColumn<Glib::ustring> symbol;
        Gtk::TreeModelColumn<Glib::ustring> stab;

        Gtk::TreeModelColumn<Glib::ustring> sizeShown;

        Gtk::TreeModelColumn<gchar*> csymbol;
        Gtk::TreeModelColumn<gchar*> cstab;

        Gtk::TreeModelColumn<gchar*> csizeShown;
        Gtk::TreeModelColumn<int > valid;
        MyModelColumns() { add(size); add(percentage); add(stab); add(symbol);  add(sizeShown); add(valid); add(csizeShown); add(cstab);add(csymbol);}
      };
      MyModelColumns mColumns;
  explicit FileParser(int id);
  virtual ~FileParser();

  int id() const;
  void launch(const std::string &filename);
  void join();
  bool unfinished() const;

  sigc::signal<void>& signal_finished();
  sigc::signal<void,int>& signal_progress();
  Glib::RefPtr<Gtk::ListStore> getSymbols(){return mSymbols;}
  Glib::RefPtr<Gtk::TreeStore> getDistribution(){return mSizeDistribution;}
  void addEntry(const Glib::ustring &file, const Glib::ustring &symbol, int size );
  int getDepth(){return mDepth; }
  FileParser &self();
private:
  void addSymbolToTree(Glib::ustring &string, unsigned int size);
  void fillPercentages();
  /* also makes the depth calculation*/
  void fillPercentages(const Gtk::TreeModel::Children &cur, int d);

  enum { ITERATIONS = 100 };

  // Note that the thread does not write to the member data at all. It only
  // reads signal_increment_, which is only written to before the thread is
  // lauched. Therefore, no locking is required.
  Glib::Thread* thread_;
  int id_;
  int mDepth;
  unsigned int progress_;
  Glib::Dispatcher signal_increment_;
  sigc::signal<void> signal_finished_;
  sigc::signal<void,int> signal_progress_;
  void progress_increment();
  void thread_function();
  Glib::RefPtr<Gtk::ListStore> mSymbols;
  std::string mFile;

  Glib::RefPtr<Gtk::TreeStore> mSizeDistribution;
};


#endif /* FILEPARSER_HPP_ */
