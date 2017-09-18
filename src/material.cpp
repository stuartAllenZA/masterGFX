//#define STB_IMAGE_IMPLEMENTATION
#include <material.hpp>

std::vector<GLuint>     Texture::textureIDs;

Texture::Texture(const char *path)
{
	loadTextureFromPath(path);
}

Texture::Texture(const Texture &rhs) { *this = rhs; }

Texture& Texture::operator=(const Texture &rhs)
{
	if (this != &rhs)
	{
		_textId = rhs._textId;
		_loaded = rhs._loaded;
	}
	return *this;
}

Texture::~Texture()
{
	//glDeleteTextures(1, &_textId);
}

bool Texture::loadTextureFromPath(const char *path)
{
	unsigned char *data;

	data = stbi_load(path, &_width, &_height, &_channel, STBI_default);
	if (data == nullptr)
	{
		std::cerr << "Failed to load texture from " << path << std::endl;
		return false;
	}
	if (_loadImageToGPU(data))
		_loaded = true;
	stbi_image_free(data);
	return true;
}

bool Texture::loadTextureFromData(unsigned char *imageData, size_t len)
{
	unsigned char *data;

	data = stbi_load_from_memory(imageData, (int)len, &_width, &_height, &_channel, STBI_default);
	if (data == nullptr)
	{
		std::cerr << "Failed to load texture from data!" << std::endl;
		return false;
	}
	if (_loadImageToGPU(data))
		_loaded = true;
	stbi_image_free(data);
	return true;
}

bool Texture::_loadImageToGPU(unsigned char *data)
{
	glGenTextures(1, &_textId);
	Texture::textureIDs.push_back(_textId);
	glBindTexture(GL_TEXTURE_2D, _textId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (_channel == STBI_rgb_alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Window::getError((char *)"texture load image to gpu!");
	return true;
}

bool Texture::isLoaded() const { return _loaded; }

void Texture::bindTexture(int index)
{
	if (_loaded)
	{
		glActiveTexture((GLenum)GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, _textId);
	}
}

void Texture::unbindTexture() { glBindTexture(GL_TEXTURE_2D, 0); }

GLuint Texture::getTextureId() const {
	if (_loaded)
		return _textId;
	return 0;
}

void Material::sendMaterialToShader(Shader& shader, Material &mat, int index)
{
	mat.texure.bindTexture(index);
	std::string str = std::string("textures[") + std::to_string(index) + std::string("]");
	std::string str1 = std::string("isTexture[") + std::to_string(index) + std::string("]");
	std::string str2 = std::string("texColor[") + std::to_string(index) + std::string("]");

	shader.setUniform1i((GLchar *)str.c_str(), (GLint)index);
	shader.setUniform1i((GLchar *)str1.c_str(), (int)mat.texure.isLoaded());
	shader.setUniform4f((GLchar *)str2.c_str(), mat.base_color);
}
