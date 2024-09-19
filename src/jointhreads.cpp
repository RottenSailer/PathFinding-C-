/**
 * @file jointrheads.cpp
 * @brief This file contains the implementation of the JoinThreads class, which manages a collection of threads and ensures they are properly joined.
 *
 * Author: Satwik Shresth <ss5278@drexel.edu>
 * Date: 2023-2024
 */
#include <threadpool/jointhreads.h>

JoinThreads::JoinThreads(std::vector<std::thread>& threads_) : threads(threads_)
{}

void JoinThreads::wait()
{
   for (unsigned long i = 0; i < threads.size(); ++i)
   {
      if (threads[i].joinable())
      {
         threads[i].join();
      }
   }
}

JoinThreads::~JoinThreads()
{
   wait();
}
