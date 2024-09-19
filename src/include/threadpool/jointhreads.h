/**
 * @author  Satwik Shresth <ss5278@drexel.edu>
 * @date June 12, 2024
 *
 * @file jointrheads.h
 * @brief This file contains the definition of the JoinThreads class, which manages a collection of threads and ensures they are properly joined.
 *
 * @section Reference
 * This program was developed with insights and techniques from the book:
 * - C++ Concurrency in Action (Second Edition) by Anthony Williams.
 *
 */

#pragma once

#include <thread>
#include <vector>

/**
 * @class JoinThreads
 * @brief A RAII class to manage a collection of threads and ensure they are properly joined.
 */
class JoinThreads
{
private:
   /**
    * @brief A reference to a vector of threads that need to be joined.
    */
   std::vector<std::thread>& threads;

public:
   /**
    * @brief Constructor that initializes the JoinThreads object with a vector of threads.
    *
    * @param threads_ A reference to a vector of threads to be managed.
    */
   explicit JoinThreads(std::vector<std::thread>& threads_);

   /**
    * @brief Joins all the threads in the managed vector.
    */
   void wait();

   /**
    * @brief Destructor that ensures all threads are joined before the object is destroyed.
    */
   ~JoinThreads();
};
