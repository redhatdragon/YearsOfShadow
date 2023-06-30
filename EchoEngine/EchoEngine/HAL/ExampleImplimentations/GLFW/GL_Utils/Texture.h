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

	void setIfUnique(const std::string& _path) {
		if (path == _path)
			return;
		init(_path);
	}
};



void Texture::init(const std::string& _path) {
	data = nullptr;
	path = _path;
	stbi_set_flip_vertically_on_load(1);
	data = stbi_load(_path.c_str(), &w, &h, &bpp, 4);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	if (!data) {
		std::cout << "Error: Texture::init(_path) texture from path " << _path <<  " failed to load!" << stbi_failure_reason() << std::endl;
		return;
	}
	stbi_image_free(data);
}
void Texture::destruct() {
	glDeleteTextures(1, &id);
}
inline void Texture::bind(uint32_t slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}
inline void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}