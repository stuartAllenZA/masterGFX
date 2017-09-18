#include <Core.hpp>

Core::Core() {
	std::cout << "Constructing Core\n";
	this->_width = this->_game.getSettings().getResolutionX();
	this->_height = this->_game.getSettings().getResolutionY();
	this->_menu = new Menu(_width, _height, &_game, &_win);
	std::cout << "Core Constructed\n";
}

Core::Core(Core const & src) {
	std::cout << "Core Copy-Constructed\n";
	*this = src;
}

Core &			Core::operator=(Core const & src) {
	this->_win = src.getWin();
	this->_game = src.getGame();
	this->_width = src.getWidth();
	this->_height = src.getHeight();
	return (*this);
}

Core::~Core() {
	std::cout << "De-Constructing Core\n";
	this->_game.stopMenuMusic();
	std::cout << "closing nanogui screen" << std::endl;
	nanogui::shutdown();
	std::cout << "nanogui screen closed successfully" << std::endl;
	delete this->_menu;
	this->_menu = nullptr;
	delete this->_gfx;
	_gfx = nullptr;
	glfwTerminate();
	std::cout << "Core De-Constructed\n";
}

void			Core::run() {
	std::cout << "initializing" << std::endl;
	init();
	gameLoop();
	std::cout << "Done" << std::endl;
	// Start main menu (set game state, render menu)
	// Simulate selection of 'New Game'
	// Spawn player
	// Spawn u-box
	// Spawn box0 - empty
	// Spawn box1 - enemy
	// Spawn box2 - upgrade
	// Spawn box3 - hatch
	// Move player
	// Place bomb
	// move player
	// detonate bomb
	// destroy box
	// Move player
	// Place bomb
	// move player
	// explode bomb
	// destroy box
	// drop enemy
	// move player
	// move enemy
	// place bomb
	// move player
	// move enemy
	// detonate bomb
	// kill enemy
	// xp up player
	// move player
	// place bomb
	// move player
	// detonate bomb
	// destroy box
	// drop upgrade
	// move player
	// collect upgrade
	// activate upgrade on player
	// move player
	// place bomb
	// move player
	// detonate bomb
	// destroy box
	// drop hatch
	// move player
	// finish demo
}

void			Core::init() {
	this->_game.initSound();
	this->_game.startMenuMusic();
	_win = nullptr;
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
	_win = glfwCreateWindow(_width, _height, "Bomberman", nullptr, nullptr);
	if (_win == nullptr)
		fatalError("GLFW context is shot");
	else {
		int xPos, yPos;
		glfwGetWindowPos(_win, &xPos, &yPos);
		this->_game.setWindowPos(xPos, yPos);
	}
	glfwMakeContextCurrent(_win);
	glfwSwapInterval(1);
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
	}
	glEnable(GL_DEPTH_TEST);
	_gfx = new GraphicsEngine(&_game, &_win);
	_gfx->init();

	std::cout << "glfw window created" << std::endl;
}

void			Core::gameLoop() {
	GameState gs;
	bool loop = true;

	std::cout << "THE ORIGINAL SETTINGS!!!" << std::endl << this->_game.getSettings() << std::endl;

	while (loop == true && !glfwWindowShouldClose(_win)) {
		input();
		gs = this->_game.getGameState();
		switch (gs) {
			case GameState::MENU :
			_menu->menu();
			break;
			case GameState::PLAY :
			initPlay();
			break;
			case GameState::LOAD :
			load();
			break;
			case GameState::SET :
			settings();
			break;
			case GameState::SAVE :
			save();
			break;
			case GameState::EXIT :
			loop = false;
			break;
		}
		drawGame();
		std::cout << "Main gameLoop looping." << std::endl;
	}
}

void			Core::input() {
	_menu->updateKeys();
	_menu->updateMouse();
}

void			Core::drawGame() {
	glfwGetFramebufferSize(_win, &_width, &_height);

	glViewport(0, 0, _width, _height);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(_win);
	glfwPollEvents();
}

void			Core::fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Press any key to exit" << std::endl;
	int temp;
	std::cin >> temp;
	std::exit(1);
}

void			Core::initPlay() {
	std::cout << "playing, ESC to exit" << std::endl;
	bool exitStatus = false;
	while (!exitStatus) {
		exitStatus = _gfx->processInput();
		_gfx->render();
	}
	this->_game.setGameState(GameState::MENU);
	_menu->setMenuState(MenuState::PAUSE);
}

void			Core::load() {

}

void			Core::settings() {

}

void			Core::save() {

}

Game			Core::getGame() const {
	return (this->_game);
}

void			Core::setGame(const Game newGame) {
	this->_game = newGame;
}

GLFWwindow		*Core::getWin() const {
	return (this->_win);
}

void			Core::setWin(GLFWwindow *win) {
	this->_win = win;
}

int				Core::getWidth() const {
	return (this->_width);
}

void			Core::setWidth(const int newWidth) {
	this->_width = newWidth;
}

int				Core::getHeight() const {
	return (this->_height);
}

void			Core::setHeight(const int newHeight) {
	this->_height = newHeight;
}
