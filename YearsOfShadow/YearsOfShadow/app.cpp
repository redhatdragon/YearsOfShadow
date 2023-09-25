#include <EchoEngine/EE_Types.h>
#include "Systems/Systems.h"
#include <thread>
#include <optick.h>
#include <Windows.h>

static NTSTATUS(__stdcall *NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) =
    (NTSTATUS(__stdcall *)(BOOL, PLARGE_INTEGER))GetProcAddress(GetModuleHandle("ntdll.dll"), "NtDelayExecution");
static NTSTATUS(__stdcall *ZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set,
                                                 OUT PULONG ActualResolution) =
    (NTSTATUS(__stdcall *)(ULONG, BOOLEAN, PULONG))GetProcAddress(GetModuleHandle("ntdll.dll"), "ZwSetTimerResolution");

static void SleepShort(float milliseconds)
{
    static bool once = true;
    if (once)
    {
        ULONG actualResolution;
        ZwSetTimerResolution(1, true, &actualResolution);
        once = false;
    }

    LARGE_INTEGER interval;
    interval.QuadPart = -1 * (int)(milliseconds * 10000.0f);
    NtDelayExecution(false, &interval);
}

void sleepForThreaded(void* data) {
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    OPTICK_THREAD(ss.str().c_str());
    OPTICK_EVENT();
    //std::this_thread::sleep_for(std::chrono::milliseconds(reinterpret_cast<int64_t>(data)));
    SleepShort((float)reinterpret_cast<int64_t>(data));
}



void netTest() {
    std::string inChar;
    std::cout << "Enter y if running client" << std::endl;
    std::cin >> inChar;
    HAL::udp_socket_handle_t conn;
    std::string sendStr = "";
    if (inChar == "y") {
        conn = HAL::UDP_open("127.0.0.1", 8182, 8183);
        sendStr = "I'MA FIRIN MA LAZAR!";
    } else {
        conn = HAL::UDP_open("127.0.0.1", 8183, 8182);
        sendStr = "WHY DO YOU CUM!";
    }

    static uint8_t buff[100000] = { 0 };
    uint32_t len = 100000;
    uint32_t outIP;
    uint16_t outPort;
    //HAL::UDP_send_packet(conn, (const uint8_t*)sendStr.c_str(), (uint16_t)sendStr.size());
    while (true) {
        while (true) {
            len = 100000;
            HAL::UDP_get_packet(conn, buff, len, outIP, outPort);
            if (len == 0) break;
            std::cout << "received: " << (const char*)buff << std::endl;
            std::cout << "len: " << len << std::endl;
        }
        HAL::UDP_send_packet(conn, (const uint8_t*)sendStr.c_str(), (uint16_t)sendStr.size());
        Sleep(1000);
    }
    //HAL::UDP_get_packet(conn, buff, len, outIP, outPort);
    //if (len != 0) {
    //    std::cout << "received: " << (const char*)buff << std::endl;
    //    std::cout << "len: " << len << std::endl;
    //    len = 100000;
    //}
    HAL::UDP_close(conn);
}

void HAL::app_start() {
    //netTest();

    OPTICK_START_CAPTURE();
    OPTICK_THREAD("MainThread");
    initSystems();
    physics.setRecording(true);
}
bool FPressedLastTick = true;
void HAL::app_loop() {
    OPTICK_THREAD("MainThread");
    OPTICK_EVENT();
    HAL::draw_background(0, 0, 0, 255);
    ecs.runSystems();

#ifdef REWIND_ENABLED
    static int ticksSinceRewind = 0;
    if (HAL::get_key_state('F')) {
        if (FPressedLastTick == false && ticksSinceRewind >= 90) {
            //physics.rewind(60);
            //ecs.rewind(60);
            voxelWorld.rewind(60);
            ticksSinceRewind = 0;
        }
        FPressedLastTick = true;
    }
    else
        FPressedLastTick = false;
    ticksSinceRewind++;
#endif
}
void HAL::app_post_frame() {
    OPTICK_THREAD("MainThread");
    OPTICK_EVENT();
    while (HAL::is_thread_pool_finished(threadPool) == false)
        continue;
    u16 threadCount = static_cast<u16>(HAL::get_thread_pool_free_thread_count(threadPool));
    clock_t remainingTime = static_cast<clock_t>(HAL::get_app_loop_time_MS() + HAL::get_draw_time_MS());
    i32 waitFor = 16 - remainingTime;
    if (waitFor <= 4)
        return;
    for (u32 i = 0; i < threadCount; i++)
        HAL::submit_thread_pool_task(threadPool, sleepForThreaded, to_void_ptr(waitFor-2));
    sleepForThreaded(to_void_ptr(waitFor - 2));
    while (HAL::is_thread_pool_finished(threadPool) == false)
        continue;
}
void HAL::app_end() {
    HAL::release_thread_pool(threadPool);

    OPTICK_STOP_CAPTURE();
    OPTICK_SAVE_CAPTURE("profiler_dump");
    OPTICK_SHUTDOWN();
}