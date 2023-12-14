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
#include <unordered_set>
#include <new>
#include <new.h>

#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

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
#include <memory_resource>

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
#include "GL_Utils/TexturedQuad2.h"
#include "GL_Utils/Mesh.h"
#include "GL_Utils/InstancedMesh2.h"
#include "GL_Utils/SceneCamera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <fox/stacktree.hpp>

// GLOBAL STATES
// Temporarily used unique_ptr to patch a memory leak
static glm::mat4 perspective;
static std::vector<std::unique_ptr<SceneCamera>> sceneCameras;
static SceneCamera* activeSceneCamera = nullptr;
static GLFWwindow *window;
//static std::vector<TexturedQuad> textures;
constexpr uint32_t max_textures = 512;
//FlatFlaggedBuffer<TexturedQuad, max_textures> textures;
static std::vector<std::unique_ptr<Mesh>> meshes = {};
static std::vector<std::unique_ptr<InstancedMesh>> instancedMeshes = {};

// A test for systems that live longer that this object
// Static intialization order is a mess...
bool log_to_file_context_alive = false; 
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
        log_to_file_context_alive = true;
    }

    void log_message(const std::string& msg)
    {
        if (!log_to_file_context_alive)
            return;

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
        log_to_file_context_alive = false;
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
    TexturedQuad* texture = new TexturedQuad();
    texture->init(static_cast<std::string>(fileName).c_str(), 0, 0, 20, 20);
    return (HAL::texture_handle_t)(uintptr_t)texture;
}

void HAL::draw_texture(HAL::texture_handle_t texture, int32_t x, int32_t y, uint32_t w, uint32_t h)
{
    TexturedQuad* t = (TexturedQuad*)texture;
    t->draw();
}

void HAL::release_texture(texture_handle_t texture)
{
    //free((void*)texture);
    delete (TexturedQuad*)texture;
}

