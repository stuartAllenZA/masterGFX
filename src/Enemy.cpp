#include <Enemy.hpp>

Enemy::Enemy(std::pair<int, int> xy, int speed, int health, int lives) : Character(xy, speed, health, lives) {
	std::cout << "Enemy with XY, Speed, Health & Lives Constructed\n";
}

Enemy::Enemy(std::pair<int, int> xy, int speed, int health) : Character(xy, speed, health) {
	std::cout << "Enemy with XY, Speed & Health Constructed\n";
}

Enemy::Enemy(std::pair<int, int> xy, int lives) : Character(xy, lives) {
	std::cout << "Enemy with XY & Lives Constructed\n";
}

Enemy::Enemy(std::pair<int, int> xy) : Character(xy) {
	std::cout << "Enemy with XY Constructed\n";
}

Enemy::Enemy(int lives) : Character(lives) {
	std::cout << "Enemy with Lives Constructed\n";
}

Enemy::Enemy() : Character() {
	std::cout << "Enemy Default Constructed\n";
}

Enemy::Enemy(Enemy const & src) {
	std::cout << "Enemy Copy-Constructed\n";
	*this = src;
}

Enemy::~Enemy() {
	std::cout << "Enemy De-Constructed\n";
}

Enemy &	Enemy::operator=(Enemy const & src) {
	Character::operator=(src);
	return (*this);
}

std::ostream & 			operator<<(std::ostream & o, Enemy const & rhs) {
	std::pair<int, int>	xy = rhs.getXY();
	o << "Dumping Enemy State\nX: " << xy.first <<"\tY: " << xy.second <<
	"\nSpeed: " << rhs.getSpeed() <<
	"\nHealth: " << rhs.getHealth() <<
	"\nLives: " << rhs.getLives() << std::endl;
	return o;
}