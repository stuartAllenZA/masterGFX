#include <Sound.hpp>

Sound::Sound() : _musicVol(1.0f), _FXVol(1.0f) {
	std::cout << "Sound Constructed\n";
}

Sound::Sound(int musicVol, int FXVol) : _musicVol(musicVol / 100), _FXVol(FXVol / 100) {
	std::cout << "Sound Constructed\n";
}

Sound::Sound(Sound const & src) {
	std::cout << "Sound Copy-Constructed\n";
	*this = src;
}

Sound::~Sound() {
	this->_engine->drop();
	std::cout << "Sound De-Constructed\n";
}

Sound &					Sound::operator=(Sound const & src) {
	this->_musicVol = src.getMusicVol();
	this->_FXVol = src.getFXVol();
	return (*this);
}

int 					Sound::getMusicVol() const {
	return (this->_musicVol);
}

void					Sound::setMusicVol(const int newVol) {
	this->_musicVol = newVol;
	this->updateMusicVol();
}

int 					Sound::getFXVol() const {
	return (this->_FXVol);
}

void					Sound::setFXVol(const int newVol) {
	this->_FXVol = newVol;
	this->updateFXVol();
}

void					Sound::init() {
	chdir(".irrKlang/");
	this->_engine = irrklang::createIrrKlangDevice();
	chdir("..");
	if (!this->_engine)
		throw (Exceptions::SoundDeviceInitError());
	else {
		this->_menuMusic = _engine->addSoundSourceFromFile("resources/sounds/menuMusic.wav");
		this->_gameMusic = _engine->addSoundSourceFromFile("resources/sounds/gameMusic.mp3");
		this->_menuPass = _engine->addSoundSourceFromFile("resources/sounds/menuPass.mp3");
		this->_menuFail = _engine->addSoundSourceFromFile("resources/sounds/menuFail.wav");
		this->_menuHover = _engine->addSoundSourceFromFile("resources/sounds/menuHover.wav");
		this->_menuClick = _engine->addSoundSourceFromFile("resources/sounds/menuClick.mp3");
		this->_menuLimit = _engine->addSoundSourceFromFile("resources/sounds/menuLimit.wav");
		this->_gameSaved = _engine->addSoundSourceFromFile("resources/sounds/gameSaved.wav");
		this->_playerSpawn = _engine->addSoundSourceFromFile("resources/sounds/playerSpawn");
		this->_playerWalk = _engine->addSoundSourceFromFile("resources/sounds/playerWalk.wav");
		this->_enemySpawn = _engine->addSoundSourceFromFile("resources/sounds/enemySpawn");
		this->_enemyWalk = _engine->addSoundSourceFromFile("resources/sounds/enemyWalk");
		this->_plantBomb = _engine->addSoundSourceFromFile("resources/sounds/plantBomb");
		this->_detonateBomb = _engine->addSoundSourceFromFile("resources/sounds/detonateBomb");
		this->_dropDrop = _engine->addSoundSourceFromFile("resources/sounds/dropDrop.wav");
		this->_pickupDrop = _engine->addSoundSourceFromFile("resources/sounds/pickupDrop.wav");
		this->updateMusicVol();
		this->updateFXVol();
		this->startMenuMusic();
	}
}

void					Sound::updateMusicVol() {
	this->_menuMusic->setDefaultVolume(this->_musicVol);
	this->_gameMusic->setDefaultVolume(this->_musicVol);
}

void					Sound::updateFXVol() {
	this->_menuPass->setDefaultVolume(this->_FXVol);
	this->_menuFail->setDefaultVolume(this->_FXVol);
	this->_menuHover->setDefaultVolume(this->_FXVol);
	this->_menuClick->setDefaultVolume(this->_FXVol);
	this->_playerSpawn->setDefaultVolume(this->_FXVol);
	this->_playerWalk->setDefaultVolume(this->_FXVol);
	this->_enemySpawn->setDefaultVolume(this->_FXVol);
	this->_enemyWalk->setDefaultVolume(this->_FXVol);
	this->_plantBomb->setDefaultVolume(this->_FXVol);
	this->_detonateBomb->setDefaultVolume(this->_FXVol);
	this->_dropDrop->setDefaultVolume(this->_FXVol);
	this->_pickupDrop->setDefaultVolume(this->_FXVol);
}

void					Sound::startMenuMusic() {
	std::cout << "Starting Menu Music\n";
	this->_engine->play2D(this->_menuMusic, true);
	std::cout << "Menu Music playing.\n";
}

void					Sound::stopMenuMusic() {
	std::cout << "Stopping Menu Music\n";
	if (this->_engine && this->_menuMusic) {
		this->_engine->removeSoundSource(this->_menuMusic);
		std::cout << "Menu Music stopped.\n";
	}
}

void					Sound::startGameMusic() {
	std::cout << "Starting Game Music\n";
	this->_engine->play2D(this->_gameMusic, true);
	std::cout << "Game Music playing.\n";
}

void					Sound::stopGameMusic() {
	std::cout << "Stopping Game Music\n";
	if (this->_engine && this->_gameMusic) {
		this->_engine->removeSoundSource(this->_gameMusic);
		std::cout << "Game Music stopped.\n";
	}
}

void					Sound::playMenuPass() {
	std::cout << "Playing Menu Pass\n";
	this->_engine->play2D(this->_menuPass, false);
}

void					Sound::playMenuFail() {
	std::cout << "Playing Menu Fail\n";
	this->_engine->play2D(this->_menuFail, false);
}

void					Sound::playMenuHover() {
	std::cout << "Playing Menu Hover\n";
	this->_engine->play2D(this->_menuHover, false);
}

void					Sound::playMenuClick() {
	std::cout << "Playing Menu Click\n";
	this->_engine->play2D(this->_menuClick, false);
}

void					Sound::playMenuLimit() {
	std::cout << "Playing Menu Limit\n";
	this->_engine->play2D(this->_menuLimit, false);
}

void					Sound::playGameSaved() {
	std::cout << "Playing Game Saved\n";
	this->_engine->play2D(this->_gameSaved, false);
}

void					Sound::playPlayerSpawn() {
	std::cout << "Playing Player Spawn\n";
	this->_engine->play2D(this->_playerSpawn, false);
}

void					Sound::playPlayerWalk() {
	std::cout << "Playing Player Walk\n";
	this->_engine->play2D(this->_playerWalk, false);
}

void					Sound::playEnemySpawn() {
	std::cout << "Playing Enemy Spawn\n";
	this->_engine->play2D(this->_enemySpawn, false);
}

void					Sound::playEnemyWalk() {
	std::cout << "Playing Enemy Walk\n";
	this->_engine->play2D(this->_enemyWalk, false);
}

void					Sound::playPlantBomb() {
	std::cout << "Playing Plant Bomb\n";
	this->_engine->play2D(this->_plantBomb, false);
}

void					Sound::playDetonateBomb() {
	std::cout << "Playing Detonate Bomb\n";
	this->_engine->play2D(this->_detonateBomb, false);
}

void					Sound::playDropDrop() {
	std::cout << "Playing Drop Drop\n";
	this->_engine->play2D(this->_dropDrop, false);
}

void					Sound::playPickupDrop() {
	std::cout << "Playing Pickup Drop\n";
	this->_engine->play2D(this->_pickupDrop, false);
}