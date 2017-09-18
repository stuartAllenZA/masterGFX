#include <RemoteDetonator.hpp>

RemoteDetonator::RemoteDetonator(std::pair<int, int> xy) : Drop(xy) {
	std::cout << "RemoteDetonator at x: " << xy.first << " y: " << xy.second << " Constructed\n";
}

RemoteDetonator::RemoteDetonator(RemoteDetonator const & src) {
	std::cout << "RemoteDetonator Copy-Constructed\n";
	*this = src;
}

RemoteDetonator::~RemoteDetonator() {
	std::cout << "RemoteDetonator De-Constructed\n";
}

RemoteDetonator &		RemoteDetonator::operator=(RemoteDetonator const & src) {
	this->_xy = src.getXY();
	return (*this);
}