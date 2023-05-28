#pragma once
#pragma warning(push, 0)

#define GLEW_STATIC
#pragma comment(lib, "opengl32.lib")

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#define GLFW_EXPOSE_NATIVE_GLX
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "../../../DArray.h"
#include "../../../FlatBuffer.h"
#include "CustomWindowsThreadPool.h"

#include "../../HAL_3D.h"

//#pragma comment(lib, "dsound.lib")
//#pragma comment(lib, "dxguid.lib")  //TODO: check which of these are required for cs_
//#pragma comment(lib, "winmm.lib")
#define CUTE_SOUND_IMPLEMENTATION
#include "cute_sound.h"

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

//Camera camera;
glm::mat4 perspective;
std::vector<glm::mat4> viewMatrices;
std::vector<SceneCamera*> sceneCameras;
SceneCamera* activeSceneCamera = nullptr;

static void clearErrors() {
    while (glGetError() != GL_NO_ERROR);
}
static void getErrors() {
    while (GLenum error = glGetError() != GL_NO_ERROR) {
        std::cout << "Opengl_Error: " << error << std::endl;
    }
}
inline void translateScreen2DToGL(float& x, float& y) {
    EE_Point2Df out;
    unsigned int width, height;
    EE_getCanvasSize(&width, &height);
    out = { x / width, (y / height) * -1 };
    out.x += -1.0f; out.y += 1.0f;
    x = out.x; y = out.y;
}
//WARNING: DEPRICATED
inline void translateScreen3DToGL(float& x, float& y, float& z) {
    /*EE_Point3Df out;
    unsigned int width, height;
    EE_getCanvasSize(&width, &height);
    out = { x / width, (y / height) * -1, (z / ((width + height) / 2)) * -1 };
    out.x += -1.0f; out.y += 1.0f;
    x = out.x; y = out.y; z = out.z;*/

    //throw;

    EE_Point3Df out;
    unsigned int width, height;
    EE_getCanvasSize(&width, &height);
    unsigned int avg = (width + height) / 2;
    out = { x / avg, y / avg, z / avg };
    out.x += -1.0f; out.y += 1.0f;
    x = out.x; y = -out.y; z = -out.z;
    
    //x = x - 1.0f; y = -y + 1.0f; z = -z + 1.0f;

    //EE_Point3Df out;
    //out = { x, y, z };
    //out.x += -1.0f; out.y += 1.0f;
    //x = out.x; y = -out.y; z = -out.z;
}
inline void getAspectRatio(float& w, float& h) {
    unsigned int width, height;
    EE_getCanvasSize(&width, &height);
    unsigned int avg = (width + height) / 2;
    w = width / avg; h = height / avg;
}



GLFWwindow* window;
//TexturedQuad rectTexturedQuad;
//std::vector<Quad> rectangles;
std::vector<TexturedQuad> textures;
//FlatBuffer<Mesh, 1000000> meshes = {};
std::vector<Mesh*> meshes = {};
std::vector<InstancedMesh*> instancedMeshes = {};
//2D
void EE_drawTexture(void* texture, int x, int y, int w, int h) {
    textures[(uint32_t)texture].draw(x, y, w, h);
}
void EE_recolorTexture(void* texture, uint8_t r, uint8_t g, uint8_t b) {

}
void* EE_getNewTexture(const char* fileName) {
    TexturedQuad texture;
    texture.init(fileName, 0, 0, 0, 0);
    textures.push_back(texture);
    return (void*)(textures.size() - 1);
}
void EE_releaseTexture(void* texture) {

}
void EE_drawBackground(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    glClearColor((float)r/256, (float)g/256, (float)b/256, (float)a/256);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void EE_drawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    //rectTextureQuad.setPosAndSiz((float)x, (float)y, (float)w, (float)h);
    //rectTextureQuad.draw();
    //rectTexturedQuad.draw(x, y, w, h);
}

