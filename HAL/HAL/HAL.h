#pragma once

/*
Screen space is in screenspace coordinates (+x,+y).  Relative to window dimensions.
Example: if winWidth and winHeight == 400;  200, 200 is the cnter of the window.
In the 3D extension of this API, +Z is away from the camera, -Z is towards it.

For now keyboard values are 1:1 with the ascii table.  Ref: https://www.asciitable.com/
*/

#include <string_view>
#include <stacktrace>
#include <span>
#include <cstdint>
#include <format>

#include <glm/glm.hpp>

const char *EE_getDirData();

namespace HAL
{
    // Define these in your application to be called by the HAL_ implimentations.
    void app_start();
    void app_loop();
    void app_post_frame();
    void app_end();

    template<class... Args>
	std::string hal_format(std::string_view format, Args... args)
    {
        return std::vformat(format, std::make_format_args(args...));
    }

    void hal_log(const std::string& msg);
    void hal_warn(const std::string &msg);
    void hal_error(const std::string &msg);

	void hal_assert(bool cond, const std::string &msg);
}

#define HAL_LOG(FORMAT, ...) HAL::hal_log(HAL::hal_format(FORMAT, __VA_ARGS__ ));
#define HAL_WARN(FORMAT, ...) HAL::hal_warn(HAL::hal_format(FORMAT, __VA_ARGS__ ));
#define HAL_ERROR(FORMAT, ...) HAL::hal_error(HAL::hal_format(FORMAT, __VA_ARGS__ ));

#ifdef NDEBUG
#define HAL_ASSERT(COND, FORMAT, ...) ;
#else
#define HAL_ASSERT(COND, FORMAT, ...) HAL_ASSERT_REL(COND, FORMAT, __VA_ARGS__);
#endif
#define HAL_ASSERT_REL(COND, FORMAT, ...)																				\
    {																													\
        if (!(COND))																									\
		{																												\
            HAL::hal_assert(COND, HAL::hal_format(FORMAT, __VA_ARGS__));												\
		    __debugbreak();																								\
		}                                                                                                               \
    };
	



namespace HAL
{
    enum class resource_handle_t : std::uintptr_t;

    using thread_pool_handle_t = resource_handle_t;
	static constexpr thread_pool_handle_t invalid_thread_pool_handle =
        static_cast<thread_pool_handle_t>(std::numeric_limits<std::uintptr_t>::max());

    using texture_handle_t = resource_handle_t;;
	static constexpr texture_handle_t invalid_texture_handle =
        static_cast<texture_handle_t>(std::numeric_limits<std::uintptr_t>::max());

    using mesh_handle_t = resource_handle_t;;
	static constexpr mesh_handle_t invalid_mesh_handle =
        static_cast<mesh_handle_t>(std::numeric_limits<std::uintptr_t>::max());

    using instanced_mesh_handle_t = resource_handle_t;;
	static constexpr instanced_mesh_handle_t invalid_instanced_mesh_handle =
        static_cast<instanced_mesh_handle_t>(std::numeric_limits<std::uintptr_t>::max());

    using camera_handle_t = resource_handle_t;;
	static constexpr camera_handle_t invalid_camera_handle =
        static_cast<camera_handle_t>(std::numeric_limits<std::uintptr_t>::max());

	//TODO: might not be needed...
	using udp_socket_handle_t = resource_handle_t;
	static constexpr udp_socket_handle_t invalid_udp_socket_handle =
		static_cast<udp_socket_handle_t>(std::numeric_limits<std::uintptr_t>::max());

    /**
     * \brief Load new texture from the file and return handle to it
     * \param fileName Texture path relative to mounted paths
     * \return Returns handle to new texture or returns handle to error texture if texture loading failed
     */
    texture_handle_t get_new_texture(const std::string_view fileName);

