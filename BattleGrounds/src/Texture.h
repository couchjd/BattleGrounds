#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "3rd Party/stb_image/stb_image.h"

class Texture {
public:
	unsigned int ID = 0;
	int channels = 0;
	int width = 0;
	int height = 0;

	Texture(const char* filename) {
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(false);
		std::cout << "Loading image data..." << std::endl;
		unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
		if (data) {
			std::cout << "Done loading image data. Generating texture..." << std::endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			std::cout << "Done generating texture. Generating mipmaps..." << std::endl;
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "Done generating mipmaps." << std::endl;
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
};

#endif