bool EE_getKeyState(char k) {
    if (k == EE_KEY_ESC)
        return (bool)glfwGetKey(window, GLFW_KEY_ESCAPE);
    if (k == EE_KEY_SPACE)
        return (bool)glfwGetKey(window, GLFW_KEY_SPACE);
    if (k == EE_KEY_TAB)
        return (bool)glfwGetKey(window, GLFW_KEY_TAB);
    return (bool)glfwGetKey(window, k);
}
void EE_getMouseState(uint8_t* leftButton, uint8_t* middleButton, uint8_t* rightButton) {
    int result;
    result = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(leftButton)
        *leftButton = (bool)result;
    result = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if(rightButton)
        *rightButton = (bool)result;
    result = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
    if(middleButton)
        *middleButton = (bool)result;
}
void EE_getMouseCanvasPos(int* x, int* y) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    *x = (int)xpos;
    *y = (int)ypos;
}
void EE_setMouseCanvasPos(int x, int y) {
    glfwSetCursorPos(window, (double)x, (double)y);
}
void EE_setMouseCursorHide(uint8_t isTrue) {

}
void EE_setMouseEnable(uint8_t isTrue) {

}

void EE_getCanvasSize(unsigned int* width, unsigned int* height) {
    glfwGetWindowSize(window, (int*)width, (int*)height);
}
const char* EE_getDirData() {
    return "./data/";
}

uint16_t EE_getHardwareThreadCount() {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return (uint16_t)sysinfo.dwNumberOfProcessors;
}

void* EE_getNewThreadPool(uint16_t maxThreadCount) {
    CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)malloc(sizeof(CustomWindowsThreadPool));
    tp->init(maxThreadCount);
    return tp;
}
void EE_releaseThreadPool(void* self) {
    free(self);
}
uint16_t EE_getThreadPoolFreeThreadCount(void* self) {
    CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)self;
    return tp->getFreeThreadCount();
}
bool EE_sendThreadPoolTask(void* self, void(*func)(void*), void* param) {
    CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)self;
    tp->addTask(func, param);
    return true;
}
bool EE_isThreadPoolFinished(void* self) {
    CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)self;
    return tp->allTasksFinished();
}

cs_context_t* cs_ctx;
cs_playing_sound_t* activeSound[256];
uint32_t activeSoundCount = 0;
bool EE_playAudioFile(const char* fileName, uint8_t loop) {
    cs_loaded_sound_t tAudio = cs_load_wav(fileName);
    cs_loaded_sound_t* audio = (cs_loaded_sound_t*)malloc(sizeof(struct cs_loaded_sound_t));
    memcpy(audio, &tAudio, sizeof(struct cs_loaded_sound_t));
    cs_playing_sound_t tAudioInstance = cs_make_playing_sound(audio);
    if (loop)
        tAudioInstance.looped = true;  //TODO: Test
    cs_playing_sound_t* audioInstance = (cs_playing_sound_t*)malloc(sizeof(cs_playing_sound_t));
    memcpy(audioInstance, &tAudioInstance, sizeof(tAudioInstance));
    activeSound[activeSoundCount] = audioInstance;
    cs_insert_sound(cs_ctx, activeSound[activeSoundCount]);
    activeSoundCount++;
    return true;
}
//bool EE_getNewAudio(const char* fileName, void* audioBuffer);
//bool EE_playAudio(void* audio, uint8_t loop);
//bool EE_stopAudio(void* audio);

//3D
void EE_drawTriangle2DRGB(const EE_Point2Df points[3], EE_ColorR8G8B8 color) {

}
void EE_drawTriangle2DRGBVertex(const EE_Point2Df points[3], const EE_ColorR8G8B8 colors[3]) {

}
void* EE_getNewMesh(const char* filepath) {
    /*uint32_t retValue = meshes.count;
    meshes[retValue].init(filepath);
    meshes.count++;
    return (void*)retValue;*/
    Mesh* retValue = new Mesh();
    retValue->init(filepath);
    meshes.push_back(retValue);
    return (void*)retValue;
}
void EE_drawMesh(void* mesh) {
    Mesh* meshPtr = (Mesh*)mesh;
    //meshes[meshID].draw(viewMatrices.back(), perspective);
    if (activeSceneCamera)
        meshPtr->draw(activeSceneCamera->GetViewMatrix(), perspective);
    else
        meshPtr->draw(viewMatrices.back(), perspective);
    //meshes[meshID].draw(glm::mat4(1), perspective);
}
void EE_setTextureSubmesh(void* mesh, uint8_t submeshIndex, const char* meshType, const char* path) {
    Mesh* meshPtr = (Mesh*)mesh;
    meshPtr->subMeshes[submeshIndex].textures[0].init(path);
}
void EE_setPositionMesh(void* mesh, float x, float y, float z) {
    Mesh* meshPtr = (Mesh*)mesh;
    //translateScreen3DToGL(x, y, z);  //TODO: why was this used?  Is it needed?
    y = -y;
    z = -z;
    //meshPtr->pos = { x/1000, y/1000, z/1000 };
    meshPtr->pos = { x, y, z };
}
void EE_setRotationMesh(void* mesh, float x, float y, float z) {
    Mesh* meshPtr = (Mesh*)mesh;
    meshPtr->rot = { x, y, z };
}
void EE_setScaleMesh(void* mesh, float x, float y, float z) {
    Mesh* meshPtr = (Mesh*)mesh;
    //translateScreen3DToGL(x, y, z);
    meshPtr->siz = { x, y, z };
}
void EE_releaseMesh(void* mesh) {
    Mesh* meshPtr = (Mesh*)mesh;
    meshPtr->destruct();
    delete meshPtr;
}

