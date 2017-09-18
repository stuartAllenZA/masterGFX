#include <Bomberman.hpp>

int main(int argc, char **argv) {
	//Check for start arguments
	if (argc > 1) {
		std::cout << "Argument has been passed.\n";
		if (strcmp(argv[1], "--help"))
			std::cout << "Please run Bomberman without any arguments as all configs are done in game.\n";
	}

	try {
		std::cout << "initializing core" << std::endl;
		Core core;
        std::cout << "running" << std::endl;
        core.run();
		std::cout << "end of try" << std::endl;
	}

	//Catch Fatal Exceptions
	catch (Exceptions::SoundDeviceInitError const & exception) {
		std::cerr << "Exceptions::SoundDeviceInitError" << exception.what() << std::endl;
	}

	catch (Exceptions::LoadPlayerError const & exception) {
		std::cerr << "Exceptions::LoadPlayerError: " << exception.what() << std::endl;
	}

	catch (Exceptions::LexOpenFileError const & exception) {
		std::cerr << "Exceptions::LexOpenFileError: " << exception.what() << std::endl;
	}

	catch (Exceptions::LexFormatError const & exception) {
		std::cerr << "Exceptions::LexFormatError: " << exception.what() << std::endl;
	}

	catch (Exceptions::LexKeyNotFound const & exception) {
		std::cerr << "Exceptions::LexKeyNotFound: " << exception.what() << std::endl;
	}

	catch (std::exception const & exception) {
		std::cerr << "std::exception: " << exception.what() << std::endl;
	}

	catch (...) {
		std::cerr << "unknown exception: " << std::endl;
	}

	return (0);
}
