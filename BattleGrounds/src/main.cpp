#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "3rd Party/stb_image/stb_image.h"
#include "shader_s.h"
#include "3rd Party/imgui/imgui.h"
#include "3rd Party/imgui/imgui_impl_glfw.h"
#include "3rd Party/imgui/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow*, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
const char* TITLE = "BattleGrounds\0";

glm::vec3 viewVec(0.0f, 0.0f, -3.0f);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, TITLE, NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewInit();
	glewExperimental = true;

	while (!glfwWindowShouldClose(window)) {

	}

	return 0;
}