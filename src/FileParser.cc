/*
 * FileParser.cc
 *
 *  Created on: 05.11.2010
 *      Author: thorsten
 */

#include "FileParser.hpp"
#include <stdio.h>
#include <iostream>

FileParser::FileParser(int id)
:
  thread_ (0),
  id_ (id),
  progress_ (0)
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

void FileParser::launch()
{
  // Create a joinable thread.
   progress_=0;
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
sigc::signal<void,int>& FileParser::signal_progress()
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

void FileParser::thread_function()
{
  Glib::Rand rand;

  for (int i = 0; i < ITERATIONS; ++i)
  {
    Glib::usleep(rand.get_int_range(6000, 20000));

    // Tell the main thread to increment the progress value.
    signal_increment_();
  }
}


