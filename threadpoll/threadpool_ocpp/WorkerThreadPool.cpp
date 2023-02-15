/*
Copyright (c) 2023 Markjl

Free software: you can redistribute it and/or modify it under the terms
of the GNU Lesser General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.
*/

#include "WorkerThreadPool.h"

namespace wheel
{
/** @brief Constructor */
WorkerThreadPool::WorkerThreadPool(size_t thread_count) : m_stop(false), m_threads(), m_job_queue()
{
    // Create thread
    for (size_t i = 0; i < thread_count; i++)
    {
        m_threads.push_back(new std::thread(std::bind(&WorkerThreadPool::workerThread, this)));
    }
}

/** @brief Destructor */
WorkerThreadPool::~WorkerThreadPool()
{
    // Stop threads
    m_stop = true;
    m_job_queue.setEnable(false);

    // Wait end of thread an release resources
    for (std::thread* thread : m_threads)
    {
        thread->join();
        delete thread;
    }
}

/** @brief Worker thread */
void WorkerThreadPool::workerThread()
{
    std::shared_ptr<IJob> job;

    // Thread loop
    while (!m_stop)
    {
        // Wait for a job
        if (m_job_queue.pop(job))
        {
            // Execute job
            job->run();

            // Release resources
            job.reset();
        }
    }
}

} // namespace wheel
