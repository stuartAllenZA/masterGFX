#include <EnemyDrop.hpp>

EnemyDrop::EnemyDrop(std::pair<int, int> xy) : Drop(xy) {
	std::cout << "EnemyDrop at x: " << xy.first << " y: " << xy.second << " Constructed\n";
}

EnemyDrop::EnemyDrop(EnemyDrop const & src) {
	std::cout << "EnemyDrop Copy-Constructed\n";
	*this = src;
}

EnemyDrop::~EnemyDrop() {
	std::cout << "EnemyDrop De-Constructed\n";
}

EnemyDrop &		EnemyDrop::operator=(EnemyDrop const & src) {
	this->_xy = src.getXY();
	return (*this);
}