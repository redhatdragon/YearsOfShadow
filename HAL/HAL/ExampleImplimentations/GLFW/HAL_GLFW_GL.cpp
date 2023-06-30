#pragma once
#pragma warning(push, 0)

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <ranges>
#include <memory>
#include <format>

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

#include <HAL/HAL.h>

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

//GLOBAL STATES
// Temporarily used unique_ptr to patch a memory leak
static glm::mat4 perspective;
static std::vector<glm::mat4> viewMatrices;
static std::vector<std::unique_ptr<SceneCamera>> sceneCameras;
static SceneCamera* activeSceneCamera = nullptr;
static GLFWwindow *window;
static std::vector<TexturedQuad> textures;
static std::vector<std::unique_ptr<Mesh>> meshes = {};
static std::vector<std::unique_ptr<InstancedMesh>> instancedMeshes = {};

void HAL_assert_impl(bool value, const std::string &msg)
{
    using namespace std::string_literals;

    if (value == true) return;

    OutputDebugStringA(("HAL Assertion Failed\n"s + msg + "\nStack Trace:\n").c_str());

    auto stack = std::stacktrace::current();

    for (auto se : stack | std::views::drop(1))
        OutputDebugStringA((se.source_file() + "("s + std::to_string(se.source_line()) + ")" + se.description() + "\n").c_str());

    __debugbreak();
}

#define HAL_ASSERT(STATEMENT, MSG) \
   HAL_assert_impl(STATEMENT, MSG)

template<class... Args>
void HAL_log_impl(std::string_view format, Args&&... args)
{
    using namespace std::string_literals;
    std::string msg = std::vformat(format, std::make_format_args(args...));
    OutputDebugStringA(("HAL: "s + msg + "\n").c_str());
}

#define HAL_LOG(FORMAT, ...) HAL_log_impl( FORMAT , __VA_ARGS__ )

static void clearErrors() {
    while (glGetError() != GL_NO_ERROR);
}
static void getErrors() {
    while (GLenum error = glGetError() != GL_NO_ERROR) {
        std::cout << "Opengl_Error: " << error << std::endl;
    }
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
    textures.push_back(texture);
    return static_cast<HAL::texture_handle_t>(textures.size() - 1);
}

void HAL::draw_texture(HAL::texture_handle_t texture, int32_t x, int32_t y, uint32_t w, uint32_t h)
{
    HAL_ASSERT(std::to_underlying(texture) < textures.size(), "Invalid texture handle.");
    textures[std::to_underlying(texture)].draw(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
}

void HAL::release_texture(texture_handle_t texture)
{
    HAL_ASSERT(std::to_underlying(texture) < textures.size(), "Invalid texture handle.");
    // TODO: Implement textures as a free list buffer
}

void HAL::draw_background(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    glClearColor(static_cast<float>(r)/256.f, static_cast<float>(g)/256.f, static_cast<float>(b)/256.f, static_cast<float>(a)/256.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

float FPS, FPSLimit, lastFPS;
float HAL::get_FPS() { return lastFPS; }

void HAL::limit_FPS(uint32_t fps) { FPSLimit = fps; };

float appLoopTimeMS, drawTimeMS;
float HAL::get_app_loop_time_MS() { return appLoopTimeMS; };

float HAL::get_draw_time_MS() { return drawTimeMS; };

float HAL::get_frame_time_MS() { return appLoopTimeMS + drawTimeMS; };


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
    //meshes[meshID].draw(viewMatrices.back(), perspective);
    if (activeSceneCamera)
        meshPtr->draw(activeSceneCamera->GetViewMatrix(), perspective);
    else
        meshPtr->draw(viewMatrices.back(), perspective);
    //meshes[meshID].draw(glm::mat4(1), perspective);
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

void HAL::set_instanced_mesh_submesh_texture(HAL::instanced_mesh_handle_t meshID, uint8_t submeshIndex,
                                             const char *textureType,
                                             const char *path)
{
    const auto imesh = reinterpret_cast<InstancedMesh*>(meshID);

    if (meshID == HAL::invalid_instanced_mesh_handle)
        return;

    if (std::to_underlying(meshID) == std::numeric_limits<std::uintptr_t>::max()) // Assert?
        return;

    imesh->subMeshes[submeshIndex].textures[0].setIfUnique(path);
}

void HAL::draw_instanced_mesh(HAL::instanced_mesh_handle_t meshID)
{
    const auto imesh = reinterpret_cast<InstancedMesh *>(meshID);

    if (meshID == HAL::invalid_instanced_mesh_handle)
        return;

    if (activeSceneCamera)
        imesh->draw(activeSceneCamera->GetViewMatrix(), perspective);
    else
        imesh->draw(viewMatrices.back(), perspective);
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

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640*2, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        printf("Error: glewInit failed\n");
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);  // Accept fragment if it closer to the camera than the former one
    //glFrontFace(GL_CW);

    //camera = Camera();  //TODO: necessary?

    //Initialize sound context
    HWND hwnd = glfwGetWin32Window(window);
    // cs_ctx = cs_make_context(hwnd, 48000, 8192 * 10, 0, NULL);
    EE_appStart();
    // cs_spawn_mix_thread(cs_ctx);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        double startTime = glfwGetTime();

        FPSLimit = 60;
        lastFPS = FPS;
        FPS = clock();
        appLoopTimeMS = clock();

        // EE_pushMatrix();

        /* Render here */
        glClearColor(0.5f, 0.5f, 0.5f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto winDim = HAL::get_canvas_size();
        //perspective = glm::perspective(45.0f, (GLfloat)winWidth / (GLfloat)winHeight, 0.00001f, 1500.0f);
        perspective = glm::perspective(45.0f, static_cast<GLfloat>(winDim.x) / static_cast<GLfloat>(winDim.y), 1.0f, 15000.0f);

        // for (size_t i = 0; i < activeSoundCount; i++) {
        //     if (activeSound[i]->active == false) {
        //         cs_free_sound(activeSound[i]->loaded_sound);
        //         free(activeSound[i]->loaded_sound);
        //         free(activeSound[i]);
        //         activeSoundCount--;
        //         activeSound[i] = activeSound[activeSoundCount];
        //     }
        // }

        EE_appLoop();
        appLoopTimeMS = clock() - appLoopTimeMS;

        drawTimeMS = clock();
        getErrors();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
        viewMatrices.clear();
        drawTimeMS = clock() - drawTimeMS;

        EE_appPostFrame();

        while (startTime + (1.0f / FPSLimit) > glfwGetTime()) {
            continue;
        }
        FPS = clock() - FPS;
    }

    EE_appEnd();

    glfwTerminate();
    return 0;
}

#pragma warning(pop)