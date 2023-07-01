#pragma once
#pragma warning(push, 0)

// #include <fstream>
#include <sstream>
#include <thread>
#include <ranges>
#include <memory>
#include <format>
#include <array>
#include <cstdio>
#include <iomanip>
#include <mutex>
#include <utility>

#define GLEW_STATIC
#pragma comment(lib, "opengl32.lib")

#define NOMINMAX
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//#define GLFW_EXPOSE_NATIVE_GLX
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <unordered_map>
#include "CustomWindowsThreadPool.h"
//#include "CustomCPP11ThreadPool.h"
#include <memory>

#include <HAL/HAL.h>

// TODO: Might need an include dependency in cmake or maybe HAL should be moved back inside EchoEngine?
// Maybe could be avoided with std::vector<T*> and checking for nullptr instead of using FlatFlaggedBuffer?
#include "../../../EchoEngine/EchoEngine/FlatBuffer.h"

//#pragma comment(lib, "dsound.lib")
//#pragma comment(lib, "dxguid.lib")  //TODO: check which of these are required for cs_
//#pragma comment(lib, "winmm.lib")
// #define CUTE_SOUND_IMPLEMENTATION - compiled in cute_sound.cpp
extern "C"
{
	#include "cute_sound.h"
}

void translateScreen2DToGL(float& x, float& y);

#define STB_IMAGE_IMPLEMENTATION

#include <HAL/ExampleImplimentations/GLFW/GL_Utils/GL_Call.h>
#include "GL_Utils/Shader.h"
#include "GL_Utils/Shapes.h"
#include "GL_Utils/VertexBuffer.h"
#include "GL_Utils/IndexBuffer.h"
#include "GL_Utils/VertexArray.h"
#include "GL_Utils/Texture.h"
#include "GL_Utils/TexturedQuad.h"
#include "GL_Utils/Mesh.h"
#include "GL_Utils/InstancedMesh2.h"
#include "GL_Utils/SceneCamera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// GLOBAL STATES
// Temporarily used unique_ptr to patch a memory leak
static glm::mat4 perspective;
static std::vector<std::unique_ptr<SceneCamera>> sceneCameras;
static SceneCamera* activeSceneCamera = nullptr;
static GLFWwindow *window;
//static std::vector<TexturedQuad> textures;
constexpr uint32_t max_textures = 512;
FlatFlaggedBuffer<TexturedQuad, max_textures> textures;
static std::vector<std::unique_ptr<Mesh>> meshes = {};
static std::vector<std::unique_ptr<InstancedMesh>> instancedMeshes = {};

static class log_to_file_context_t
{
    FILE *last_log_;
    FILE *date_log_;
    std::array<char, 1024> output_buffer_ ;
    size_t buffer_pos_;

public:
    log_to_file_context_t()
        : buffer_pos_(0)
    {
        fopen_s(&last_log_ , "logs/last_log.txt", "w+");
        auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        
        fopen_s(&date_log_, ("logs/" + ss.str() + ".txt").c_str(), "w+");
    }

    void log_message(const std::string& msg)
    {
        if (buffer_pos_ + msg.size() > output_buffer_.size())
            flush();

        std::copy(msg.begin(), msg.end(), output_buffer_.begin() + buffer_pos_);
        buffer_pos_ += msg.size();
    }

    void flush()
    {
        if (last_log_)
        {
            fwrite(output_buffer_.data(), 1, buffer_pos_, last_log_);
            fflush(last_log_);
        }
        if (date_log_)
        {
            fwrite(output_buffer_.data(), 1, buffer_pos_, date_log_);
            fflush(date_log_);
        }

        buffer_pos_ = 0;
    }

    ~log_to_file_context_t()
    {
        flush();

        if (last_log_)
            fclose(last_log_);
        if (date_log_)
            fclose(date_log_);
    }
} log_to_file_context;


static bool consoleSupportsColors  = false;
static std::mutex consoleMutex;

void HAL::hal_log(const std::string &msg)
{
    std::unique_lock<std::mutex> lock(consoleMutex);
    OutputDebugStringA(("LOG: " + msg).c_str());
    log_to_file_context.log_message(("LOG: " + msg).c_str());
    if (consoleSupportsColors)
       printf_s("\033[37m%s\033[0m", msg.c_str());
    else 
        printf_s("%s", msg.c_str());
}

