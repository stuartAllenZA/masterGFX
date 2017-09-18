#include <BreakableBox.hpp>

BreakableBox::BreakableBox() : Box() {
	std::cout << "BreakableBox Constructed\n";
}

BreakableBox::BreakableBox(Drop *drop) : Box(drop) {
	if (drop)
		std::cout << "BreakableBox with drop Constructed\n";
	else
		std::cout << "BreakableBox was passed a NULL drop, Constructed\n";
}

BreakableBox::BreakableBox(std::pair<int, int> xy) : Box(xy) {
	std::cout << "BreakableBox at x: " << xy.first << " y: " << xy.second << " Constructed\n";
}

BreakableBox::BreakableBox(std::pair<int, int> xy, Drop *drop) : Box(xy, drop) {
	if (drop)
		std::cout << "BreakableBox with drop at x: " << xy.first << " y: " << xy.second << " Constructed\n";
	else
		std::cout << "BreakableBox at x: " << xy.first << " y: " << xy.second << " was passed a NULL drop, Constructed\n";
}

BreakableBox::BreakableBox(BreakableBox const & src) {
	std::cout << "BreakableBox Copy-Constructed\n";
	*this = src;
}

BreakableBox::~BreakableBox() {
	std::cout << "BreakableBox De-Constructed\n";
}

BreakableBox &		BreakableBox::operator=(BreakableBox const & src) {
	this->_xy = src.getXY();
	this->_drop = src.getDrop();
	return (*this);
}
