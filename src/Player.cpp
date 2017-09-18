#include <Player.hpp>

Player::Player(std::pair<int, int> xy, int lives, std::string name) : Character(xy, lives), _fileLocation("resources/profiles/" + name + ".profile"), _name(name), _level(1), _experience(0), _numberOfBombs(0) {
	std::cout << "Player with name " << this->_name << ", XY & Lives Constructed\n";
}

Player::Player(std::pair<int, int> xy, std::string name) : Character(xy), _fileLocation("resources/profiles/" + name + ".profile"), _name(name), _level(1), _experience(0), _numberOfBombs(0) {
	std::cout << "Player with name " << this->_name << "& XY Constructed\n";
}

Player::Player(int lives, std::string name) : Character(lives), _fileLocation("resources/profiles/" + name + ".profile"), _name(name), _level(1), _experience(0), _numberOfBombs(0) {
	std::cout << "Player with name " << this->_name << " & Lives Constructed\n";
}

Player::Player(std::string name) : Character(), _fileLocation("resources/profiles/" + name + ".profile"), _name(name), _level(1), _experience(0), _numberOfBombs(0) {
	std::cout << "Player with name " << this->_name << " Constructed\n";
}

Player::Player() : Character(), _level(1), _experience(0), _numberOfBombs(0) {
	std::cout << "Player with no name Default Constructed\n";
}

Player::Player(Player const & src) {
	if (this->_name.size() > 0)
		std::cout << "Player with name " << this->_name << " Copy-Constructed\n";
	else
		std::cout << "Default Player Copy-Constructed\n";
	*this = src;
}

Player::~Player() {
	std::cout << "Player with name " << this->_name << " De-Constructed\n";
}

Player &	Player::operator=(Player const & src) {
	Character::operator=(src);
	this->_fileLocation = src.getFileLocation();
	this->_name = src.getName();
	this->_level = src.getLevel();
	this->_experience = src.getExperience();
	this->_numberOfBombs = src.getNumberOfBombs();
	return (*this);
}

std::string	Player::getFileLocation() const {
	return (this->_fileLocation);
}

void		Player::setFileLocation(const std::string newLocation) {
	this->_fileLocation = newLocation;
}


std::string	Player::getName() const {
	return (this->_name);
}

void		Player::setName(const std::string newName) {
	this->_name = newName;
}

int 		Player::getLevel() const {
	return (this->_level);
}

void		Player::setLevel(const int newLevel) {
	this->_level = newLevel;
}

int 		Player::getExperience() const {
	return (this->_experience);
}

void		Player::setExperience(const int newExp) {
	this->_experience = newExp;
}

int			Player::getNumberOfBombs() const {
	return (this->_numberOfBombs);
}

void		Player::setNumberOfBombs(const int bombs) {
	this->_numberOfBombs = bombs;
}

std::ostream & 			operator<<(std::ostream & o, Player const & rhs) {
	std::pair<int, int>	xy = rhs.getXY();
	o << "Dumping Player State\nX: " << xy.first <<"\tY: " << xy.second <<
	"\nSpeed: " << rhs.getSpeed() <<
	"\nHealth: " << rhs.getHealth() <<
	"\nLives: " << rhs.getLives() <<
	"\nName: " << rhs.getName() <<
	"\nLevel: " << rhs.getLevel() <<
	"\nExperience: " << rhs.getExperience() <<
	"\nNumber of Bombs: " << rhs.getNumberOfBombs() <<
	"\nFile Location: " << rhs.getFileLocation() << std::endl;
	return o;
}
