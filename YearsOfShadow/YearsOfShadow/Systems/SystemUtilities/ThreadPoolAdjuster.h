#pragma once
#include <HAL/HAL.h>

class ThreadPoolAdjuster {
    static constexpr uint32_t test_count = 100000000;  //100 mill
    static constexpr uint32_t iteration_count = 10;
    static uint8_t buff1[test_count];
    static uint8_t buff2[test_count];

public:
    static uint32_t getIdealThreadCount() {
        size_t maxThreadCount = HAL::get_hardware_thread_count() - 1;
        uint32_t bestThreadCount = 1;
        float bestThreadCountWorst = 0, bestThreadCountAverage = 0;
        HAL::thread_pool_handle_t tp = HAL::get_new_thread_pool(1);
        test(tp, bestThreadCountWorst, bestThreadCountAverage);
        HAL::release_thread_pool(tp);
        for (uint32_t i = 2; i < maxThreadCount; i++) {
            float worst = 0, average = 0;
            HAL::thread_pool_handle_t tp = HAL::get_new_thread_pool(i);
            test(tp, worst, average);
            HAL::release_thread_pool(tp);
            if (worst < bestThreadCountWorst) {
                bestThreadCountWorst = worst;
                bestThreadCount = i;
            }
        }
        return bestThreadCount;
    }

private:
    static void test(HAL::thread_pool_handle_t tp, float &worst, float &average) {
        //uint32_t threadCount = (uint32_t)HAL::get_thread_pool_free_thread_count(tp);
        //uint32_t workPerThread = test_count / threadCount;
        //uint32_t remainder = test_count % threadCount;
        threadCount = (uint32_t)HAL::get_thread_pool_free_thread_count(tp);
        workPerThread = test_count / threadCount;
        remainder = test_count % threadCount;

        for (uint32_t i = 0; i < iteration_count; i++) {
            clock_t c = clock();
            for (uint32_t j = 0; j < threadCount; j++) {
                HAL::submit_thread_pool_task(tp, threadedFunction, (void*)(uint64_t)j);
            }
            doWork(workPerThread * threadCount, remainder);
            while (HAL::is_thread_pool_finished(tp) == false)
                continue;
            c = clock() - c;
            if ((uint32_t)c > worst)
                worst = (float)c;
        }
    }
    static void threadedFunction(void* data) {
        uint32_t startingIndex = (uint32_t)(uint64_t)data;
        doWork(startingIndex, workPerThread);
    }
    static void doWork(uint32_t index, uint32_t count) {
        for (uint32_t i = index; i < count; i++) {
            buff1[i] += buff2[i];
        }
    }

    static uint32_t threadCount;
    static uint32_t workPerThread;
    static uint32_t remainder;
};

uint8_t ThreadPoolAdjuster::buff1[ThreadPoolAdjuster::test_count];
uint8_t ThreadPoolAdjuster::buff2[ThreadPoolAdjuster::test_count];
uint32_t ThreadPoolAdjuster::threadCount;
uint32_t ThreadPoolAdjuster::workPerThread;
uint32_t ThreadPoolAdjuster::remainder;