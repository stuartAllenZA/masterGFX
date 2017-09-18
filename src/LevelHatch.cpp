#include <LevelHatch.hpp>

LevelHatch::LevelHatch(std::pair<int, int> xy) : Drop(xy) {
	std::cout << "LevelHatch at x: " << xy.first << " y: " << xy.second << " Constructed\n";
}

LevelHatch::LevelHatch(LevelHatch const & src) {
	std::cout << "LevelHatch Copy-Constructed\n";
	*this = src;
}

LevelHatch::~LevelHatch() {
	std::cout << "LevelHatch De-Constructed\n";
}

LevelHatch &		LevelHatch::operator=(LevelHatch const & src) {
	this->_xy = src.getXY();
	return (*this);
}