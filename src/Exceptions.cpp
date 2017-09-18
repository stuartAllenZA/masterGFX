#include <Exceptions.hpp>

Exceptions::LoadPlayerError::LoadPlayerError(std::string const & playerName) throw() : _msg("Cannot load player: " + playerName + ". File not found") {}

Exceptions::LoadPlayerError::~LoadPlayerError() throw() {}

const char* Exceptions::LoadPlayerError::what() const throw()
{
	return this->_msg.c_str();
}

Exceptions::LexOpenFileError::LexOpenFileError(std::string const & fileName) throw() : _msg("Cannot open file: " + fileName) {}

Exceptions::LexOpenFileError::~LexOpenFileError() throw() {}

const char* Exceptions::LexOpenFileError::what() const throw()
{
	return this->_msg.c_str();
}

Exceptions::LexFormatError::LexFormatError(int const & num, std::string const & line) throw() : _msg(std::string("Key & Value incorrectly formatted on line ") + std::to_string(num) + ": " + line) {}

Exceptions::LexFormatError::~LexFormatError() throw() {}

const char* Exceptions::LexFormatError::what() const throw()
{
	return this->_msg.c_str();
}

Exceptions::LexKeyNotFound::LexKeyNotFound(std::string const & key) throw() : _msg(std::string("Cannot find key: ") + key) {}

Exceptions::LexKeyNotFound::~LexKeyNotFound() throw() {}

const char* Exceptions::LexKeyNotFound::what() const throw()
{
	return this->_msg.c_str();
}
