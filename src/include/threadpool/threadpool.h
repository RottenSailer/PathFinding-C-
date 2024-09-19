/**
 * @file ThreadPool.h
 * @brief This file contains the definition of the ThreadPool class, which manages a pool of threads to execute tasks concurrently.
 * 
 * @section Description
 * The ThreadPool class is responsible for managing a pool of worker threads that can execute tasks concurrently. 
 * It provides methods to submit tasks, manage task queues, and ensure all tasks are completed before shutting down.
 *
 * @section Reference
 * This program was developed with insights and techniques from the book:
 * - C++ Concurrency in Action (Second Edition) by Anthony Williams.
 * 
 * Author: Satwik Shresth <ss5278@drexel.edu>
 * Date: 2023-2024
 */

#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include "threadpool/jointhreads.h"
#include "threadpool/threadqueue.h"
#include "threadpool/workstealqueue.h"

/**
 * @class ThreadPool
 * @brief Manages a pool of threads to execute tasks concurrently.
 */
class ThreadPool
{
private:
   const unsigned                                                      threadCount;    ///< The number of threads in the pool.
   std::atomic<bool>                                                   done;           ///< Atomic flag to indicate if the pool is shutting down.
   std::vector<std::thread>                                            threads;        ///< Vector of worker threads.
   JoinThreads                                                         joiner;         ///< Helper object to ensure threads are joined on destruction.
   ThreadedQueue<std::function<void()>>                                workQueue;      ///< Queue of tasks for the threads to execute.
   std::vector<std::unique_ptr<WorkStealQueue<std::function<void()>>>> queues;         ///< Vector of work-stealing queues for the threads.
   thread_local static WorkStealQueue<std::function<void()>>*          localWorkQueue; ///< Thread-local pointer to the work-stealing queue.
   thread_local static unsigned                                        myIndex;        ///< Thread-local index of the worker thread.
   mutable std::mutex                                                  mutex;          ///< Mutex for synchronizing access.
   std::condition_variable                                             cv;             ///< Condition variable for task notification.

   /**
    * @brief Function executed by each worker thread.
    *
    * @param myIndex_ The index of the worker thread.
    */
   void workerThread(unsigned myIndex_);

public:
   /**
    * @brief Construct a new ThreadPool object.
    */
   ThreadPool();

   /**
    * @brief Get the number of threads in the pool.
    * 
    * @return The number of threads in the pool.
    */
   unsigned getThreadCount();

   /**
    * @brief Check if the task queue is empty.
    * 
    * @return true if the task queue is empty, false otherwise.
    */
   bool isEmpty();

   /**
    * @brief Destroy the ThreadPool object.
    */
   ~ThreadPool();

   /**
    * @brief Pop a task from the local work queue.
    * 
    * @param task The task to be executed.
    * @return true if a task was successfully popped, false otherwise.
    */
   bool popLocal(std::function<void()>& task);

   /**
    * @brief Pop a task from the pool's work queue.
    * 
    * @param task The task to be executed.
    * @return true if a task was successfully popped, false otherwise.
    */
   bool popPoolQueue(std::function<void()>& task);

   /**
    * @brief Pop a task from other threads' work queues.
    * 
    * @param task The task to be executed.
    * @return true if a task was successfully popped, false otherwise.
    */
   bool popOtherThreads(std::function<void()>& task);

   /**
    * @brief Run a pending task from any available queue.
    */
   void runPendingTask();

   /**
    * @brief waits for all the queued task to complete
    */
   void wait();

   /**
    * @brief Submit a task to the thread pool.
    * 
    * @param task The task to be executed.
    */
   void submit(std::function<void()> task);
};