void HAL::hal_warn(const std::string &msg)
{
    std::unique_lock<std::mutex> lock(consoleMutex);
    OutputDebugStringA(("WARNING: " + msg).c_str());
    log_to_file_context.log_message(("WARNING: " + msg).c_str());
    if (consoleSupportsColors)
        printf_s("\033[33m%s\033[0m", msg.c_str());
    else
        printf_s("WARNING: %s", msg.c_str());
}

void HAL::hal_error(const std::string &msg)
{
    std::unique_lock<std::mutex> lock(consoleMutex);
    OutputDebugStringA(("ERROR: " + msg).c_str());
    log_to_file_context.log_message(("ERROR: " + msg).c_str());
    if (consoleSupportsColors)
        printf_s("\033[31m%s\033[0m", msg.c_str());
    else
        printf_s("ERROR: %s", msg.c_str());
}

void HAL::hal_assert(bool cond, const std::string &msg)
{
    using namespace std::string_literals;

    if (cond == true)
        return;

    hal_error(("HAL Assertion Failed\n"s + msg + "\nStack Trace:\n").c_str());

    auto stack = std::stacktrace::current();

    for (auto se : stack | std::views::drop(1))
        hal_error(
            (se.source_file() + "("s + std::to_string(se.source_line()) + ")" + se.description() + "\n").c_str());
}



inline void translateScreen2DToGL(float& x, float& y)
{
    glm::vec2 out;
    auto dim = HAL::get_canvas_size();
    out = { x / dim.x, (y / dim.y) * -1 };
    out.x += -1.0f; out.y += 1.0f;
    x = out.x; y = out.y;
}

inline void getAspectRatio(float& w, float& h) {
    auto dim = HAL::get_canvas_size();
    unsigned int avg = (dim.x + dim.y) / 2;
    w = dim.x / avg; h = dim.y / avg;
}

HAL::texture_handle_t HAL::get_new_texture(const std::string_view fileName)
{
    TexturedQuad texture;
    texture.init(static_cast<std::string>(fileName).c_str(), 0, 0, 0, 0);
    return static_cast<HAL::texture_handle_t>(textures.insert(texture));
}

void HAL::draw_texture(HAL::texture_handle_t texture, int32_t x, int32_t y, uint32_t w, uint32_t h)
{
    HAL_ASSERT(std::to_underlying(texture) < max_textures && textures.getIsValid(std::to_underlying(texture)),
               "Invalid texture handle.");
    textures[std::to_underlying(texture)].draw(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
}

void HAL::release_texture(texture_handle_t texture)
{
    HAL_ASSERT(std::to_underlying(texture) < max_textures && textures.getIsValid(std::to_underlying(texture)),
               "Invalid texture handle.");
    // TODO: Implement textures as a free list buffer
    // KOI: ^ attempting this now with the FlatFlaggedBuffer if am understanding free list buffers?
}

void HAL::draw_background(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    GL_CALL(glClearColor(static_cast<float>(r) / 256.f, static_cast<float>(g) / 256.f, static_cast<float>(b) / 256.f,
                        static_cast<float>(a) / 256.f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

bool HAL::get_key_state(char k)
{
    return static_cast<bool>(glfwGetKey(window, k));
}

bool HAL::get_key_state(HAL::key_code_t k)
{
    return static_cast<bool>(glfwGetKey(window, std::to_underlying(k))); // Maps 1 to 1 to glfw keycodes
}

bool HAL::get_mouse_left_state()
{
    return static_cast<bool>(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT));
}

bool HAL::get_mouse_right_state() {
    return static_cast<bool>(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT));
}

bool HAL::get_mouse_middle_state() {
    return static_cast<bool>(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE));
}

glm::ivec2 HAL::get_mouse_canvas_pos()
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { static_cast<int>(xpos), static_cast<int>(ypos) };
}

void HAL::set_mouse_canvas_pos(glm::ivec2 pos)
{
    glfwSetCursorPos(window, static_cast<double>(pos.x), static_cast<double>(pos.y));
}

glm::uvec2 HAL::get_canvas_size()
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return { static_cast<unsigned int>(width), static_cast<unsigned int>(height) };
}

void HAL::set_canvas_size(glm::uvec2 size)
{
    glfwSetWindowSize(window, static_cast<int>(size.x), static_cast<int>(size.y));
}

const char* EE_getDirData() {
    return "./data/";
}

