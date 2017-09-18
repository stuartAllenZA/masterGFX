#include <Settings.hpp>

Settings::Settings() : _resolution(std::make_pair(800, 600)), _windowed(true), _xPos(560), _yPos(153), _upKey(GLFW_KEY_W), _downKey(GLFW_KEY_S), _leftKey(GLFW_KEY_A), _rightKey(GLFW_KEY_D), _actionKey(GLFW_KEY_SPACE), _acceptKey(GLFW_KEY_ENTER), _escapeKey(GLFW_KEY_ESCAPE), _musicVol(100), _FXVol(100) {
	//std::cout << "Settings Default Constructed\n";
}

Settings::Settings(Settings const & src) {
	//std::cout << "Settings Copy-Constructed\n";
	*this = src;
}

Settings::~Settings() {
	//std::cout << "Settings De-Constructed\n";
}

Settings &			Settings::operator=(Settings const & src) {
	this->_resolution = src.getResolution();
	this->_lastPlayer = src.getLastPlayer();
	this->_windowed = src.getWindowed();
	this->_xPos = src.getXPos();
	this->_yPos = src.getYPos();
	this->_upKey = src.getUpKey();
	this->_downKey = src.getDownKey();
	this->_leftKey = src.getLeftKey();
	this->_rightKey = src.getRightKey();
	this->_actionKey = src.getActionKey();
	this->_acceptKey = src.getAcceptKey();
	this->_escapeKey = src.getEscapeKey();
	this->_musicVol = src.getMusicVol();
	this->_FXVol = src.getFXVol();
	return (*this);
}

std::pair<int, int>	Settings::getResolution() const {
	return (this->_resolution);
}

void				Settings::setResolution(const std::pair<int, int> wh) {
	this->_resolution = wh;
}

std::string			Settings::getLastPlayer() const {
	return (this->_lastPlayer);
}

void				Settings::setLastPlayer(const std::string newLastPlayer) {
	this->_lastPlayer = newLastPlayer;
}

bool				Settings::getWindowed() const {
	return (this->_windowed);
}

void				Settings::setWindowed(const bool newWin) {
	this->_windowed = newWin;
}

int					Settings::getXPos() const {
	return (this->_xPos);
}

void				Settings::setXPos(const int newXPos) {
	this->_xPos = newXPos;
	std::cout << "_xPos set to: " << newXPos << " Check: " << this->_xPos << std::endl;
}

int					Settings::getYPos() const {
	return (this->_yPos);
}

void				Settings::setYPos(const int newYPos) {
	this->_yPos = newYPos;
	std::cout << "_yPos set to: " << newYPos << " Check: " << this->_yPos << std::endl;
}

int 				Settings::getUpKey() const {
	return (this->_upKey);
}

void				Settings::setUpKey(const int newKey) {
	this->_upKey = newKey;
}

int 				Settings::getDownKey() const {
	return (this->_downKey);
}

void				Settings::setDownKey(const int newKey) {
	this->_downKey = newKey;
}

int 				Settings::getLeftKey() const {
	return (this->_leftKey);
}

void				Settings::setLeftKey(const int newKey) {
	this->_leftKey = newKey;
}

int 				Settings::getRightKey() const {
	return (this->_rightKey);
}

void				Settings::setRightKey(const int newKey) {
	this->_rightKey = newKey;
}

int 				Settings::getActionKey() const {
	return (this->_actionKey);
}

void				Settings::setActionKey(const int newKey) {
	this->_actionKey = newKey;
}

int 				Settings::getAcceptKey() const {
	return (this->_acceptKey);
}

void				Settings::setAcceptKey(const int newKey) {
	this->_acceptKey = newKey;
}

int 				Settings::getEscapeKey() const {
	return (this->_escapeKey);
}

void				Settings::setEscapeKey(const int newKey) {
	this->_escapeKey = newKey;
}

int 				Settings::getMusicVol() const {
	return (this->_musicVol);
}

void				Settings::setMusicVol(const int newVol) {
	this->_musicVol = newVol;
}

int 				Settings::getFXVol() const {
	return (this->_FXVol);
}

void				Settings::setFXVol(const int newVol) {
	this->_FXVol = newVol;
}

int					Settings::getResolutionX() const {
	return (this->_resolution.first);
}

int					Settings::getResolutionY() const {
	return (this->_resolution.second);
}

std::ostream & 			operator<<(std::ostream & o, Settings const & rhs) {
	o << "\n\nDumping Current Settings\nResolution: " << rhs.getResolutionX() << "x" << rhs.getResolutionY() <<
	"\nLast Player: " << rhs.getLastPlayer() <<
	"\nWindowed: " << std::boolalpha << rhs.getWindowed() <<
	"\nX Pos: " << rhs.getXPos() <<
	"\nY Pos: " << rhs.getYPos() <<
	"\nUp Key: " << rhs.getUpKey() <<
	"\nDown Key: " << rhs.getDownKey() <<
	"\nLeft Key: " << rhs.getLeftKey() <<
	"\nRight Key: " << rhs.getRightKey() <<
	"\nMusic Volume: " << rhs.getMusicVol() <<
	"\nFX Volume: " << rhs.getFXVol();
	return o;
}