void* EE_getNewInstancedMesh(const char* filePath) {
    InstancedMesh* imesh = new InstancedMesh();
    imesh->init(filePath);
    if (imesh->subMeshes.size() == 0) {
        std::cout << "Warning: EE_getNewInstancedMesh()'s filepath arg didn't turn up a usable mesh file, path: "
            << filePath << std::endl;
        imesh->destruct();
        delete imesh;
        return (void*)-1;
    }
    return imesh;
}
void EE_drawInstancedMesh(void* meshID) {
    InstancedMesh* imesh = (InstancedMesh*)meshID;
    if (meshID == (void*)-1) return;
    if (activeSceneCamera)
        imesh->draw(activeSceneCamera->GetViewMatrix(), perspective);
    else
        imesh->draw(viewMatrices.back(), perspective);
}
void EE_releaseInstancedMesh(void* meshID) {
    InstancedMesh* imesh = (InstancedMesh*)meshID;
    if (meshID == (void*)-1) return;
    delete imesh;
}
void EE_setInstancedMeshPositions(void* meshID, const EE_Point3Df* _posBuffer, uint32_t count) {
    InstancedMesh* imesh = (InstancedMesh*)meshID;
    if (meshID == (void*)-1) return;
    if (count >= 256 * 256) {
        std::cout << "Error: EE_setInstancedMeshPositions()'s count arg is larger than 256*256-1, count is: "
            << count << std::endl;
        throw;
    }
    EE_Point3Df* posBuffer = (EE_Point3Df*)malloc(sizeof(EE_Point3Df) * count);
    if (posBuffer == nullptr)
        throw;
    memcpy(posBuffer, _posBuffer, sizeof(EE_Point3Df) * count);
    for (uint32_t i = 0; i < count; i++) {
        posBuffer[i].y = -posBuffer[i].y;
        posBuffer[i].z = -posBuffer[i].z;
    }
    imesh->setOffsets((Vec3D<float>*)posBuffer, count);
    free(posBuffer);
}
void EE_setInstancedMeshScale(void* meshID, EE_Point3Df scale) {
    InstancedMesh* imesh = (InstancedMesh*)meshID;
    if (meshID == (void*)-1) return;
    imesh->setScale(scale.x, scale.y, scale.z);
}
void EE_setInstancedSubmeshTexture(void* meshID, uint8_t submeshIndex, const char* textureType, const char* path) {
    InstancedMesh* imesh = (InstancedMesh*)meshID;
    if (meshID == (void*)-1) return;
    imesh->subMeshes[submeshIndex].textures[0].init(path);
}

