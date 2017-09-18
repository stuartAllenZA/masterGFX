#include <ExtraBomb.hpp>

ExtraBomb::ExtraBomb(std::pair<int, int> xy) : Drop(xy) {
	std::cout << "ExtraBomb at x: " << xy.first << " y: " << xy.second << " Constructed\n";
}

ExtraBomb::ExtraBomb(ExtraBomb const & src) {
	std::cout << "ExtraBomb Copy-Constructed\n";
	*this = src;
}

ExtraBomb::~ExtraBomb() {
	std::cout << "ExtraBomb De-Constructed\n";
}

ExtraBomb &		ExtraBomb::operator=(ExtraBomb const & src) {
	this->_xy = src.getXY();
	return (*this);
}