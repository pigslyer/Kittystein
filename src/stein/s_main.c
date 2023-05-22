#include "s_internal.h"

GLFWwindow* mainWindow = NULL;

void stein_run(void)
{
	stein_init_window();

	while (stein_loop_should_run())
	{
		stein_loop();
	}
}

void stein_init_window(void)
{
	if (!glfwInit())
	{
		exit(1);
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, DEFAULT_WINDOW_NAME, null, null);

	if (!mainWindow)
	{
		exit(1);
	}
}

bool stein_loop_should_run(void)
{
	return glfwWindowShouldClose(mainWindow);
}

void stein_loop(void)
{
	
}


