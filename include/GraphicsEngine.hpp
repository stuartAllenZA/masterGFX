#pragma once

#include <GL/glew.h>
#include "glfw3.h"
#include "../GraphicsEngine/glm/glm.hpp"
#include "../GraphicsEngine/glm/gtc/type_ptr.hpp"

#include "camera.hpp"
//#include "Shader.hpp"
#include "shader_m.h"
#include "Model.hpp"
#include "tiny_gltf.h"
#include "stb_image.h"
#include "picojson.h"
#include "util.h"

class GraphicsEngine {
	private:
	Camera							*_camera;
	Shader2							*_shader;
	std::map<std::string, Model*>	_models;
	//Game							*_game;
	GLFWwindow						*_window;

	void	_loadResources();
	void	_loadModels();
	void	_loadModel();

	public:
//		GraphicsEngine(Game	*game, GLFWwindow **window);
		GraphicsEngine();
		GraphicsEngine(GLFWwindow **window);
		GraphicsEngine(GLFWwindow *window);
		~GraphicsEngine();
		void	init();
		void	initSystems();
		bool	processInput();
		void	render();
};