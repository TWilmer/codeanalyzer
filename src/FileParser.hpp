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
  explicit FileParser(int id);
  virtual ~FileParser();

  int id() const;
  void launch();
  void join();
  bool unfinished() const;

  sigc::signal<void>& signal_finished();
  sigc::signal<void,int>& signal_progress();
private:
  enum { ITERATIONS = 100 };

  // Note that the thread does not write to the member data at all. It only
  // reads signal_increment_, which is only written to before the thread is
  // lauched. Therefore, no locking is required.
  Glib::Thread* thread_;
  int id_;
  unsigned int progress_;
  Glib::Dispatcher signal_increment_;
  sigc::signal<void> signal_finished_;
  sigc::signal<void,int> signal_progress_;
  void progress_increment();
  void thread_function();
};


#endif /* FILEPARSER_HPP_ */
