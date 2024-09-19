/**
 * @file ThreadPool.cpp
 * @brief This file contains the implmentation of the ThreadPool class, which manages a pool of threads to execute tasks concurrently.
 *
 * The ThreadPool class is responsible for managing a pool of worker threads that can execute tasks concurrently.
 * It provides methods to submit tasks, manage task queues, and ensure all tasks are completed before shutting down.
 *
 * Author: Satwik Shresth <ss5278@drexel.edu>
 * Date: 2023-2024
 */

#include "threadpool/threadpool.h"

thread_local unsigned int                           ThreadPool::myIndex        = 0;
thread_local WorkStealQueue<std::function<void()>>* ThreadPool::localWorkQueue = nullptr;

void ThreadPool::workerThread(unsigned myIndex_)
{
   myIndex        = myIndex_;
   localWorkQueue = queues[myIndex].get();
   while (!done)
   {
      runPendingTask();
      cv.notify_all();
   }
}

ThreadPool::ThreadPool() : threadCount(std::thread::hardware_concurrency()), done(false), joiner(threads)
{
   try
   {
      std::unique_lock<std::mutex> lock(mutex);
      for (unsigned i = 0; i < threadCount; ++i)
      {
         queues.emplace_back(new WorkStealQueue<std::function<void()>>);
      }

      for (unsigned i = 0; i < threadCount; ++i)
      {
         threads.emplace_back(&ThreadPool::workerThread, this, i);
      }
   }
   catch (...)
   {
      done = true;

      throw;
   }
}

unsigned ThreadPool::getThreadCount()
{
   return threadCount;
}

bool ThreadPool::isEmpty()
{
   for (auto& queue : queues)
   {
      if (!queue->empty())
      {
         return false;
      }
   }

   return workQueue.empty();
}

ThreadPool::~ThreadPool()
{
   {
      std::unique_lock<std::mutex> lock(mutex);
      cv.wait(lock, [&] { return isEmpty(); });
      done = true;
   }
   joiner.wait();
}

bool ThreadPool::popLocal(std::function<void()>& task)
{
   return localWorkQueue && localWorkQueue->tryPop(task);
}

bool ThreadPool::popPoolQueue(std::function<void()>& task)
{
   return workQueue.tryPop(task);
}

bool ThreadPool::popOtherThreads(std::function<void()>& task)
{
   for (unsigned i = 0; i < queues.size(); ++i)
   {
      unsigned const index = (myIndex + i + 1) % queues.size();
      if (queues.at(index)->trySteal(task))
      {
         return true;
      }
   }
   return false;
}

void ThreadPool::runPendingTask()
{
   std::function<void()> task;
   if (popLocal(task) || popPoolQueue(task) || popOtherThreads(task))
   {
      task();
   }
   else
   {
      std::this_thread::yield();
   }
}

void ThreadPool::submit(std::function<void()> task)
{
   if (localWorkQueue)
   {
      localWorkQueue->push(std::move(task));
   }
   else
   {
      workQueue.push(std::move(task));
   }
}