struct telemetry_counters_t
{
    long double FPS, FPSLimit, lastFPS;
    long double appLoopTimeMS, drawTimeMS;

    long double appLoopTimeMSAvg, drawTimeMSAvg, FPSAvg;

    size_t count = {};
    long double appLoopTimeMSCount, drawTimeMSCount, FPSAvgCount;

    uint64_t appTime;
    uint64_t drawTime;
} telemetry_counters;

float HAL::get_FPS() { return telemetry_counters.lastFPS; }

void HAL::limit_FPS(uint32_t fps) { telemetry_counters.FPSLimit = fps; };

float HAL::get_app_loop_time_MS() { return telemetry_counters.appLoopTimeMS; };

float HAL::get_draw_time_MS() { return telemetry_counters.drawTimeMS; };

//float HAL::get_frame_time_MS() { return telemetry_counters.appLoopTimeMS + telemetry_counters.drawTimeMS; };


size_t HAL::get_hardware_thread_count()
{
    return std::thread::hardware_concurrency();
}

HAL::thread_pool_handle_t HAL::get_new_thread_pool(size_t maxThreadCount)
{
    CustomThreadPool *tp = new CustomThreadPool();
    tp->init(maxThreadCount);
    return static_cast<HAL::thread_pool_handle_t>(std::bit_cast<std::uintptr_t>(static_cast<void *>(tp)));
}
void HAL::release_thread_pool(HAL::thread_pool_handle_t self) {
    delete std::bit_cast<CustomThreadPool*>(std::to_underlying(self));
}
size_t HAL::get_thread_pool_free_thread_count(HAL::thread_pool_handle_t self)
{
    const auto tp = reinterpret_cast<CustomThreadPool*>(self);
    return tp->getFreeThreadCount();
}
bool HAL::submit_thread_pool_task(HAL::thread_pool_handle_t self, void (*func)(void *), void *param)
{
    const auto tp = reinterpret_cast<CustomThreadPool *>(self);
    tp->addTask(func, param);
    return true;
}
bool HAL::is_thread_pool_finished(HAL::thread_pool_handle_t self)
{
    const auto tp = reinterpret_cast<CustomThreadPool *>(self);
    return tp->allTasksFinished();
}

// cs_context_t* cs_ctx;
// cs_playing_sound_t* activeSound[256];
// uint32_t activeSoundCount = 0;
// bool EE_playAudioFile(const char* fileName, uint8_t loop)
// {
//     cs_loaded_sound_t tAudio = cs_load_wav(fileName);
//     cs_loaded_sound_t* audio = (cs_loaded_sound_t*)malloc(sizeof(struct cs_loaded_sound_t));
//     memcpy(audio, &tAudio, sizeof(struct cs_loaded_sound_t));
//     cs_playing_sound_t tAudioInstance = cs_make_playing_sound(audio);
//     if (loop)
//         tAudioInstance.looped = true;  //TODO: Test
//     cs_playing_sound_t* audioInstance = (cs_playing_sound_t*)malloc(sizeof(cs_playing_sound_t));
//     memcpy(audioInstance, &tAudioInstance, sizeof(tAudioInstance));
//     activeSound[activeSoundCount] = audioInstance;
//     cs_insert_sound(cs_ctx, activeSound[activeSoundCount]);
//     activeSoundCount++;
//     return true;
// }
//bool EE_getNewAudio(const char* fileName, void* audioBuffer);
//bool EE_playAudio(void* audio, uint8_t loop);
//bool EE_stopAudio(void* audio);


HAL::mesh_handle_t HAL::get_new_mesh(const std::string_view filepath)
{
    // TODO: Use vector
    auto retValue = std::make_unique<Mesh>();
    retValue->init(static_cast<std::string>(filepath).c_str());
    meshes.push_back(std::move(retValue));
    return static_cast<HAL::mesh_handle_t>(reinterpret_cast<std::uintptr_t>(static_cast<void *>(meshes.back().get())));
}

void HAL::draw_mesh(mesh_handle_t mesh)
{
    const auto meshPtr = reinterpret_cast<Mesh*>(mesh);
    if (activeSceneCamera)
        meshPtr->draw(activeSceneCamera->getPosition(),
        activeSceneCamera->getFront(), activeSceneCamera->GetViewMatrix(),
                      perspective);
    else
        meshPtr->draw({}, {0, 0, 1}, {}, perspective);
}

