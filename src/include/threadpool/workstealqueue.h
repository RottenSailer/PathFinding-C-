/**
 * @file WorkStealQueue.h
 * @brief This file contains the definition of the WorkStealQueue class template, which implements a thread-safe work-stealing queue.
 *
 * @section Description
 * The WorkStealQueue class is designed to allow tasks to be pushed and popped by the owner thread,
 * and also stolen by other threads to balance the workload across multiple threads.
 *
 * @section Reference
 * This program was developed with insights and techniques from the book:
 * - C++ Concurrency in Action (Second Edition) by Anthony Williams.
 *
 * Author: Satwik Shresth <ss5278@drexel.edu>
 * Date: 2023-2024
 */

#pragma once

#include <deque>
#include <mutex>

/**
 * @class WorkStealQueue
 * @brief Implements a thread-safe work-stealing queue.
 *
 * @tparam T The type of elements stored in the queue.
 */
template <class T>
class WorkStealQueue
{
private:
   std::deque<T>      deque;  ///< The double-ended queue to store tasks.
   mutable std::mutex mutex;  ///< Mutex for synchronizing access.

public:
   /**
    * @brief Construct a new WorkStealQueue object.
    */
   WorkStealQueue()
   {}

   /**
    * @brief Check if the queue is empty.
    *
    * @return true if the queue is empty, false otherwise.
    */
   bool empty();

   /**
    * @brief Push a task onto the front of the queue.
    *
    * @param data The task to be pushed.
    */
   void push(T data);

   /**
    * @brief Try to steal a task from the back of the queue.
    *
    * @param result The task that was stolen.
    * @return true if a task was successfully stolen, false otherwise.
    */
   bool trySteal(T& result);

   /**
    * @brief Try to pop a task from the front of the queue.
    *
    * @param result The task that was popped.
    * @return true if a task was successfully popped, false otherwise.
    */
   bool tryPop(T& result);
};

template <class T>
bool WorkStealQueue<T>::empty()
{
   return deque.empty();
}

template <class T>
void WorkStealQueue<T>::push(T data)
{
   std::lock_guard<std::mutex> lock(mutex);
   deque.push_front(std::move(data));
}

template <class T>
bool WorkStealQueue<T>::trySteal(T& result)
{
   std::lock_guard<std::mutex> lock(mutex);

   if (deque.empty())
   {
      return false;
   }

   result = std::move(deque.back());
   deque.pop_back();

   return true;
}

template <class T>
bool WorkStealQueue<T>::tryPop(T& result)
{
   std::lock_guard<std::mutex> lock(mutex);

   if (deque.empty())
   {
      return false;
   }

   result = std::move(deque.front());
   deque.pop_front();

   return true;
}