    /**
     * \brief 
     * \param texture 
     * \param x 
     * \param y 
     * \param w 
     * \param h 
     */
    void draw_texture(texture_handle_t texture, int32_t x, int32_t y, uint32_t w, uint32_t h);

	void release_texture(texture_handle_t texture);

	void draw_background(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	void draw_text(const char* str, int x, int y, unsigned int fontWidth);

	enum class key_code_t : int32_t
    {
        key_unknown = -1,

		// alphanumeric keys - correspond to ascii
		key_space = 32,
		key_apostrophe = 39, /* ' */
		key_comma = 44, /* , */
		key_minus = 45, /* - */
		key_period = 46, /* . */
		key_slash = 47, /* / */
		key_0 = 48,
		key_1 = 49,
		key_2 = 50,
		key_3 = 51,
		key_4 = 52,
		key_5 = 53,
		key_6 = 54,
		key_7 = 55,
		key_8 = 56,
		key_9 = 57,
		key_semicolon = 59, /* ; */
		key_equal = 61, /* = */
		key_a = 65,
		key_b = 66,
		key_c = 67,
		key_d = 68,
		key_e = 69,
		key_f = 70,
		key_g = 71,
		key_h = 72,
		key_i = 73,
		key_j = 74,
		key_k = 75,
		key_l = 76,
		key_m = 77,
		key_n = 78,
		key_o = 79,
		key_p = 80,
		key_q = 81,
		key_r = 82,
		key_s = 83,
		key_t = 84,
		key_u = 85,
		key_v = 86,
		key_w = 87,
		key_x = 88,
		key_y = 89,
		key_z = 90,
		key_left_bracket = 91, /* [ */
		key_backslash = 92, /* \ */
		key_right_bracket = 93, /* ] */
		key_grave_accent = 96, /* ` */
		key_world_1 = 161, /* non-us #1 */
		key_world_2 = 162, /* non-us #2 * ion keys */

		// special keys
		key_escape = 256,
		key_enter = 257,
		key_tab = 258,
		key_backspace = 259,
		key_insert = 260,
		key_delete = 261,
		key_right = 262,
		key_left = 263,
		key_down = 264,
		key_up = 265,
		key_page_up = 266,
		key_page_down = 267,
		key_home = 268,
		key_end = 269,
		key_caps_lock = 280,
		key_scroll_lock = 281,
		key_num_lock = 282,
		key_print_screen = 283,
		key_pause = 284,
		key_f1 = 290,
		key_f2 = 291,
		key_f3 = 292,
		key_f4 = 293,
		key_f5 = 294,
		key_f6 = 295,
		key_f7 = 296,
		key_f8 = 297,
		key_f9 = 298,
		key_f10 = 299,
		key_f11 = 300,
		key_f12 = 301,
		key_f13 = 302,
		key_f14 = 303,
		key_f15 = 304,
		key_f16 = 305,
		key_f17 = 306,
		key_f18 = 307,
		key_f19 = 308,
		key_f20 = 309,
		key_f21 = 310,
		key_f22 = 311,
		key_f23 = 312,
		key_f24 = 313,
		key_f25 = 314,
		key_kp_0 = 320,
		key_kp_1 = 321,
		key_kp_2 = 322,
		key_kp_3 = 323,
		key_kp_4 = 324,
		key_kp_5 = 325,
		key_kp_6 = 326,
		key_kp_7 = 327,
		key_kp_8 = 328,
		key_kp_9 = 329,
		key_kp_decimal = 330,
		key_kp_divide = 331,
		key_kp_multiply = 332,
		key_kp_subtract = 333,
		key_kp_add = 334,
		key_kp_enter = 335,
		key_kp_equal = 336,
		key_left_shift = 340,
		key_left_control = 341,
		key_left_alt = 342,
		key_left_super = 343,
		key_right_shift = 344,
		key_right_control = 345,
		key_right_alt = 346,
		key_right_super = 347,
		key_menu = 348,
		num_keys = 349
    };

	bool get_key_state(char k);
    bool get_key_state(key_code_t key_code);

	bool get_mouse_left_state();
    bool get_mouse_middle_state();
    bool get_mouse_right_state();

	glm::ivec2 get_mouse_canvas_pos();
	void set_mouse_canvas_pos(glm::ivec2 pos);

	glm::uvec2 get_canvas_size();
	void set_canvas_size(glm::uvec2 size);

	// const char* EE_getDirData();

	float get_FPS();
	void limit_FPS(uint32_t fps);
	float get_app_loop_time_MS();
	float get_draw_time_MS();
	//float get_frame_time_MS();  TODO: need a better name for this or maybe have it removed?

	// bool EE_playAudioFile(const char* fileName, uint8_t loop);
	// bool EE_getNewAudio(const char* fileName, void* audioBuffer);
	// bool EE_playAudio(void* audio, uint8_t loop);
	// bool EE_stopAudio(void* audio);

	// bool EE_sendPacketUDP(void* packet);
	// struct EE_PacketUDP* EE_recvPacketUDP();

	constexpr uint32_t maxPacketLen = 256 * 256;

	//open with 127.0.0.1 ip address to set as server.
	udp_socket_handle_t UDP_open(const char* ip, uint16_t inPort, uint16_t outPort);
	void UDP_close(udp_socket_handle_t soc);
	void UDP_send_packet(udp_socket_handle_t soc, const uint8_t* data, uint16_t len,
		const char* ip, uint16_t port = 0);
	void UDP_get_packet(udp_socket_handle_t soc, uint8_t* outData, uint32_t& outLen,
		uint32_t& outIP, uint16_t& outPort);

	size_t get_hardware_thread_count();

	thread_pool_handle_t get_new_thread_pool(size_t maxThreadCount);
    void release_thread_pool(thread_pool_handle_t self);
    size_t get_thread_pool_free_thread_count(thread_pool_handle_t self);
    bool submit_thread_pool_task(thread_pool_handle_t self, void (*func)(void *), void *param);
    bool is_thread_pool_finished(thread_pool_handle_t self);

	//Automatically defined and called in the HAL_ implimmentation if needed.
	void EE_init();

	mesh_handle_t get_new_mesh(const std::string_view filepath);
    void draw_mesh(mesh_handle_t mesh);
    void set_mesh_submesh_texture(mesh_handle_t mesh, uint8_t submeshIndex, const char *textureType, const char *path);
    void set_mesh_position(mesh_handle_t mesh, glm::vec3 pos);
    void set_mesh_rotation(mesh_handle_t mesh, glm::vec3 angles);
    void set_mesh_scale(mesh_handle_t mesh, glm::vec3 scale);
    void release_mesh(mesh_handle_t mesh);

    instanced_mesh_handle_t get_new_instanced_mesh(const std::string_view filePath);
    void set_instanced_mesh_submesh_texture(instanced_mesh_handle_t meshID, uint8_t submeshIndex,
                                            const char *textureType, const char *path);
    void set_instanced_mesh_submesh_texture(instanced_mesh_handle_t meshID, uint8_t submeshIndex,
                                            const char *textureType, texture_handle_t texture);

    void draw_instanced_mesh(instanced_mesh_handle_t meshID);

    void set_instanced_mesh_positions(instanced_mesh_handle_t meshID, std::span<const glm::vec3> posBuffer);

    void set_instanced_mesh_scale(instanced_mesh_handle_t meshID, glm::vec3 scale);

    void release_instanced_mesh(instanced_mesh_handle_t meshID);

    camera_handle_t get_new_camera();
    void camera_look_at(camera_handle_t self, glm::vec3 at);
    void set_camera_rotation(camera_handle_t, glm::vec3 angles);
    void set_camera_position(camera_handle_t self, glm::vec3 pos);
    void use_camera(camera_handle_t self);
    void release_camera(camera_handle_t self);
}
