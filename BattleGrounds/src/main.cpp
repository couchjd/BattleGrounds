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

	float yVal = ((float)background.height / (float)background.width) - 1.0f;

	float vertices[] = {
		// positions			// texture coords
		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f, // top right
		 1.0f,  yVal, 0.0f,		1.0f, 0.0f, // bottom right
		-1.0f,  yVal, 0.0f,		0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

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
		shader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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