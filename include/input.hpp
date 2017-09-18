#pragma once
#define MAX_KEYS 1024
#define MAX_BUTTONS 32
#define GLEW_STATIC
#include <GL/glew.h>
#include "glfw3.h"
#include <string>
#include <iostream>

class Input
{
	private:
		static bool     _prevKeys[MAX_KEYS];
		static bool     _keys[MAX_KEYS];
		static float    _keyTime[MAX_KEYS];
		static bool     _mouse[MAX_BUTTONS];
		static double   _mouseX;
		static double   _mouseY;
	public:
		static bool     (*mouseCallback2)(int, int, int);
		static bool     (*keyCallback2)(int, int, int, int);
		static bool     (*cursorPositionCallback2)(int, int);
		static bool     isPoll;
		
		Input();
		Input(const Input & rhs);
		Input&  operator=(const Input & rhs);
		~Input() = default;

		static void     keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void     mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void     cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
		static void     updateKeys();
		static bool     getKeyPressOnce(int key);
		bool            getKeyStatus(int key) const;
		bool            getMouseButtonStatus(int button) const;
		double          getMousePosX() const;
		double          getMousePosY() const;
};
