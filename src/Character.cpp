#include <Character.hpp>

Character::Character(std::pair<int, int> xy, int speed, int health, int lives) : _xy(xy), _speed(speed), _health(health), _lives(lives) {
	std::cout << "Character with XY, Speed, Health & Lives Constructed\n";
}

Character::Character(std::pair<int, int> xy, int speed, int health) : _xy(xy), _speed(speed), _health(health), _lives(1) {
	std::cout << "Character with XY, Speed & Health Constructed\n";
}

Character::Character(std::pair<int, int> xy, int lives) : _xy(xy), _speed(100), _health(100), _lives(lives) {
	std::cout << "Character with XY & Lives Constructed\n";
}

Character::Character(std::pair<int, int> xy) : _xy(xy), _speed(100), _health(100), _lives(1) {
	std::cout << "Character with XY Constructed\n";
}

Character::Character(int lives) : _xy(std::make_pair(0, 0)), _speed(100), _health(100), _lives(lives) {
	std::cout << "Character with Lives Constructed\n";
}

Character::Character() : _xy(std::make_pair(0, 0)), _speed(100), _health(100), _lives(1) {
	std::cout << "Character Default Constructed\n";
}

Character::Character(Character const & src) {
	std::cout << "Character Copy-Constructed\n";
	*this = src;
}

Character::~Character() {
	std::cout << "Character De-Constructed\n";
}

Character &			Character::operator=(Character const & src) {
	this->_xy = src.getXY();
	this->_speed = src.getSpeed();
	this->_health = src.getHealth();
	this->_lives = src.getLives();
	return (*this);
}

std::pair<int, int>	Character::getXY() const {
	return (this->_xy);
}

void				Character::setXY(const std::pair<int, int> xy) {
	this->_xy = xy;
}

int 				Character::getSpeed() const {
	return (this->_speed);
}

void				Character::setSpeed(const int newSpeed) {
	this->_speed = newSpeed;
}

int 				Character::getHealth() const {
	return (this->_health);
}

void				Character::setHealth(const int newHealth) {
	this->_health = newHealth;
}

int 				Character::getLives() const {
	return (this->_lives);
}

void				Character::setLives(const int newLives) {
	this->_lives = newLives;
}