void HAL::set_mesh_submesh_texture(mesh_handle_t mesh, uint8_t submeshIndex, const char *meshType, const char *path)
{
    const auto meshPtr = reinterpret_cast<Mesh *>(mesh);
    meshPtr->subMeshes[submeshIndex].textures[0].init(path);
}

void HAL::set_mesh_position(mesh_handle_t mesh, glm::vec3 pos)
{
    const auto meshPtr = reinterpret_cast<Mesh *>(mesh);
    //translateScreen3DToGL(x, y, z);  //TODO: why was this used?  Is it needed?
    pos.y = -pos.y;
    pos.z = -pos.z;
    //meshPtr->pos = { x/1000, y/1000, z/1000 };
    meshPtr->pos = { pos.x, pos.y, pos.z };
}

void HAL::set_mesh_rotation(mesh_handle_t mesh, glm::vec3 angles)
{
    const auto meshPtr = reinterpret_cast<Mesh *>(mesh);
    meshPtr->rot = { angles.x, angles.y, angles.z };
}

void HAL::set_mesh_scale(mesh_handle_t mesh, glm::vec3 scale)
{
    const auto meshPtr = reinterpret_cast<Mesh *>(mesh);
    //translateScreen3DToGL(x, y, z);
    meshPtr->siz = { scale.x, scale.y, scale.z };
}

void HAL::release_mesh(mesh_handle_t mesh)
{
    const auto meshPtr = reinterpret_cast<Mesh *>(mesh);
    auto r = std::ranges::find_if(meshes, [meshPtr](const auto &mesh) { return mesh.get() == meshPtr; });
    HAL_ASSERT(r != std::end(meshes), "Invalid mesh handle.");
    r->release();
}

HAL::instanced_mesh_handle_t HAL::get_new_instanced_mesh(const std::string_view filePath)
{
    auto imesh = std::make_unique<InstancedMesh>();
    imesh->init(static_cast<std::string>(filePath).c_str()); // TODO: Return false if failed to load

    if (std::empty(imesh->subMeshes)) 
    {
        HAL_LOG("EE_getNewInstancedMesh()'s filepath arg didn't turn up a usable mesh file, path: {}", filePath);
        imesh.reset();
        return HAL::invalid_instanced_mesh_handle;
    }

    instancedMeshes.push_back(std::move(imesh));

    return static_cast<HAL::instanced_mesh_handle_t>(reinterpret_cast<std::uintptr_t>(instancedMeshes.back().get()));
}

void HAL::set_instanced_mesh_submesh_texture(instanced_mesh_handle_t meshID, uint8_t submeshIndex,
                                             const char *textureType,
                                             const char *path)
{
    const auto imesh = reinterpret_cast<InstancedMesh*>(meshID);

    if (meshID == HAL::invalid_instanced_mesh_handle)
        return;

    if (std::to_underlying(meshID) == std::numeric_limits<std::uintptr_t>::max()) // Assert?
        return;

    imesh->subMeshes[submeshIndex].textures[0].swapIfUnique(path);
}
void HAL::set_instanced_mesh_submesh_texture(instanced_mesh_handle_t meshID, uint8_t submeshIndex,
                                        const char *textureType,
                                        texture_handle_t texture)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(meshID);

    if (meshID == HAL::invalid_instanced_mesh_handle)
        return;

    if (std::to_underlying(meshID) == std::numeric_limits<std::uintptr_t>::max()) // Assert?
        return;
    if (imesh->subMeshes[submeshIndex].textures[0].getPath().empty())
        imesh->subMeshes[submeshIndex].textures[0].destruct();  //TODO: rework this...
    imesh->subMeshes[submeshIndex].textures[0] = textures[std::to_underlying(texture)].texture;
}

void HAL::draw_instanced_mesh(HAL::instanced_mesh_handle_t meshID)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(meshID);

    if (meshID == HAL::invalid_instanced_mesh_handle)
        return;

    if (activeSceneCamera)
        imesh->draw(activeSceneCamera->getPosition(), activeSceneCamera->getFront(), activeSceneCamera->GetViewMatrix(),
                    perspective);
    else
        imesh->draw({}, {0, 0, 1}, {}, perspective);
}

void HAL::release_instanced_mesh(HAL::instanced_mesh_handle_t meshID)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(meshID);

    auto r = std::ranges::find_if(instancedMeshes, [imesh](const auto &mesh) { return mesh.get() == imesh; });
    HAL_ASSERT(r != std::end(instancedMeshes), "Invalid mesh handle.");
    r->release();
}

