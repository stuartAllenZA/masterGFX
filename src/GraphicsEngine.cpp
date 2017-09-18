#include <GraphicsEngine.hpp>
/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
*/
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


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
	//_shader = new Shader("GraphicsEngine/shaders/basic.vert", "GraphicsEngine/shaders/basic.frag");	
	_shader = new Shader2("GraphicsEngine/shaders/7.3.camera.vs", "GraphicsEngine/shaders/7.3.camera,fs");	
//	_camera = new Camera(glm::vec3(0, 0, 3));
	glm::mat4       projectionMatrix;
	projectionMatrix = glm::perspective(glm::radians(60.0f), 1920.0f / 1280.0f, 0.1f, 1000.0f);
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
	//_models["bomberman"] = new Model("resources/models/bomberman.gltf");
	_models["block"] = new Model("resources/models/block1.gltf");
}

bool	GraphicsEngine::processInput() {
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return true;
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	return false;
	// updates
}

void	GraphicsEngine::render() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		_shader->use();

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		_shader->setMat4("projection", projection);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		auto search = _models.find("block");
		if(search != _models.end()) {
			std::cout << "Found " << search->first << " " << search->second << std::endl;
			glm::mat4 model = search->second->mat;
			_shader->setMat4("view", view);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
			_shader->setMat4("model", model);
			search->second->render(search->second->mat);
		}
		else {
			std::cout << "Not found\n";
		}




/*	auto search = _models.find("bomberman");
	if(search != _models.end()) {
		std::cout << "Found " << search->first << " " << search->second << std::endl;
		search->second->render(search->second->mat);
	}*/
	glfwSwapBuffers(_window);
	glfwPollEvents();
}
/*
bool	 processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return true;

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	return false;
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
*/
