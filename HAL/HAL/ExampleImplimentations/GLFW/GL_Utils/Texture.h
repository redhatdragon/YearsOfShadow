#pragma once
#include "../stb/stb_image.h"

class Texture {
	uint32_t id;
	std::string path;
	uint8_t* data = nullptr;
	int32_t w, h, bpp;
public:
	void init(const std::string& _path);
	void destruct();
	inline void bind(uint32_t slot = 0);
	inline void unbind();
	inline uint16_t getWidth() const { return w; }
	inline uint16_t getHeight() const { return h; }
	inline uint8_t getBPP() const { return bpp; }
    inline std::string getPath() const { return path; }
	inline const uint8_t* getPixels() const { return data; }

	void swapIfUnique(const std::string& _path) {
		if (path == _path)
			return;
        if (path.empty() == false)
            destruct();
		init(_path);
	}
};



void Texture::init(const std::string& _path) {
	data = nullptr;
	path = {};
	path = _path;
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(_path.c_str(), &w, &h, &bpp, 4);
	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	if (!data) {
		HAL_ERROR("Error: Texture::init(_path) texture from path {} failed to load! {}\n",
			_path, stbi_failure_reason());
		return;
	}
	stbi_image_free(data);
}
void Texture::destruct() {
	glDeleteTextures(1, &id);
}
inline void Texture::bind(uint32_t slot) {
	GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
}
inline void Texture::unbind() {
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}