void HAL::set_instanced_mesh_positions(HAL::instanced_mesh_handle_t meshID, std::span<const glm::vec3> _posBuffer)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(meshID);

    if (meshID == HAL::invalid_instanced_mesh_handle)
        return;

    HAL_ASSERT(_posBuffer.size() < 256llu * 256llu,
               "Error: EE_setInstancedMeshPositions()'s count arg is larger than 256*256-1");

    std::vector<glm::vec3> posBuffer(_posBuffer.begin(), _posBuffer.end());

    for (auto &p : posBuffer)
    {
        p.y = -p.y;
        p.z = -p.z;
    }

    imesh->setOffsets(std::data(posBuffer), static_cast<uint32_t>(std::size(posBuffer)));
}

void HAL::set_instanced_mesh_scale(HAL::instanced_mesh_handle_t meshID, glm::vec3 scale)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(meshID);

    if (meshID == HAL::invalid_instanced_mesh_handle)
        return;

    imesh->setScale(scale.x, scale.y, scale.z);
}

HAL::camera_handle_t HAL::get_new_camera()
{
    sceneCameras.push_back(std::make_unique<SceneCamera>());
    return static_cast<HAL::camera_handle_t>(reinterpret_cast<std::uintptr_t>(sceneCameras.back().get()));
}

void HAL::camera_look_at(HAL::camera_handle_t self, glm::vec3 at)
{
    const auto cam = reinterpret_cast<SceneCamera*>(self);
    glm::vec3 newFront = {at.x, -at.y, -at.z};
    auto camPos = cam->getPosition();
    newFront -= camPos;
    cam->setFront(newFront.x, newFront.y, newFront.z);
}

void HAL::set_camera_position(HAL::camera_handle_t self, glm::vec3 pos)
{
    const auto cam = reinterpret_cast<SceneCamera *>(self);
    
    cam->setPosition(pos.x, -pos.y, -pos.z);
}

void HAL::use_camera(HAL::camera_handle_t self) {
    activeSceneCamera = reinterpret_cast<SceneCamera *>(self);
}

void HAL::release_camera(HAL::camera_handle_t self)
{
    const auto cam = reinterpret_cast<SceneCamera *>(self);

    // TODO: Use free-list buffer

    for (uint32_t i = 0; i <= std::size(sceneCameras); i++) {
        HAL_ASSERT(i != std::size(sceneCameras), "Invalid camera handle. Failed to release");

        if (sceneCameras[i].get()  == cam)
        {
            if (sceneCameras[i].get() == activeSceneCamera)
                activeSceneCamera = nullptr;

            sceneCameras[i].reset();

            break;
        }
    }
}

static void show_performance_overlay(bool *p_open)
{
    static int location = 0;
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

    if (ImGui::Begin("Performance", p_open, window_flags))
    {
        ImGui::Text("Performance Statistics");
        ImGui::Separator();
        ImGui::Text("FPS LIMIT: (%.4f)", static_cast<float>(telemetry_counters.FPSLimit));
        ImGui::Text("App Loop Time: (%.4f) [MS]", static_cast<float>(telemetry_counters.appLoopTimeMS));
        ImGui::Text("Draw Time: (%.4f) [MS]", static_cast<float>(telemetry_counters.drawTimeMS));
        ImGui::Text("FPS: (%.4f)", static_cast<float>(telemetry_counters.FPS));
        ImGui::Spacing();
        ImGui::Text("Averages");
        ImGui::Separator();
        ImGui::Text("App Loop Time: (%.4f) [MS]", static_cast<float>(telemetry_counters.appLoopTimeMSAvg));
        ImGui::Text("Draw Time: (%.4f) [MS]", static_cast<float>(telemetry_counters.drawTimeMSAvg));
        ImGui::Text("FPS: (%.4f)", static_cast<float>(telemetry_counters.FPSAvg));
    }
    ImGui::End();
}