void EE_translate(float x, float y, float z) {
    glm::mat4& matrix = viewMatrices[viewMatrices.size() - 1];
    translateScreen3DToGL(x, y, z);
    matrix = glm::translate(viewMatrices[viewMatrices.size() - 1], { x, y, z });
}
void EE_rotate(float x, float y, float z) {
    glm::mat4& matrix = viewMatrices[viewMatrices.size() - 1];
    //translateScreen3DToGL(x, y, z);  //TODO: needed?
    //glm::lookAt({}, {}, {});
    
    //glm::vec3 position = {0.0f, 0.0f, 0.0f};
    //glm::vec3 front = {x, y, z};
    //matrix = glm::lookAt(position, position + front, { 0.0f, 1.0f, 0.0f });

    matrix = glm::rotate(matrix, glm::radians(x), { 1.0f, 0.0f, 0.0f });
    matrix = glm::rotate(matrix, glm::radians(y), { 0.0f, 1.0f, 0.0f });
    matrix = glm::rotate(matrix, glm::radians(z), { 0.0f, 0.0f, 1.0f });
}
void EE_scale(float x, float y, float z) {
    glm::mat4& matrix = viewMatrices[viewMatrices.size() - 1];
    translateScreen3DToGL(x, y, z);
    matrix = glm::scale(viewMatrices[viewMatrices.size() - 1], { x, y, z });
}
void EE_pushMatrix() {
    viewMatrices.push_back(glm::mat4(1));
}
void EE_popMatrix() {
    viewMatrices.pop_back();
}

void* EE_getNewCamera() {
    sceneCameras.push_back(new SceneCamera());
    return (void*)sceneCameras[sceneCameras.size() - 1];
}
void EE_cameraLookAt(void* self, float x, float y, float z) {
    SceneCamera* cam = (SceneCamera*)self;
    Vec3D<float> newFront = { x, -y, -z };
    auto camPos = cam->getPosition();
    newFront -= camPos;
    cam->setFront(newFront.x, newFront.y, newFront.z);
}
//TODO: SET THIS TO TRANSLATE SCREENSPACE TO GL  PS: probably remove this later...
//void EE_setCameraRotation(void* self, float x, float y, float z) {
//    SceneCamera* cam = (SceneCamera*)self;
//    cam->setRotation(x, -y, -z);
//}
void EE_setCameraPos(void* self, float x, float y, float z) {
    SceneCamera* cam = (SceneCamera*)self;
    //translateScreen3DToGL(x, y, z);
    //y = -y;
    //z = -z;  //TODO: this isn't working?
    //x = x - 1.0f; y = -y + 1.0f; z = -z + 1.0f;
    //x /= 700; y /= 700; z /= 700;
    cam->setPosition(x, -y, -z);
}
void EE_useCamera(void* self) {
    activeSceneCamera = (SceneCamera*)self;
}
void EE_releaseCamera(void* self) {
    SceneCamera* cam = (SceneCamera*)self;
    uint32_t index = -1;
    uint32_t cameraCount = sceneCameras.size();
    for (uint32_t i = 0; i < cameraCount; i++) {
        if (sceneCameras[i] == cam) {
            index = i;
            break;
        }
    }
    if (index == -1) {

    }
    if (sceneCameras[index] == activeSceneCamera)
        activeSceneCamera = nullptr;
    sceneCameras[index] = sceneCameras[sceneCameras.size() - 1];
    sceneCameras.pop_back();
}



int main() {

    if (::SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS) == FALSE) {
        DWORD dwPriorityErr = ::GetLastError();
        std::cout << "Setting main thread priority to high failed, error: " << dwPriorityErr 
            << std::endl;
    }

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
    cs_ctx = cs_make_context(hwnd, 48000, 8192 * 10, 0, NULL);
    EE_appStart();
    cs_spawn_mix_thread(cs_ctx);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        double startTime = glfwGetTime();

        EE_pushMatrix();

        /* Render here */
        glClearColor(0.5f, 0.5f, 0.5f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        unsigned int winWidth, winHeight;
        EE_getCanvasSize(&winWidth, &winHeight);
        //perspective = glm::perspective(45.0f, (GLfloat)winWidth / (GLfloat)winHeight, 0.00001f, 1500.0f);
        perspective = glm::perspective(45.0f, (GLfloat)winWidth / (GLfloat)winHeight, 1.0f, 15000.0f);

        for (size_t i = 0; i < activeSoundCount; i++) {
            if (activeSound[i]->active == false) {
                cs_free_sound(activeSound[i]->loaded_sound);
                free(activeSound[i]->loaded_sound);
                free(activeSound[i]);
                activeSoundCount--;
                activeSound[i] = activeSound[activeSoundCount];
            }
        }
        EE_appLoop();
        if (activeSoundCount);
            //cs_mix(ctx);
        getErrors();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        viewMatrices.clear();

        while (startTime + (1.0f / 60.0f) > glfwGetTime()) {
            continue;
        }
    }

    EE_appEnd();

    glfwTerminate();
    return 0;
}

#pragma warning(pop)