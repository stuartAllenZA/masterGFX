#include <Game.hpp>

Game::Game() : _gameState(GameState::MENU), _playState(PlayState::PLAYER_SELECT), _keyPress(0), _keyPressState(false), _settings(Settings()), _hasSave(false) {
	std::cout << "Constructing Game\n";
	if (this->checkPlayers().size() > 0)
		this->_hasSave = true;
	std::cout << "Game Constructed\n";
}

Game::Game(Game const & src) {
	*this = src;
	std::cout << "Game Copy-Constructed\n";
}

Game::~Game() {
	std::cout << "De-Constructing Game\n";
	if (this->_player.getName().size() > 0)
		savePlayer();
	std::cout << "Game De-Constructed\n";
}

Game &					Game::operator=(Game const & src) {
	this->_gameState = src.getGameState();
	this->_playState = src.getPlayState();
	this->_keyPress = src.getKeyPress();
	this->_keyPressState = src.getKeyPressState();
	this->_settings = src.getSettings();
	this->_settings = src.getSettings();
	this->_enemies = src.getEnemies();
	this->_hasSave = src.getHasSave();
	return (*this);
}

GameState				Game::getGameState() const {
	return (this->_gameState);
}

void					Game::setGameState(const GameState newState) {
	this->_gameState = newState;
}

PlayState				Game::getPlayState() const {
	return (this->_playState);
}

void					Game::setPlayState(const PlayState newState) {
	this->_playState = newState;
}

int						Game::getKeyPress() const {
	return (this->_keyPress);
}

void					Game::setKeyPress(const int newInput) {
	this->_keyPress = newInput;
}

bool					Game::getKeyPressState() const {
	return (this->_keyPressState);
}

void					Game::setKeyPressState(const bool newState){
	this->_keyPressState = newState;
}

Settings				Game::getSettings() const {
	return (this->_settings);
}

void					Game::setSettings(const Settings newSettings) {
	this->_settings = newSettings;
}

Player					Game::getPlayer() const {
	return (this->_player);
}

void					Game::setPlayer(const Player newPlayer) {
	this->_player = newPlayer;
	this->_settings.setLastPlayer(newPlayer.getName());
}

Sound				&	Game::getSound() {
	return (this->_sound);
}

void					Game::setSound(Sound newSound) {
	this->_sound = newSound;
}

std::vector<Character*>	Game::getEnemies() const {
	return (this->_enemies);
}

void					Game::setEnemies(const std::vector<Character*> newEnemies) {
	this->_enemies = newEnemies;
}

bool 					Game::getHasSave() const{
	return (this->_hasSave);
}

void 					Game::setHasSave(const bool newHasSave){
	this->_hasSave = newHasSave;
}

void					Game::savePlayer() {
	DIR							*dir;

	if ((dir = opendir ("resources/profiles/")) == NULL) {
		mkdir("resources/profiles/", 0775);
	}
	std::cout << "Saving profile to : " << this->_player.getFileLocation() << std::endl;
	std::ofstream profileFileOut(this->_player.getFileLocation(), std::ofstream::out);
	profileFileOut << "level:" + (std::to_string(this->_player.getLevel()))+"\n";
	profileFileOut << "experience:" + (std::to_string(this->_player.getExperience()))+"\n";
	profileFileOut << "noOfBombs:" + (std::to_string(this->_player.getNumberOfBombs()))+"\n";
	profileFileOut << "resolutionX:" + (std::to_string(this->_settings.getResolutionX()))+"\n";
	profileFileOut << "resolutionY:" + (std::to_string(this->_settings.getResolutionY()))+"\n";
	if (this->_settings.getWindowed())
		profileFileOut << "windowed:true\n";
	else
		profileFileOut << "windowed:false\n";
	profileFileOut << "upKey:" + (std::to_string(this->_settings.getUpKey()))+"\n";
	profileFileOut << "downKey:" + (std::to_string(this->_settings.getDownKey()))+"\n";
	profileFileOut << "leftKey:" + (std::to_string(this->_settings.getLeftKey()))+"\n";
	profileFileOut << "rightKey:" + (std::to_string(this->_settings.getRightKey()))+"\n";
	profileFileOut << "actionKey:" + (std::to_string(this->_settings.getActionKey()))+"\n";
	profileFileOut << "acceptKey:" + (std::to_string(this->_settings.getAcceptKey()))+"\n";
	profileFileOut << "escapeKey:" + (std::to_string(this->_settings.getEscapeKey()))+"\n";
	profileFileOut << "musicVol:" + (std::to_string(this->_settings.getMusicVol()))+"\n";
	profileFileOut << "FXVol:" + (std::to_string(this->_settings.getFXVol()))+"\n";
	profileFileOut.close();
	std::cout << "Profile for " << this->_player.getName() << " saved to: " << this->_player.getFileLocation() << std::endl;
}

std::vector<std::string>		Game::checkPlayers() const {
	DIR							*dir;
	struct dirent				*ent;
	std::vector<std::string>	arr;
	char						*temp;
	int							len;

	std::cout << "Checking for old players.\n";
	if ((dir = opendir ("resources/profiles/")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				temp = strstr(ent->d_name, ".profile");
				if (temp) {
					len = temp - ent->d_name;
					std::string push(ent->d_name, len);
					std::cout << "Adding: " << push << std::endl;
					arr.push_back(push);
				}
			}
		}
		closedir(dir);
	}
	return (arr);
}