int main()
{
    CoInitialize(nullptr);

    // Colorful output!
#ifdef _WIN32
    {
        DWORD dwMode;
        HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleMode(hOutput, &dwMode);
        dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        consoleSupportsColors = (SetConsoleMode(hOutput, dwMode) != 0);    
    }

    //SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
#endif

    HAL_LOG("Hello, LOG!\n");
    HAL_WARN("Hello, WARN!\n");
    HAL_ERROR("Hello, ERROR!\n");

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640*2, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        HAL_ERROR("Error: glewInit failed\n");
    }

    GL_CALL(glEnable(GL_MULTISAMPLE));

    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glEnable(GL_DEPTH_TEST));
    GL_CALL(glDepthFunc(GL_LESS));  // Accept fragment if it closer to the camera than the former one
    //glFrontFace(GL_CW);

    //camera = Camera();  //TODO: necessary?

    //Initialize sound context
    HWND hwnd = glfwGetWin32Window(window);
    // cs_ctx = cs_make_context(hwnd, 48000, 8192 * 10, 0, NULL);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    HAL::app_start();
    // cs_spawn_mix_thread(cs_ctx);

    /* Loop until the user closes the window */

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    long double freq = static_cast<long double>(frequency.QuadPart);
    long double freqMS = freq / 1000.0;

    bool show_demo = true, performance_window = true;
    while (!glfwWindowShouldClose(window)) {
        double startTime = glfwGetTime();  //TODO: replace with below time_start

        LARGE_INTEGER time_start;
        QueryPerformanceCounter(&time_start);
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo)
            ImGui::ShowDemoWindow(&show_demo);

        if (performance_window)
            show_performance_overlay(&performance_window);

        telemetry_counters.FPSLimit = 60;
        telemetry_counters.lastFPS = telemetry_counters.FPS;

        /* Render here */
        GL_CALL(glClearColor(0.5f, 0.5f, 0.5f, 1));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        auto winDim = HAL::get_canvas_size();
        //perspective = glm::perspective(45.0f, (GLfloat)winWidth / (GLfloat)winHeight, 0.00001f, 1500.0f);
        perspective = glm::perspective(45.0f, static_cast<GLfloat>(winDim.x) / static_cast<GLfloat>(winDim.y), 
            0.01f, 15000.0f);

        HAL::app_loop();

        LARGE_INTEGER app_end;
        QueryPerformanceCounter(&app_end);

        telemetry_counters.appTime = app_end.QuadPart - time_start.QuadPart;
        telemetry_counters.appLoopTimeMS = static_cast<long double>(telemetry_counters.appTime) / freqMS;

        /* Swap front and back buffers */
        
        /* Poll for and process events */

        LARGE_INTEGER draw_end;
        QueryPerformanceCounter(&draw_end);
        telemetry_counters.drawTime = draw_end.QuadPart - app_end.QuadPart;
        telemetry_counters.drawTimeMS = static_cast<long double>(telemetry_counters.drawTime) / freqMS;

        HAL::app_post_frame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);

        //TODO: needs a tunup to be more accurate
        while (glfwGetTime() < startTime + (1.0 / telemetry_counters.FPSLimit))
        {
            continue;
        }

        LARGE_INTEGER frame_end;
        QueryPerformanceCounter(&frame_end);

        telemetry_counters.FPS = static_cast<long double>(freq) / static_cast<long double>(frame_end.QuadPart - time_start.QuadPart);

        if (telemetry_counters.count % static_cast<size_t>(telemetry_counters.FPSLimit) == 0 ||
            telemetry_counters.count >= static_cast<size_t>(telemetry_counters.FPSLimit))
        {
            telemetry_counters.FPSAvg =
                telemetry_counters.FPSAvgCount / static_cast<long double>(telemetry_counters.count);

            telemetry_counters.appLoopTimeMSAvg =
                telemetry_counters.appLoopTimeMSCount / static_cast<long double>(telemetry_counters.count);

            telemetry_counters.drawTimeMSAvg =
                telemetry_counters.drawTimeMSCount / static_cast<long double>(telemetry_counters.count);

            telemetry_counters.FPSAvgCount = telemetry_counters.appLoopTimeMSCount =
                telemetry_counters.drawTimeMSCount = 0.0;

            telemetry_counters.count = 0;
        }

        telemetry_counters.count += 1;
        telemetry_counters.FPSAvgCount += telemetry_counters.FPS;
        telemetry_counters.appLoopTimeMSCount += telemetry_counters.appLoopTimeMS;
        telemetry_counters.drawTimeMSCount += telemetry_counters.drawTimeMS;
    }

    HAL::app_end();

    log_to_file_context.flush();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

#pragma warning(pop)