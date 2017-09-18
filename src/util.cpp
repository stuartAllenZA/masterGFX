#include <util.h>

char        *readFile(const char *filePath)
{
	FILE    *file;
	long    size;
	char    *data;

	if ((file = fopen(filePath, "rt")) == nullptr)
	{
		std::cerr << "Failed to open file : " << filePath << "!" << std::endl;
		return nullptr;
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	if ((data = (char *)malloc(sizeof(char) * size + 1)) == nullptr)
	{
		fclose(file);
		std::cerr << "Failed to malloc for file : " << filePath << "!" << std::endl;
		return nullptr;
	}
	bzero(data, (size_t)size + 1);
	if (fread(data, 1, (size_t)size, file) != size)
	{
		fclose(file);
		free(data);
		std::cerr << "Failed to read file : " << filePath << "!" << std::endl;
		return nullptr;
	}
	fclose(file);
	return data;
}
