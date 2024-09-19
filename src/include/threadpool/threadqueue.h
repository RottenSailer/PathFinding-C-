/**
 @file threadqueue.h
 @author Satwik Shresth <ss5278@drexel.edu>
 @date 2023-2024
 @section Description

 This is a class that wraps queue to make it thread-safe
*/

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
/**
 * @class ThreadedQueue
 * @brief A thread-safe queue that stores any objects.
 *
 * This class provides a thread-safe implementation of a queue specifically for
 * storing any objects. It uses a std::mutex to ensure that push and pop
 * operations are atomic and safe to call from multiple threads.
 */
template <class T>
class ThreadedQueue : public std::queue<T>
{
private:
   mutable std::mutex      mtx;  ///< Mutex to protect access to the queue.
   std::condition_variable cv;   ///< conditional variable to notify other waiting threads

public:
   /**
    * @brief Push a any onto the queue.
    *
    * Thread-safe method to push a new CoOrdinate object onto the queue.
    *
    * @param value The CoOrdinate object to be pushed onto the queue.
    */
   void push(T value);

   /**
    * @brief Pop a any from the queue.
    *
    * Thread-safe method to pop the top any object from the queue.
    * if not avaiable waits for it to become abvilable
    *
    * @return void it doesnt return anything
    */
   T waitToPop();

   /**
    * @brief checks if queue is empty.
    *
    * @return bool return true if empty
    */
   bool empty();

   /**
    * @brief tries to pop the queue safely if empty returns false.
    *
    * @return bool return false if empty
    */
   bool tryPop(T& result);
};

template <class T>
void ThreadedQueue<T>::push(T value)
{
   std::lock_guard<std::mutex> lock(mtx);
   std::queue<T>::push(std::move(value));
   cv.notify_one();
}

template <class T>
bool ThreadedQueue<T>::empty()
{
   std::lock_guard<std::mutex> lock(mtx);
   return std::queue<T>::empty();
}

template <class T>
T ThreadedQueue<T>::waitToPop()
{
   std::unique_lock<std::mutex> lock(mtx);
   cv.wait(lock, [this] { return !std::queue<T>::empty(); });

   T temp = std::move(this->front());
   this->pop();

   return temp;
}

template <class T>
bool ThreadedQueue<T>::tryPop(T& result)
{
   std::lock_guard<std::mutex> lock(mtx);

   if (std::queue<T>::empty())
   {
      return false;
   }

   result = std::move(this->front());
   this->pop();

   return true;
}
