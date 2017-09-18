#include <GraphicsEngine.hpp>

GraphicsEngine::GraphicsEngine() {
	std::cout << "graphics engine constructed\n";
}

/*
GraphicsEngine::GraphicsEngine(Game *game, GLFWwindow **window) : _window(*window), _game(game) {
	std::cout << "graphics engine constructed\n";
}
*/

GraphicsEngine::GraphicsEngine(GLFWwindow **window) : _window(*window) {
	std::cout << "graphics engine constructed\n";
}

GraphicsEngine::GraphicsEngine(GLFWwindow *window) : _window(window) {
	std::cout << "graphics engine constructed\n";
}

GraphicsEngine::~GraphicsEngine() {
	std::cout << "graphics engine destructed\n";
}

// external loader - external from Core
void	GraphicsEngine::initSystems() {
	_window = nullptr;
	std::cout << "creating glfw window" << std::endl;
	glfwInit();
	glfwSetTime(0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	_window = glfwCreateWindow(1920, 1280, "Bomberman", nullptr, nullptr);
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
	}
	glEnable(GL_DEPTH_TEST);
}

void	GraphicsEngine::init() {
	std::cout << "inititializing graphics engine\n";
	_shader = new Shader("GraphicsEngine/shaders/basic.vert", "GraphicsEngine/shaders/basic.frag");	
	_loadResources();
}

void	GraphicsEngine::_loadResources() {
	std::cout << "loading graphics resources\n";
	_loadModels();
}

void	GraphicsEngine::_loadModels() {
	_loadModel();	
}

void	GraphicsEngine::_loadModel() {
	_models["bomberman"] = new Model("resources/models/bomberman.gltf");
}

bool	GraphicsEngine::processInput() {
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return true;
	return false;
	// updates
}

void	GraphicsEngine::render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	auto search = _models.find("bomberman");
	if(search != _models.end()) {
		std::cout << "Found " << search->first << " " << search->second << '\n';
		search->second->render();
	}
	else {
		std::cout << "Not found\n";
	}
	//_models["bomberman"].render();
	// foreach (model)
	// 	call model->render
	glfwSwapBuffers(_window);
	glfwPollEvents();
}
