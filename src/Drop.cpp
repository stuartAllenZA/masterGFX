#include <Drop.hpp>

Drop::Drop() {
	std::cout << "Drop Default Constructed\n";
}

Drop::Drop(std::pair<int, int> xy) : _xy(xy) {
	std::cout << "Drop at x: " << xy.first << " y: " << xy.second << " Constructed\n";
}

Drop::Drop(Drop const & src) {
	std::cout << "Drop Copy-Constructed\n";
	*this = src;
}

Drop::~Drop() {
	std::cout << "Drop De-Constructed\n";
}

Drop &				Drop::operator=(Drop const & src) {
	this->_xy = src.getXY();
	return (*this);
}

std::pair<int, int>	Drop::getXY() const {
	return (this->_xy);
}

void				Drop::setXY(const std::pair<int, int> xy) {
	this->_xy = xy;
}
