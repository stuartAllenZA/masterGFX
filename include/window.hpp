#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include "glfw3.h"
#include "input.hpp"
#include <string>
#include <iostream>

class Window
{
	private:
		const char  *_title;
		int         _height;
		int         _width;
		GLFWwindow  *_window;
		Input       _input;
	private:
		bool        createGlfwWindow();
	public:
		static  float   startTime;
		static  float   frameStartTime;
		static  float   frameChangeTime;
	public:
		Window() = default;
		Window(GLFWwindow *window, const char *title, int width, int height);
		Window(const char *title, int width, int height);
		Window(const Window & rhs);
		Window&     operator=(const Window & rhs);
		~Window();

		static void getError(char *ref);

		bool        initWindow(const char *title, int width, int height);
		bool        initWindow(GLFWwindow *window, const char *title, int width, int height);
		GLFWwindow& getWindow() const;
		int         getHeight() const;
		int         getWidth() const;
		void        enableVsync();
		void        disableVsync();
		void        updateWindow();
		void        clearWindow(float r, float g, float b, float a);
		bool        shouldClose() const;
		bool        isKeyPressed(int key) const;
		bool        isMouseButtonPressed(int button) const;
		void        getMousePos(double &x, double &y) const;
};
