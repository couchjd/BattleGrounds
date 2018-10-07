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

		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

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