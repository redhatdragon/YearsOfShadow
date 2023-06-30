#pragma once
#include <vector>
//#include <Windows.h>
#include <iostream>
#include <deque>
//#include <atomic>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <optional>
#include <semaphore>
#include <ranges>
#include <variant>
#include <future>

namespace rn = std::ranges;
namespace vi = rn::views;



//class CustomThreadPool {
//    using Task = std::move_only_function<void()>;
//public:
//    CustomThreadPool(size_t numWorkers) {
//        workers_.reserve(numWorkers);
//        for (size_t i = 0; i < numWorkers; i++) {
//            workers_.emplace_back(this);
//        }
//    }
//    template<typename F, typename...A>
//    auto Run(F&& function, A&&...args)
//    {
//        using ReturnType = std::invoke_result_t<F, A...>;
//        auto pak = std::packaged_task<ReturnType()>{ std::bind(
//            std::forward<F>(function), std::forward<A>(args)...
//        ) };
//        auto future = pak.get_future();
//        Task task{ [pak = std::move(pak)]() mutable { pak(); } };
//        {
//            std::lock_guard lk{ taskQueueMtx_ };
//            tasks_.push_back(std::move(task));
//        }
//        taskQueueCv_.notify_one();
//        return future;
//    }
//    void WaitForAllDone()
//    {
//        std::unique_lock lk{ taskQueueMtx_ };
//        allDoneCv_.wait(lk, [this] {return tasks_.empty(); });
//    }
//    ~CustomThreadPool()
//    {
//        for (auto& w : workers_) {
//            w.RequestStop();
//        }
//    }
//
//private:
//    // functions
//    Task GetTask_(std::stop_token& st)
//    {
//        Task task;
//        std::unique_lock lk{ taskQueueMtx_ };
//        taskQueueCv_.wait(lk, st, [this] {return !tasks_.empty(); });
//        if (!st.stop_requested()) {
//            task = std::move(tasks_.front());
//            tasks_.pop_front();
//            if (tasks_.empty()) {
//                allDoneCv_.notify_all();
//            }
//        }
//        return task;
//    }
//    // types
//    class Worker_
//    {
//    public:
//        Worker_(ThreadPool* pool) : pool_{ pool }, thread_(std::bind_front(&Worker_::RunKernel_, this)) {}
//        void RequestStop()
//        {
//            thread_.request_stop();
//        }
//    private:
//        // functions
//        void RunKernel_(std::stop_token st)
//        {
//            while (auto task = pool_->GetTask_(st)) {
//                task();
//            }
//        }
//        // data
//        ThreadPool* pool_;
//        std::jthread thread_;
//    };
//    // data
//    std::mutex taskQueueMtx_;
//    std::condition_variable_any taskQueueCv_;
//    std::condition_variable allDoneCv_;
//    std::deque<Task> tasks_;
//    std::vector<Worker_> workers_;
//};




//class CustomThreadPool {
//	//static constexpr uint32_t MAX_POSSIBLE_THREADS = 256;
//	//CustomCPP11Thread threads[MAX_POSSIBLE_THREADS];
//	//uint16_t maxThreadCount;
//
//	//void ThreadLoop();
//
//	bool should_terminate = false;           // Tells threads to stop looking for jobs
//	std::mutex queue_mutex;                  // Prevents data races to the job queue
//	std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
//	std::vector<std::thread> threads;
//	//std::queue<std::function<void()>> jobs;
//	std::queue<void (*)(void*)> jobs;
//	std::queue<void*> datas;
//	//std::vector<void*> data;
//public:
//	void init(uint16_t _maxThreadCount) {
//		threads.resize(_maxThreadCount);
//		for (uint32_t i = 0; i < _maxThreadCount; i++) {
//			threads.at(i) = std::thread(ThreadLoop, this);
//		}
//	}
//	void addTask(void (*_foo)(void*), void* _data) {
//		{
//			std::unique_lock<std::mutex> lock(queue_mutex);
//			//jobs.push(job);
//
//			jobs.push(_foo);
//			datas.push(_data);
//		}
//		mutex_condition.notify_one();
//	}
//
//	bool allTasksFinished() {
//		//ThreadLoop(this);
//
//		while (true) {
//			bool isFinished;
//			{
//				std::unique_lock<std::mutex> lock(queue_mutex);
//				isFinished = jobs.empty();
//			}
//			if (isFinished)
//				break;
//		}
//		return true;
//	}
//	uint16_t getFreeThreadCount() {
//		
//	}
//
//	static void ThreadLoop(CustomThreadPool* self) {
//		while (true) {
//			//std::function<void()> job;
//			void (*job)(void*);
//			void* data;
//			{
//				std::unique_lock<std::mutex> lock(self->queue_mutex);
//				self->mutex_condition.wait(lock, [self] {
//					//return !self->jobs.empty() || self->should_terminate;
//					});
//				if (self->should_terminate) {
//					//return;
//				}
//				job = self->jobs.front();
//				data = self->datas.front();
//				self->jobs.pop();
//				self->datas.pop();
//			}
//			job(data);
//		}
//	}
//};