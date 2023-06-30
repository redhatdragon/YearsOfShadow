#include <EchoEngine/EE_Types.h>
#include <HAL/HAL_3D.h>
#include "Systems/Systems.h"


void sleepForThreaded(void* data) {
   _sleep((int64_t)data);
}



void* exampleMesh;
void* exampleInstancedMeshTest;
void EE_appStart() {
    initSystems();
}
bool FPressedLastTick = true;
;
void EE_appLoop() {
    EE_drawBackground(0, 0, 0, 255);
    ecs.runSystems();
    // auto dbgInfo = ecs.getDebugInfoStr();
    // for (auto& i : dbgInfo)
    //	std::cout << i << std::endl;
    auto dbgInfo = ecs.getDebugInfoStr();
    std::string dbgStr = "";
    for (auto &str : dbgInfo)
    {
        dbgStr += str;
        dbgStr += '\n';
    }
    fwrite(dbgStr.c_str(), dbgStr.size(), 1, stderr);

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
void EE_appPostFrame() {
    while (EE_isThreadPoolFinished(threadPool) == false)
        continue;
    u16 threadCount = EE_getThreadPoolFreeThreadCount(threadPool);
    clock_t remainingTime = static_cast<clock_t>(EE_getAppLoopTimeMS() + EE_getDrawTimeMS());
    i32 waitFor = 16 - remainingTime;
    std::string printStr = "waitFor: ";
    printStr += std::to_string(waitFor);
    printStr += " LoopTime: ";
    printStr += std::to_string(EE_getAppLoopTimeMS());
    printStr += " DrawTime: ";
    printStr += std::to_string(EE_getDrawTimeMS());
    printStr += '\n';
    fwrite(printStr.c_str(), printStr.size(), 1, stderr);
    if (waitFor <= 2)
        return;
    for (u32 i = 0; i < threadCount; i++)
        EE_sendThreadPoolTask(threadPool, sleepForThreaded, to_void_ptr(waitFor));
    while (EE_isThreadPoolFinished(threadPool) == false)
        continue;
}
void EE_appEnd() {

}