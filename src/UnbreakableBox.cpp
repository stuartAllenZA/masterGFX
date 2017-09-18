#include <UnbreakableBox.hpp>

UnbreakableBox::UnbreakableBox() : Box() {
	std::cout << "UnbreakableBox Constructed\n";
}

UnbreakableBox::UnbreakableBox(Drop *drop) : Box(drop) {
	if (drop)
		std::cout << "UnbreakableBox with drop Constructed\n";
	else
		std::cout << "UnbreakableBox was passed a NULL drop, Constructed\n";
}

UnbreakableBox::UnbreakableBox(std::pair<int, int> xy) : Box(xy) {
	std::cout << "UnbreakableBox at x: " << xy.first << " y: " << xy.second << " Constructed\n";
}

UnbreakableBox::UnbreakableBox(std::pair<int, int> xy, Drop *drop) : Box(xy, drop) {
	if (drop)
		std::cout << "UnbreakableBox with drop at x: " << xy.first << " y: " << xy.second << " Constructed\n";
	else
		std::cout << "UnbreakableBox at x: " << xy.first << " y: " << xy.second << " was passed a NULL drop, Constructed\n";
}

UnbreakableBox::UnbreakableBox(UnbreakableBox const & src) {
	std::cout << "UnbreakableBox Copy-Constructed\n";
	*this = src;
}

UnbreakableBox::~UnbreakableBox() {
	std::cout << "UnbreakableBox De-Constructed\n";
}

UnbreakableBox &		UnbreakableBox::operator=(UnbreakableBox const & src) {
	this->_xy = src.getXY();
	this->_drop = src.getDrop();
	return (*this);
}
