#include <iostream>
#include <vector>
#include <chrono>

#include "WorkerThreadPool.h"

using namespace wheel;

void Thread_Manage()
{
    WorkerThreadPool worker_thread_pool(3);

    std::mutex   jobs_done_mutex;
    unsigned int jobs_done = 0;

    std::mutex              end_job1_mutex;
    std::condition_variable end_job1_var;
    bool                    end_job_1 = false;
    auto                    job1 = [&jobs_done, &end_job1_mutex, &end_job1_var, &end_job_1]
    {
        std::unique_lock<std::mutex> lock(end_job1_mutex);
        end_job1_var.wait_for(lock, std::chrono::milliseconds(1000u), [&end_job_1] { return end_job_1; });
        jobs_done++;
    };

    Waiter<void> waiter1 = worker_thread_pool.run<void>(job1);
    Waiter<void> waiter1_1 = worker_thread_pool.run<void>(job1);

    auto job2 = [&jobs_done, &jobs_done_mutex]
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(25u));
        std::lock_guard<std::mutex> lock(jobs_done_mutex);
        jobs_done++;
    };
    worker_thread_pool.run<void>(job2);
    worker_thread_pool.run<void>(job2);
    worker_thread_pool.run<void>(job2);
    worker_thread_pool.run<void>(job2);
    Waiter<void> waiter2 = worker_thread_pool.run<void>(job2);

    waiter2.wait();

    end_job_1 = true;
    end_job1_var.notify_all();

    waiter1.wait();
    waiter1_1.wait();
}

void Without_ReturnValue()
{
    WorkerThreadPool worker_thread_pool(3);

    auto job1 = [] { std::this_thread::sleep_for(std::chrono::milliseconds(20)); };
    auto job2 = [] { return; };
    auto job3 = []
    {
        throw std::exception();
        return false;
    };

    Waiter<void> waiter1 = worker_thread_pool.run<void>(job1);
    Waiter<void> waiter2 = worker_thread_pool.run<void>(job2);
    Waiter<void> waiter3 = worker_thread_pool.run<void>(job3);

    waiter1.wait();
    waiter2.wait();
    waiter3.wait();

    waiter1.success();
    waiter2.success();
    waiter3.success();
}

void With_ReturnValue()
{
    WorkerThreadPool worker_thread_pool(3);

    auto job1 = []
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return false;
    };
    auto job2 = [] { return true; };
    auto job3 = [] { return false; };
    auto job4 = []
    {
        throw std::exception();
        return false;
    };

    Waiter<bool> waiter1 = worker_thread_pool.run<bool>(job1);
    Waiter<bool> waiter2 = worker_thread_pool.run<bool>(job2);
    Waiter<bool> waiter3 = worker_thread_pool.run<bool>(job3);
    Waiter<bool> waiter4 = worker_thread_pool.run<bool>(job4);

    waiter1.wait();
    waiter2.wait();
    waiter3.wait();
    waiter4.wait();

    waiter1.success();
    waiter1.value();
    waiter2.success();
    waiter2.value();
    waiter3.success();
    waiter3.value();
    waiter4.success();

    auto job5 = []
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return std::string("Pif");
    };
    auto job6 = [] { return std::string("Paf"); };
    auto job7 = [] { return std::string("Pouf"); };
    auto job8 = []
    {
        throw std::exception();
        return std::string("Poum");
    };

    Waiter<std::string> waiter5 = worker_thread_pool.run<std::string>(job5);
    Waiter<std::string> waiter6 = worker_thread_pool.run<std::string>(job6);
    Waiter<std::string> waiter7 = worker_thread_pool.run<std::string>(job7);
    Waiter<std::string> waiter8 = worker_thread_pool.run<std::string>(job8);

    waiter5.wait();
    waiter6.wait();
    waiter7.wait();
    waiter8.wait();

    if (waiter5.success())
    {
    
    }

    if (waiter5.value() == "Pif")
    {
    
    }

    waiter6.success();

    if (waiter6.value(), "Paf")
    {
    
    }

    waiter7.success();

    if (waiter7.value(), "Pouf")
    {
    
    }

    waiter8.success();
}

int main()
{
    
    
    return 0;
}
