#ifndef UTILITY_H
#define UTILITY_H

#include <GL/glfw3.h>
#include <glm/glm.hpp>

/***
* This is currently just a spot to put global helper functions. These should probably be moved into classes at some point.
***/

extern glm::vec3 viewVec;
extern unsigned int SCR_HEIGHT;
extern unsigned int SCR_WIDTH;

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//pan slower when zoomed in further
	float panSpeed = abs(viewVec.z)/* / 4*/;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		viewVec.y -= 0.01 * panSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		viewVec.y += 0.01 * panSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		viewVec.x += 0.01 * panSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		viewVec.x -= 0.01 * panSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		if (viewVec.z >= -20.0f)
			viewVec.z -= 0.00125;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		if (viewVec.z <= -0.01)
			viewVec.z += 0.00125;
}

double lerp(double a, double b, double f) {
	return (double)(a * (1.0f - f)) + (b *f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	SCR_HEIGHT = height;
	SCR_WIDTH = width;
}

void scroll_callback(GLFWwindow*, double xoffset, double yoffset) {
	double yZoom = yoffset;

	if (yZoom >= 0.5f)
		yZoom = 0.5f;
	if (yZoom <= -0.5f)
		yZoom = -0.5f;

	float zoomAmt = abs(viewVec.z) / 5;

	viewVec.z += yZoom * zoomAmt;

	if (viewVec.z <= -20.0f)
		viewVec.z = -20.0f;
	if (viewVec.z >= -0.01f)
		viewVec.z = -0.01f;
}

#endif // !UTILITY_H

