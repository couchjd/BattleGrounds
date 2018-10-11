#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "3rd Party/stb_image/stb_image.h"
#include "Shader.h"
#include "3rd Party/imgui/imgui.h"
#include "3rd Party/imgui/imgui_impl_glfw.h"
#include "3rd Party/imgui/imgui_impl_opengl3.h"

#include "Texture.h"
#include "utility.h"

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
const char* TITLE = "BattleGrounds\0";

glm::vec3 viewVec(0.0f, 0.0f, -3.0f);

void init();
void cleanup();

int main() {
	init();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE, NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewInit();
	glewExperimental = true;

	Shader shader("./res/shaders/vertexShader.glsl", "./res/shaders/fragmentShader.glsl");

	Texture background("./res/textures/faerun_no_tags.jpg");

	//float yVal = ((float)background.height / (float)background.width) - 1.0f;
	int xRes = 5/* background.width / 100 */;
	int yRes = 5/* background.height / 100 */;

	double *vertices = (double*)malloc(sizeof(double)* xRes * yRes * 5);
	for (int y = 0; y < yRes; y++) {
		for (int x = 0; x < xRes * 5; x += 5) {
			/*
			std::cout << "TESTING\n";
			std::cout << (double)((x / 5.0f) / (xRes - 1.0f)) << std::endl;
			std::cout << (double)lerp(-1.0f, 1.0f, (double)(x / 5.0f) / (xRes - 1)) << std::endl;
			*/
			int index = (x + (y * ( xRes - 1) * 5));
			vertices[index]		= (double)(((2 * (x / 5)) / (xRes - 1)) - 1.0f);	// x-coordinate
			vertices[index + 1] = (double)(((2 * y) / (yRes - 1)) - 1.0f);			// y-coordinate
			vertices[index + 2] = (double)0.0f;										// z-coordinate
			vertices[index + 3] = (double)((x / 5) / (xRes - 1));					// texture u-coordinate
			vertices[index + 4] = (double)(y / (yRes - 1));							// texture v-coordinate
		}
	}

	unsigned int *indices = (unsigned int*)malloc(6 * sizeof(unsigned int) * (xRes - 1) * (yRes - 1));
	for (int y = 0; y < yRes; ++y) {
		for (int x = 0; x < (xRes - 1) * 6; x += 6) {
			int base = x * 6 + y * xRes;
			int index = (x + (y * (xRes - 1) * 6));
			//std::cout << "Base: " << base << std::endl;
			//std::cout << "Index: " << index << std::endl;

			std::cout << "indices[" << index << "]: " << index / 6 << std::endl;
			std::cout << "indices[" << index + 1 << "]: " << (index / 6) + 1 << std::endl;
			std::cout << "indices[" << index + 2 << "]: " << (index / 6) + xRes + 1 << std::endl;
			std::cout << "indices[" << index + 3 << "]: " << (index / 6) + xRes + 1 << std::endl;
			std::cout << "indices[" << index + 4 << "]: " << (index / 6) + xRes << std::endl;
			std::cout << "indices[" << index + 5 << "]: " << index / 6 << std::endl;
			std::cout << "------------------------------------------------------" << std::endl;

			indices[index]		= index / 6;
			indices[index + 1]	= (index / 6) + 1;
			indices[index + 2]	= (index / 6) + xRes + 1;
			indices[index + 3]	= (index / 6 + xRes) + 1;
			indices[index + 4]	= (index / 6) + xRes;
			indices[index + 5]	= index / 6;
			/*
			std::cout << "Adding Quad at indices[" << x + y << "]:" << std::endl;
			std::cout << x + y << " " << x + y + 1 << " " << x + y + 6 << std::endl;
			std::cout << x + y + 6 << " " << x + y + 5 << " " << x + y << std::endl;
			*/
		}
	}

	std::cout << "Quad 1:" << std::endl;
	std::cout << indices[0] << " " << indices[1] << " " << indices[2] << std::endl;
	std::cout << indices[3] << " " << indices[4] << " " << indices[5] << std::endl;
	std::cout << "Quad 2:" << std::endl;
	std::cout << indices[6] << " " << indices[7] << " " << indices[8] << std::endl;
	std::cout << indices[9] << " " << indices[10] << " " << indices[11] << std::endl;
	std::cout << "Quad 3:" << std::endl;
	std::cout << indices[12] << " " << indices[13] << " " << indices[14] << std::endl;
	std::cout << indices[15] << " " << indices[16] << " " << indices[17] << std::endl;
	std::cout << "Quad 4:" << std::endl;
	std::cout << indices[18] << " " << indices[19] << " " << indices[20] << std::endl;
	std::cout << indices[21] << " " << indices[22] << " " << indices[23] << std::endl;
	std::cout << "Quad 5:" << std::endl;
	std::cout << indices[24] << " " << indices[25] << " " << indices[26] << std::endl;
	std::cout << indices[27] << " " << indices[28] << " " << indices[29] << std::endl;
	/*
	float vertices[] = {
	-1.0f,	1.0f, 0.0f,		0.0f,	1.0f, //[0]
	-0.5f,  1.0f, 0.0f,		0.25f,  1.0f,
	0.0f,	1.0f, 0.0f,		0.5f,   1.0f,
	0.5f,	1.0f, 0.0f,		0.75f,  1.0f,
	1.0f,	1.0f, 0.0f,		1.0f,	1.0f,
	-1.0f,	0.5f, 0.0f,		0.0f,	0.75f, //[5]
	-0.5f,	0.5f, 0.5f,		0.25f,	0.75f,
	0.0f,	0.5f, 0.5f,		0.5f,   0.75f,
	0.5f,	0.5f, 0.5f,		0.75f,  0.75f,
	1.0f,	0.5f, 0.0f,		1.0f,	0.75f,
	-1.0f,	0.0f, 0.0f,		0.0f,	0.5f, //[10]
	-0.5f,	0.0f, 0.5f,		0.25f,	0.5f,
	0.0f,	0.0f, 0.75f,	0.5f,	0.5f,
	0.5f,	0.0f, 0.5f,		0.75f,	0.5f,
	1.0f,	0.0f, 0.0f,		1.0f,	0.5f,
	-1.0f, -0.5f, 0.0f,		0.0f,	0.25f, //[15]
	-0.5f, -0.5f, 0.5f,		0.25f,	0.25f,
	0.0f, -0.5f, 0.5f,		0.5f,	0.25f,
	0.5f, -0.5f, 0.5f,		0.75f,	0.25f,
	1.0f, -0.5f, 0.0f,		1.0f,	0.25f,
	-1.0f, -1.0f, 0.0f,		0.0f,	0.0f, //[20]
	-0.5f, -1.0f, 0.0f,		0.25f,	0.0f,
	0.0f, -1.0f, 0.0f,		0.5f,	0.0f,
	0.5f, -1.0f, 0.0f,		0.75f,	0.0f,
	1.0f, -1.0f, 0.0f,		1.0f,	0.0f
	};
	unsigned int indices[] = {
	0,  1,  5, //row 1
	5,  1,  6,
	1,  2,  6,
	6,  2,  7,
	2,  3,  7,
	7,  3,  8,
	3,  4,  8,
	8,  4,  9,
	5,  6, 10, //row 2
	10,  6, 11,
	6,  7, 11,
	11,  7, 12,
	7,  8, 12,
	12,	8, 13,
	8,	9, 13,
	13,  9, 14,
	10, 11, 15, //row 3
	15, 11, 16,
	11, 12, 16,
	16, 12, 17,
	12, 13, 17,
	17, 13, 18,
	13, 14, 18,
	18, 14, 19,
	15, 16, 20, //row 4
	20, 16, 21,
	16, 17, 21,
	21, 17, 22,
	17, 18, 22,
	22, 18, 23,
	18, 19, 23,
	23, 19, 24
	};
	*/

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	shader.use();

	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);

	//Need to implement ImGui to toggle this setting.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, background.ID);

		shader.use();

		glm::mat4 view(1);
		glm::mat4 projection(1);
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.00000001f, 100.0f);
		view = glm::translate(view, viewVec);

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		glBindVertexArray(VAO);

		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::fvec3(1.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, (xRes - 1) * (yRes - 1) * 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void cleanup() {

}