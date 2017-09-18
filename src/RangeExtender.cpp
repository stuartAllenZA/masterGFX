#include <RangeExtender.hpp>

RangeExtender::RangeExtender(std::pair<int, int> xy) : Drop(xy) {
	std::cout << "RangeExtender at x: " << xy.first << " y: " << xy.second << " Constructed\n";
}

RangeExtender::RangeExtender(RangeExtender const & src) {
	std::cout << "RangeExtender Copy-Constructed\n";
	*this = src;
}

RangeExtender::~RangeExtender() {
	std::cout << "RangeExtender De-Constructed\n";
}

RangeExtender &		RangeExtender::operator=(RangeExtender const & src) {
	this->_xy = src.getXY();
	return (*this);
}