void					Game::loadPlayer(const std::string playerName) {
	DIR							*dir;
	struct dirent				*ent;
	char						*temp;
	bool						flag = false;
	int							len = 0;

	if ((dir = opendir ("resources/profiles/")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				temp = strstr(ent->d_name, ".profile");
				len = temp - ent->d_name;
				if (temp) {
					std::string test(ent->d_name, len);
					if (test == playerName) {
						flag = true;
						std::cout << "Loading player: " + playerName + "\n";
						std::string fileName = "resources/profiles/" + playerName + ".profile";
						this->_player.setFileLocation(fileName);
						this->_player.setName(playerName);
						this->_player.setLevel(std::stoi(lexFile(fileName, "level")));
						this->_player.setExperience(std::stoi(lexFile(fileName, "experience")));
						this->_player.setNumberOfBombs(std::stoi(lexFile(fileName, "noOfBombs")));
						int resX = std::stoi(lexFile(fileName, "resolutionX"));
						int resY = std::stoi(lexFile(fileName, "resolutionY"));
						std::pair<int, int> resolution = std::make_pair(resX, resY);
						this->_settings.setResolution(resolution);

						std::string ret = lexFile(fileName, "windowed");
						if (ret == "false")
							this->_settings.setWindowed(false);
						else
							this->_settings.setWindowed(true);

						this->_settings.setUpKey(std::stoi(lexFile(fileName, "upKey")));
						this->_settings.setDownKey(std::stoi(lexFile(fileName, "downKey")));
						this->_settings.setLeftKey(std::stoi(lexFile(fileName, "leftKey")));
						this->_settings.setRightKey(std::stoi(lexFile(fileName, "rightKey")));
						this->_settings.setActionKey(std::stoi(lexFile(fileName, "actionKey")));
						this->_settings.setAcceptKey(std::stoi(lexFile(fileName, "acceptKey")));
						this->_settings.setEscapeKey(std::stoi(lexFile(fileName, "escapeKey")));
						this->_settings.setMusicVol(std::stoi(lexFile(fileName, "musicVol")));
						this->_settings.setFXVol(std::stoi(lexFile(fileName, "FXVol")));
						std::cout << "Post loading player profile!\n" << this->_settings << std::endl;
					}
				}
			}
		}
		if (flag == false)
			throw (Exceptions::LoadPlayerError(playerName));
		closedir(dir);
	}
}

std::string				Game::lexFile(const std::string fileName, const std::string find) {
	std::ifstream handle(fileName);
	std::string line;
	std::string key;
	std::string value;

	if (!handle)
		throw (Exceptions::LexOpenFileError(fileName));
	else
	{
		while (std::getline(handle, key, ':')) {
			std::getline(handle, value);
			if (key == find) {
				//std::cout << "Found: " << value << " for key " << find << std::endl;
				return value;
			}
			//std::cout << "Looking for key: " << find << "\tFound key: " << key << std::endl;
		}
		throw (Exceptions::LexKeyNotFound(find));
	}	
	return ("ERROR");
}

void					Game::setWindowPos(const int xPos, const int yPos) {
	std::cout << "Setting Xpos: " << xPos << " yPos: " << yPos << std::endl;
	this->_settings.setXPos(xPos);
	this->_settings.setYPos(yPos);
}

void					Game::initSound() {
	this->_sound.init();
}

void					Game::startMenuMusic() {
	this->_sound.startMenuMusic();
}

void					Game::stopMenuMusic() {
	this->_sound.stopMenuMusic();
}

void					Game::startGameMusic() {
	this->_sound.startGameMusic();
}

void					Game::stopGameMusic() {
	this->_sound.stopGameMusic();
}

void					Game::processKeyInput() {
	
}

std::ostream & 			operator<<(std::ostream & o, Game const & rhs) {
	int num = 0;
	o << "Dumping Game State" <<
	"\nGame State: " << static_cast<std::underlying_type<GameState>::type>(rhs.getGameState()) <<
	"\nPlay State: " << static_cast<std::underlying_type<PlayState>::type>(rhs.getPlayState()) <<
	"\nKey Press: " << rhs.getKeyPress() << std::endl <<
	"\nKey Press State: " << rhs.getKeyPressState() << std::endl <<
	"\nHas Save: " << std::boolalpha << rhs.getHasSave() <<
	"\nSettings: " << rhs.getSettings() << std::endl <<
	"\nPlayer: " << rhs.getPlayer();
	if (rhs.getEnemies().size() > 0) {
		for (std::vector<Character*>::iterator it = rhs.getEnemies().begin(); it != rhs.getEnemies().end(); ++it) {
			num++;
			o << "Enemy " << num << ": " << *it << std::endl;
		}
	}
	else
		o << "Enemies: 0\n";
	o << rhs.getSettings() << std::endl;
	return o;
}

/*
void				Game::up() {
	// update coords x++;
}

void				Game::updateGameData() {
	switch (_gameInput) {
		case UP:
		case DOWN:
		case LEFT:
		case RIGHT:
		case SPACE:
		case ESC:
	}
	// detectCollisions
	// AIBehaveUpdate
	// detectCollisions
}

// map has to have an odd number of x's and y's
// i.e. maxX and maxY must be even numbers (starting at 0)
void				Game::mapGenerator(int xMax, int yMax) {
	for (int xmin = 0; xmin < xmax; xmin++) {
		for (int ymin = 0; ymin < ymax; ymin++) {
			if (xmin != 0 && ymin != 0 && xmin != xmax && ymin != ymax) {
				if (xmin % 2 == 0 && ymin % 2 == 0) {
					unbreakableWallTemp	unbreakableWall(xmin, ymin);
					_unbreakableWalls.push_back(*unbreakableWall);
				}
			}
			else if (xmin == 0 || xmin == xmax || ymin == 0 || ymin == ymax) {
				unbreakableWallTemp	unbreakableWall(xmin, ymin);
				_unbreakableWalls.push_back(*unbreakableWall);
			}
		}
	}
}
*/
