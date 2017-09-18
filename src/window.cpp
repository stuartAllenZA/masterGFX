#include <window.hpp>

float   Window::startTime;
float   Window::frameStartTime;
float   Window::frameChangeTime;

void Window::getError(char *ref)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "OpenGl error code : " << error << " in " << ref << std::endl;
}

Window::Window(const char *title, int width, int height) : _title(title), _width(width), _height(height)
{
	if (!createGlfwWindow())
		glfwTerminate();
}

Window::Window(GLFWwindow *window, const char *title, int width, int height)
{
	initWindow(window, title, width, height);
}

Window::Window(const Window &rhs) { *this = rhs; }

Window& Window::operator=(const Window &rhs)
{
	if (this != &rhs)
	{
		_title = rhs._title;
		_height = rhs._height;
		_width = rhs._width;
		_window = rhs._window;
	}
	return *this;
}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

GLFWwindow & Window::getWindow() const { return *_window; }
int Window::getHeight() const { return _height; }
int Window::getWidth() const { return _width; }

bool Window::initWindow(const char *title, int width, int height)
{
	_title = title;
	_width = width;
	_height = height;
	if (!createGlfwWindow())
	{
		glfwTerminate();
		return false;
	}
	return true;
}

bool Window::initWindow(GLFWwindow *window, const char *title, int width, int height)
{
	_title = title;
	_width = width;
	_height = height;
	_window = window;
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW!" << std::endl;
		return false;
	}
	glViewport(0, 0, _width, _height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	enableVsync();
	glfwSetKeyCallback(_window, _input.keyCallback);
	glfwSetMouseButtonCallback(_window, _input.mouseButtonCallback);
	glfwSetCursorPosCallback(_window, _input.cursorPositionCallback);
	return true;
}

bool Window::createGlfwWindow()
{
	if (!(bool)glfwInit())
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
	glfwWindowHint(GLFW_SAMPLES, 4);
	if (!(bool)(_window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr)))
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(_window);
	glewExperimental = GL_TRUE;
	//glfwSetWindowSizeCallback(_Window, window_resize);
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW!" << std::endl;
		return false;
	}
	glViewport(0, 0, _width, _height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	enableVsync();
	glfwSetKeyCallback(_window, _input.keyCallback);
	glfwSetMouseButtonCallback(_window, _input.mouseButtonCallback);
	glfwSetCursorPosCallback(_window, _input.cursorPositionCallback);
	return true;
}

void Window::enableVsync() { glfwSwapInterval(0); }
void Window::disableVsync() { glfwSwapInterval(1); }

void Window::updateWindow()
{
	Window::getError((char *)"window update");
	glfwPollEvents();
	glfwSwapBuffers(_window);
}

void Window::clearWindow(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::shouldClose() const
{
	return (bool)glfwWindowShouldClose(_window);
}

bool Window::isKeyPressed(int key) const { return _input.getKeyStatus(key); }
bool Window::isMouseButtonPressed(int button) const { return _input.getMouseButtonStatus(button); }
void Window::getMousePos(double &x, double &y) const
{
	x = _input.getMousePosX();
	y = _input.getMousePosY();
}
