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

	int xRes = 2/* background.width / 100 */;
	int yRes = 2/* background.height / 100 */;

	float vertices1[] = {
		// positions            // texture coords
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,  // top left 
		1.0f,  1.0f, 0.0f,		1.0f, 1.0f, // top right
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, // bottom left
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f, // bottom right
	};

	unsigned int indices1[] = {
		3, 0, 1, // first triangle
		1, 2, 3  // second triangle
	};

	int verticesSize = sizeof(float) * xRes * yRes * 5;
	std::cout << "Size of vertices: " << verticesSize << std::endl;
	std::cout << "Size of vertices1: " << sizeof(vertices1) << std::endl;
	
	float *vertices = (float*)malloc(verticesSize);
	for (int x = 0; x < verticesSize; x++) {
		vertices[x] = 0.0f;
	}

	for (int y = 0; y < yRes; y++) {
		for (int x = 0; x < xRes * 5; x += 5) {

			int index = (x + (y * ( xRes - 1) * 5));
			vertices[index]		= (float)(((2 * (x / 5)) / (xRes - 1)) - 1.0f);	// x-coordinate
			vertices[index + 1] = (float)(((2 * y) / (yRes - 1)) - 1.0f);			// y-coordinate
			vertices[index + 2] = (float)0.0f;										// z-coordinate
			vertices[index + 3] = (float)((x / 5) / (xRes - 1));					// texture u-coordinate
			vertices[index + 4] = (float)(y / (yRes - 1));							// texture v-coordinate
		}
	}
	for (int x = 0; x < verticesSize; x++) {
		std::cout << "Vertex[" << x << "]: " << (float)vertices[x] << std::endl;
	}

	int indicesSize = 6 * sizeof(unsigned int) * (xRes - 1) * (yRes - 1);
	std::cout << "Size of indices: " << indicesSize << std::endl;
	std::cout << "Size of indices1: " << sizeof(indices1) << std::endl;
	unsigned int *indices = (unsigned int*)malloc(indicesSize);
	
	for (int y = 0; y < yRes - 1; ++y) {
		for (int x = 0; x < (xRes - 1) * 6; x += 6) {
			int index = (x + (y * (xRes - 1) * 6));

			std::cout << "indices[" << index << "]: " << index / 6 + y << std::endl;
			std::cout << "indices[" << index + 1 << "]: " << (index / 6) + 1 + y << std::endl;
			std::cout << "indices[" << index + 2 << "]: " << (index / 6) + xRes + 1  + y << std::endl;
			std::cout << "indices[" << index + 3 << "]: " << (index / 6) + xRes + 1 + y << std::endl;
			std::cout << "indices[" << index + 4 << "]: " << (index / 6) + xRes + y << std::endl;
			std::cout << "indices[" << index + 5 << "]: " << index / 6 + y << std::endl;
			std::cout << "------------------------------------------------------" << std::endl;

			indices[index]		= index / 6 + y;
			indices[index + 1]	= (index / 6) + 1 + y;
			indices[index + 2]	= (index / 6) + xRes + 1 + y;
			indices[index + 3]	= (index / 6 + xRes) + 1 + y;
			indices[index + 4]	= (index / 6) + xRes + y;
			indices[index + 5]	= index / 6 + y;
		}
	}

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

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