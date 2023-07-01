#include <EchoEngine/EE_Types.h>
#include "Systems/Systems.h"
#include <thread>

void sleepForThreaded(void* data) {
    std::this_thread::sleep_for(std::chrono::milliseconds(reinterpret_cast<int64_t>(data)));
}



void* exampleMesh;
void* exampleInstancedMeshTest;
void HAL::appStart() {
    initSystems();
}
bool FPressedLastTick = true;
;
void HAL::appLoop() {
    HAL::draw_background(0, 0, 0, 255);
    ecs.runSystems();
    // auto dbgInfo = ecs.getDebugInfoStr();
    // for (auto& i : dbgInfo)
    //	std::cout << i << std::endl;
    // auto dbgInfo = ecs.getDebugInfoStr();
    // std::string dbgStr = "";
    // for (auto &str : dbgInfo)
    // {
    //     dbgStr += str;
    //     dbgStr += '\n';
    // }
    // fwrite(dbgStr.c_str(), dbgStr.size(), 1, stderr);

#ifdef REWIND_ENABLED
    static int ticksSinceRewind = 0;
    if (EE_getKeyState('F'))
    {
        if (FPressedLastTick == false && ticksSinceRewind >= 90)
        {
            physics.rewind(60);
            ticksSinceRewind = 0;
        }
        FPressedLastTick = true;
    }
    else
        FPressedLastTick = false;
    ticksSinceRewind++;
#endif
}
void HAL::appPostFrame() {
    while (HAL::is_thread_pool_finished(threadPool) == false)
        continue;
    u16 threadCount = static_cast<u16>(HAL::get_thread_pool_free_thread_count(threadPool));
    clock_t remainingTime = static_cast<clock_t>(HAL::get_app_loop_time_MS() + HAL::get_draw_time_MS());
    i32 waitFor = 16 - remainingTime;
    // std::string printStr = "waitFor: ";
    // printStr += std::to_string(waitFor);
    // printStr += " LoopTime: ";
    // printStr += std::to_string(HAL::get_app_loop_time_MS());
    // printStr += " DrawTime: ";
    // printStr += std::to_string(HAL::get_draw_time_MS());
    // printStr += '\n';
    // fwrite(printStr.c_str(), printStr.size(), 1, stderr);
    //if (waitFor <= 4)
    //    return;
    //for (u32 i = 0; i < threadCount; i++)
    //    HAL::submit_thread_pool_task(threadPool, sleepForThreaded, to_void_ptr(waitFor-2));
    //while (HAL::is_thread_pool_finished(threadPool) == false)
    //    continue;
}
void HAL::appEnd() {

}