void HAL::draw_background(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    GL_CALL(glClearColor(static_cast<float>(r) / 256.f, static_cast<float>(g) / 256.f, static_cast<float>(b) / 256.f,
                        static_cast<float>(a) / 256.f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void HAL::draw_text(const char* str, int x, int y, unsigned int fontWidth) {

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



uint32_t get_ip_from_string(const std::string_view str) {
    if (str.size() > 15) {
        std::string errStr = "Error: get_ip_from_string()'s input str character length is > 15, len: ";
        errStr += str.size();
        HAL::hal_error(errStr);
        return 0;
    }
    uint8_t segments[4];
    uint32_t segCount = 0;
    std::string segStr = "";
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '.') {
            uint32_t result = stoi(segStr);
            segments[segCount] = result;
            segCount++;
            segStr = "";
        }
        if (str[i] < '0' || str[i] > '9') {
            std::string errStr = "Error: get_ip_from_string()'s input str has non valid character\n";
            errStr += "Pos: "; errStr += i; errStr += '\n';
            errStr += "Val: "; errStr += str[i];
            HAL::hal_error(errStr);
            return 0;
        }
    }
    uint32_t retValue;
    memcpy(&retValue, &segments[0], sizeof(uint32_t));
    return retValue;
}



#include "CustomSockets.h"



size_t HAL::get_hardware_thread_count()
{
    return std::thread::hardware_concurrency();
}

HAL::thread_pool_handle_t HAL::get_new_thread_pool(size_t maxThreadCount)
{
    CustomThreadPool* tp = new CustomThreadPool();
    tp->init(maxThreadCount);
    return static_cast<HAL::thread_pool_handle_t>(std::bit_cast<std::uintptr_t>(static_cast<void *>(tp)));
}
void HAL::release_thread_pool(HAL::thread_pool_handle_t self)
{
    CustomThreadPool* tp = std::bit_cast<CustomThreadPool*>(std::to_underlying(self));
    tp->destruct();
    delete tp;
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

const char* HAL::get_mesh_name(mesh_handle_t mesh) {
    const auto meshPtr = reinterpret_cast<Mesh*>(mesh);
    if (mesh == invalid_mesh_handle) {
        HAL_WARN("get_mesh_name()'s mesh input == invalid_mesh_handle\n");
        return nullptr;
    }
    return meshPtr->filepath.c_str();
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

void HAL::set_instanced_mesh_submesh_texture(instanced_mesh_handle_t mesh, uint8_t submeshIndex,
                                             const char *textureType,
                                             const char *path)
{
    const auto imesh = reinterpret_cast<InstancedMesh*>(mesh);

    if (mesh == HAL::invalid_instanced_mesh_handle)
        return;

    if (std::to_underlying(mesh) == std::numeric_limits<std::uintptr_t>::max()) // Assert?
        return;

    imesh->subMeshes[submeshIndex].textures[0].swapIfUnique(path);
}
void HAL::set_instanced_mesh_submesh_texture(instanced_mesh_handle_t mesh, uint8_t submeshIndex,
                                        const char *textureType,
                                        texture_handle_t texture)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(mesh);

    if (mesh == HAL::invalid_instanced_mesh_handle) {
        HAL_WARN("HAL::set_instanced_mesh_submesh_texture()'s input mesh is invalid!");
        return;
    }
    if (imesh->subMeshes[submeshIndex].textures[0].getPath().empty())
        imesh->subMeshes[submeshIndex].textures[0].destruct();  //TODO: rework this...
    TexturedQuad* t = (TexturedQuad*)texture;
    imesh->subMeshes[submeshIndex].textures[0] = t->texture;
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

void HAL::set_instanced_mesh_positions(HAL::instanced_mesh_handle_t mesh, std::span<const glm::vec3> _posBuffer)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(mesh);

    if (mesh == HAL::invalid_instanced_mesh_handle) {
        HAL_WARN("set_instanced_mesh_positions()'s mesh input == invalid_instanced_mesh_handle\n");
        return;
    }

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

void HAL::set_instanced_mesh_scale(HAL::instanced_mesh_handle_t mesh, glm::vec3 scale)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(mesh);

    if (mesh == HAL::invalid_instanced_mesh_handle) {
        HAL_WARN("set_instanced_mesh_scale()'s mesh input == invalid_instanced_mesh_handle\n");
        return;
    }

    imesh->setScale(scale.x, scale.y, scale.z);
}
const char* HAL::get_instanced_mesh_name(HAL::instanced_mesh_handle_t mesh) {
    const auto imesh = reinterpret_cast<InstancedMesh*>(mesh);
    if (mesh == HAL::invalid_instanced_mesh_handle) {
        HAL_WARN("get_instanced_mesh_name()'s mesh input == invalid_instanced_mesh_handle\n");
        return nullptr;
    }
    return imesh->filepath.c_str();
}
void HAL::release_instanced_mesh(HAL::instanced_mesh_handle_t mesh)
{
    const auto imesh = reinterpret_cast<InstancedMesh*>(mesh);

    auto r = std::ranges::find_if(instancedMeshes, [imesh](const auto& mesh) { return mesh.get() == imesh; });
    HAL_ASSERT(r != std::end(instancedMeshes), "Invalid mesh handle.");
    r->release();
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

void HAL::use_camera(HAL::camera_handle_t self)
{
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

    //SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

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

    HWND hwnd = glfwGetWin32Window(window);
    //Initialize sound context
    // cs_ctx = cs_make_context(hwnd, 48000, 8192 * 10, 0, NULL);
    _network_init();


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

        //if (show_demo)
        //    ImGui::ShowDemoWindow(&show_demo);

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

#ifdef ALLOCATION_TRACKING

/*
 *  C++ only requires us to implement:
 *  https://en.cppreference.com/w/cpp/memory/new/operator_new
 *  void* operator new  ( std::size_t count );
 *  void* operator new  ( std::size_t count, std::align_val_t al );
 *
 *  https://en.cppreference.com/w/cpp/memory/new/operator_delete
 *  void operator delete  ( void* ptr ) throw();
 *  void operator delete  ( void* ptr, std::align_val_t al ) noexcept;
 *
 *  For data-colleciton reasons we will also replace the array-throwing functions
 */

struct non_tracking_memory_resource : public std::pmr::memory_resource
{
public:
    virtual void* do_allocate(size_t bytes, size_t align)
    {
#ifdef _MSC_VER
        void* const memory = _aligned_malloc(bytes, align);
#else
#error "Use allocation function that doesn't invoke NEW/DELETE"
#endif	    
        return memory;
    }
    virtual void do_deallocate(void* ptr, size_t bytes, size_t align)
    {
#ifdef _MSC_VER
        _aligned_free(ptr);
#else
#error "Use deallocation function that doesn't invoke NEW/DELETE"
#endif
    }

    virtual bool do_is_equal(const memory_resource& other) const noexcept
    {
        return true;
    }
};

bool allocation_tracker_alive = false;
struct allocation_tracker
{
    non_tracking_memory_resource non_tracking_memory_resource;
    std::pmr::unsynchronized_pool_resource memory_pool{ std::addressof(non_tracking_memory_resource) };
    std::pmr::polymorphic_allocator<void> allocator{ std::addressof(memory_pool) };
	
#define ENABLE_ALLOC_STACK_TRACE

    // Caching the names should significantly reduce the memory footprint
#ifdef ENABLE_ALLOC_STACK_TRACE
    std::pmr::unordered_map<std::size_t, std::pmr::stacktrace> stack_trace_storage{ allocator }; 

    std::size_t register_stack_trace(std::size_t skip)
    {
        auto stack_trace = std::pmr::stacktrace::current(skip + 1, allocator);
        auto hash = std::hash<decltype(stack_trace)>{}(stack_trace);
        stack_trace_storage.insert_or_assign(hash, std::move(stack_trace));
        return hash;
    }

    const std::pmr::stacktrace& get_stack_trace(std::size_t hash)
    {
        return stack_trace_storage.at(hash);
    }
#endif

    struct allocation_info
    {
        size_t allocation_idx;
        void* pointer;
        bool new_array;
        std::size_t size;
        std::size_t alignment;
#ifdef ENABLE_ALLOC_STACK_TRACE
        std::size_t alloc_st_hash = {};
        std::size_t dealloc_st_hash = {};
#endif

    };

    mutable std::mutex mx;
    std::pmr::unordered_map<void*, allocation_info> active_allocations{ allocator };
    std::pmr::vector<allocation_info> allocation_history{ allocator };

    void* allocate(size_t size, size_t alignment, bool is_array)
    {
    	if (size == 0)
            return nullptr;

#ifdef _MSC_VER
        void* const memory = _aligned_malloc(size, alignment);
#else
        void* const memory = std::aligned_alloc()
#endif

    	if (!allocation_tracker_alive)
    		return memory;

        if (memory == nullptr) // Allocating 0-size array is valid and might return a nullptr
        {
            // Shouldn't throw because nothrow versions exist
            HAL_ASSERT_REL(size == 0, "Failed to allocate memory\n."); // TODO: Log stack-trace
            return nullptr;
        }

        std::unique_lock lck(mx);

        allocation_history.emplace_back(
            allocation_info{
                .allocation_idx = std::size(allocation_history),
                .pointer = memory,
                .new_array = is_array,
                .size = size,
                .alignment = alignment
#ifdef ENABLE_ALLOC_STACK_TRACE
                ,
                .alloc_st_hash = register_stack_trace(2) // TODO: Also skip new/delete
#endif
            });

        active_allocations.emplace(memory, allocation_history.back());

        return memory;
    }

    void deallocate(void* ptr, bool is_array)
    {
        if (ptr == nullptr) // Deallocating nullptr is valid
            return;

#ifdef _MSC_VER
        _aligned_free(ptr);
#else
        void* const memory = std::free(ptr);
#endif

        if (!allocation_tracker_alive)
            return;

        std::unique_lock lck(mx);

        const auto r = active_allocations.find(ptr);

        // Check if we own the memory
        if(r == std::end(active_allocations))
        {
            HAL_ASSERT_REL(false, "Trying to deallocate memory address we don't own!");
        }

        // Check if the memory was deallocated with the same new/delete kind
		if(r->second.new_array == false && is_array == true)
		{
            HAL_ASSERT_REL(false, "new/delete[] mismatch\n. Memory was allocated using operator new() and deallcoated using operator delete[]()");
		}
        else if(r->second.new_array == true && is_array == false)
        {
            HAL_ASSERT_REL(false, "new[]/delete mismatch\n. Memory was allocated using operator new[]() and deallcoated using operator delete()");
        }

#ifdef ENABLE_ALLOC_STACK_TRACE
        allocation_history[r->second.allocation_idx].dealloc_st_hash = register_stack_trace(2);
#endif

        active_allocations.erase(r);
    }

    allocation_tracker()
    {
        allocation_tracker_alive = true;
        _set_new_mode(false);
    }

    struct stacktree_allocation_info
    {
        std::size_t num_allocations = {};
        std::size_t size_allocation = {};
    };

    friend std::ostream& operator<<(std::ostream& os, const stacktree_allocation_info& v)
    {
        return os << v.num_allocations << " / " << v.size_allocation << " Bytes";
    }

    ~allocation_tracker() noexcept
    {
        std::unique_lock lck(mx);
        allocation_tracker_alive = false;

        HAL_LOG("TOTAL MEMORY ALLOCATIONS: {}\n",
            std::size(this->allocation_history));

        if (std::empty(active_allocations))
            return;

        for(const auto& a : active_allocations | std::views::values)
        {
            HAL_ERROR("MEMORY LEAK:\nAddress: {}\nSize: {}\nAlignment: {}\nAllocated at:\n",
                a.pointer, a.size, a.alignment);

            using namespace std::string_literals;

#ifdef ENABLE_ALLOC_STACK_TRACE
            auto stack = get_stack_trace(a.alloc_st_hash);

            for (const auto& se : stack)
                HAL_ERROR("{} ({}) {}\n",
                    se.source_file(),
                    se.source_line(),
                    se.description()
                );
#endif
        }

        // Generate stacktree
        HAL_LOG("BUILING STACKTREE OF ALLOCATIONS...\n");
        fox::stacktree<stacktree_allocation_info> stacktree;
    	for (const auto& a : allocation_history)
        {
            auto st = this->stack_trace_storage.at(a.alloc_st_hash);
            stacktree.insert(st, [&](auto& v) { v.value().num_allocations += 1; v.value().size_allocation += a.size; });
        }

        const auto stacktree_dump = fox::to_string(stacktree);

        {
            FILE* stacktree_log;
            fopen_s(&stacktree_log, "logs/alloc_stacktree.txt", "w+");
            fprintf(stacktree_log, "%s", stacktree_dump.c_str());
            fclose(stacktree_log);
        }

        HAL_LOG("==\n\n{}\n\n==\n", stacktree_dump);
    }
};

allocation_tracker& get_allocation_tracker()
{
    static allocation_tracker instance;
    return instance;
}

[[nodiscard]] void* operator new(std::size_t n) noexcept(false)
{
    return get_allocation_tracker().allocate(n, alignof(std::max_align_t), false);
}

[[nodiscard]] void* operator new(std::size_t n, std::align_val_t al) noexcept(false)
{
    return get_allocation_tracker().allocate(n, std::to_underlying(al), false);
}

[[nodiscard]] void* operator new[](std::size_t n) noexcept(false)
{
    return get_allocation_tracker().allocate(n, alignof(std::max_align_t), true);
}

[[nodiscard]] void* operator new[](std::size_t n, std::align_val_t al) noexcept(false)
{
    return get_allocation_tracker().allocate(n, std::to_underlying(al), true);
}

void operator delete (void* ptr) 
{
    return get_allocation_tracker().deallocate(ptr, false);
}

void operator delete (void* ptr, std::align_val_t) 
{
    return get_allocation_tracker().deallocate(ptr, false);
}

void operator delete[] (void* ptr) 
{
    return get_allocation_tracker().deallocate(ptr, true);
}

void operator delete[] (void* ptr, std::align_val_t) 
{
    return get_allocation_tracker().deallocate(ptr, true);
}

#endif

