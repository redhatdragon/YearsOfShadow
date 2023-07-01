# <a id="reference"></a> Reference
### Check out [simple example](#simple-example) to get started.

## User/App defined functions
[void app_start()](#app-start)\
[void app_loop()](#app-loop)\
[void app_post_frame()](#app-post-frame)\
[void app_end()](#app-end)

## 2D GFX functions
[texture_handle_t get_new_texture(const std::string_view fileName)](#get-new-texture)\
[void draw_texture(texture_handle_t texture, int32_t x, int32_t y, uint32_t w, uint32_t h)](#draw-texture)\
[void release_texture(texture_handle_t texture)](#release-texture)\
\
[void draw_background(uint8_t r, uint8_t g, uint8_t b, uint8_t a)](draw-background)\
\
[void draw_text(const char* str, int x, int y, unsigned int fontWidth)](#draw-text)

## IO devices (keyboard, mouse, ect)
[bool get_key_state(char k)](#get-key-state-char)\
[bool get_key_state(key_code_t key_code)](#get-key-state-key-code-t)\
\
[bool get_mouse_left_state()](#get-mouse-left-state)\
[bool get_mouse_middle_state()](#get-mouse-middle-state)\
[bool get_mouse_right_state()](#get-mouse-right-state)\
\
[glm::ivec2 get_mouse_canvas_pos()](#get-mouse-canvas-pos)\
[void set_mouse_canvas_pos(glm::ivec2 pos)](set-mouse-canvas-pos)\

## Utilities
[glm::uvec2 get_canvas_size()](get-canvas-size)\
[void set_canvas_size(glm::uvec2 size)](set-canvas-size)\
\
[loat get_FPS()](get-fps)\
[void limit_FPS(uint32_t fps)](limit-fps)\
[float get_app_loop_time_MS()](get-app-loop-time-ms)\
[float get_draw_time_MS()](get-draw-time-ms)\

## Thread functions
[size_t get_hardware_thread_count()](get-hardware-thread-count)\
[thread_pool_handle_t get_new_thread_pool(size_t maxThreadCount)](get-new-thread-pool)\
[void release_thread_pool(thread_pool_handle_t self)](release-thread-pool)\
DEPRICATION CANDIDATE: [size_t get_thread_pool_free_thread_count(thread_pool_handle_t self)](get-thread-pool-free-thread-count)\
[bool submit_thread_pool_task(thread_pool_handle_t self, void (*func)(void *), void *param)](submit-thread-pool-task)\
[bool is_thread_pool_finished(thread_pool_handle_t self)](is-thread-pool-finished)

## 3D GFX functions



## <a id="simple-example"></a> simple-example
### Example code
```cpp
int x = 0;

HAL::texture_handle_t texture;
void HAL::app_start(){
    // Relative to project and executable location
    texture = HAL::get_new_texture("someTexture.png");
}
void HAL::app_loop(){
    // Draw black background per frame
    HAL::draw_background(0, 0, 0, 0);
    // Animate our texture to move right 1 pixel per frame
    HAL::draw_texture(texture, x, 10, 64, 64);
}

void HAL::app_post_frame(){}  // Optional logic may run after frame buffer is swapped

void HAL::app_end(){
    printf("App is closing, good place to log potential errors...");
}
```

## <a id="app-start"></a> void app_start()
### About:
This is REQUIRED to be defined by the end user (it may be left blank).\
The user defined entry point of our application.\
Use this to prep your application/game before the loop phase.\
### Example Code:
See this [simple example](#simple-example)
### See also:
[void app_loop()](#app-loop)\
[void app_post_frame()](#app-post-frame)\
[void app_end()](#app-end)\
Back to [Reference](#reference)

## <a id="app-loop"></a> void app_loop()
### About:
This is REQUIRED to be defined by the end user (the body may be left blank).\
The user defined loop function, called every frame (gfx buffer swap).  Your gfx and game code should go here.\
### Example Code:
See this [simple example](#simple-example)
### See also:
[void app_start()](#app-loop)\
[void app_post_frame()](#app-post-frame)\
[void app_end()](#app-end)\
Back to [Reference](#reference)

## <a id="app-post-frame"></a> void app_post_frame()
### About:
This is REQUIRED to be defined by the end user (the body may be left blank).\
The user defined 'post frame' function, called 'after' every frame (after gfx buffer swap).\
Could occasionally be useful for logging/debugging purposes, don't put gfx code in here!.\
### Example Code:
See this [simple example](#simple-example)
### See also:
[void app_start()](#app-loop)\
[void app_loop()](#app-loop)\
[void app_end()](#app-end)\
Back to [Reference](#reference)

## <a id="app-end"></a> void app_end()
### About:
This is REQUIRED to be defined by the end user (the body may be left blank).\
The user defined function meant to be ran right before the application fully closes.\
Note that aside the end user clicking the X icon, it's possible to manually signal the application should end.\
This can be done with [void signal_end()](#signal-end)\
### Example Code:
See this [simple example](#simple-example)
### See also:
[void app_start()](#app-loop)\
[void app_loop()](#app-loop)\
[void app_post_frame()](#app-post-frame)\
[void signal_end()](#signal-end)\
Back to [Reference](#reference)

## <a id="signal-end"></a> void signal_exit()
### About:
Call this function to signal to HAL that you wish to exit the application after this post frame ends.\
You may wish to call return in your game loop and manually inform your [void app_post_frame()](#app-post-frame) you wish to avoid extra processing/logic or to perform special logic if necessary.  The choice is yours.\
### Example Code:
```cpp
void HAL::app_loop(){
    if(someCondition){
        HAL::signal_end();
        return;
    }
    //ect...
}
```
### See also:
[void app_end()](#app-end)\
Back to [Reference](#reference)