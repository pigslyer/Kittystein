#include <delight.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STEIN_INTERNAL

extern GLFWwindow* mainWindow;

void stein_init_window(void);

bool stein_loop_should_run(void);
void stein_loop(void);

// -------------------------------------------------------
// CONSTANTS
extern const uint DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT;
extern const char* const DEFAULT_WINDOW_NAME;

#include <stein.h>