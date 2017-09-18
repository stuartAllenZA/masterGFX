#pragma once
#include "util.h"
//#include "tiny_gltf.h"
//#include "picojson.h"
#include "Shader.hpp"
#include "material.hpp"
#include "animation.hpp"
#include <map>
#include <GL/glew.h>
#include "../GraphicsEngine/glm/glm.hpp"
#include "../GraphicsEngine/glm/common.hpp"
#include "../GraphicsEngine/glm/gtc/matrix_transform.hpp"
#include "../GraphicsEngine/glm/gtx/quaternion.hpp"
#include "../GraphicsEngine/glm/gtc/matrix_transform.hpp"
#include "../GraphicsEngine/glm/gtc/type_ptr.hpp"

struct Joint
{
	int         id;
	int         index;
	std::string name;
	glm::mat4   invMatrix;
	std::vector<Joint *> children;
};

class Model {
	private:
		GLuint				_vao;
		std::vector<GLuint>	_vbos;
		GLuint				 _ibo;
		GLuint				_texture1;
		GLuint				_texture2;
		tinygltf::Model		_model;
		Shader				_shader;
		std::vector<int>    _nodes;
		std::string         _path;
		std::vector<GLfloat>  _joints;
		std::vector<GLfloat>      _weights;
		std::vector<Joint *>    _bones;
		std::vector<Animation *>    _animations;
		int                     _boneCount = 0;
		std::vector<GLushort>   _indices;
		std::vector<float>      _vertex;
		std::vector<float>      _matIndexs;
		std::vector<float>      _normals;
		std::vector<float>      _uvs;
		std::map<int, Material> _materials;
		bool                    _loaded = false;
		bool                    _hasJoint = false;
		glm::mat4               _locMat;
		GLushort                _indicesCount;
		std::map<int, glm::mat4>   _animeMatrice;
		std::vector<GLfloat>     _targetPosition;
		std::vector<GLfloat>     _targetNormal;
		std::vector<GLfloat>     _targetPosition1;
		std::vector<GLfloat>     _targetNormal1;

	public:
		glm::mat4 mat;
		glm::mat4	makeMat();
		Model();
		Model(const char *modelFile);
		~Model();

		bool    _processModel();
		void    _processModelMesh(tinygltf::Mesh& mesh, int node);
		void    _processNode(int index);
		void    _processSkin(tinygltf::Skin& skin);
		Joint   *_processSkinJoints(int id, std::vector<glm::mat4> mats, int start, int skeleton);
		void    _loadDataToGpu();
		void    _loadMaterials();
		void    _clearVectors();
		bool    _calcAnimation(tinygltf::Animation& anime, float time);
		glm::mat4   _processChanel(std::string& type, tinygltf::AnimationSampler& samp, float time);
		void    _loadMatrices(Joint *bone, glm::mat4 parentTransform);

		bool        loadFromFile(Shader& shader, const char *path);
		void        render(glm::mat4 matrix);
		void        simpleRender(glm::mat4 matrix);
		bool        loadAnimationMatrix(int animeType, float time);


		void	loadMeshData();
		void	addTextures();

		template <typename T>
			static  void    loadOneManyToVector(std::vector<T>& list, T val, int count)
			{
				for (int i = 0; i < count; i++)
					list.push_back(val);
			}


		void            setLocalMatrix(glm::mat4 mat) { _locMat = mat; }

		void            loadMaterialToShader()
		{
			for (std::pair<int, Material> material : _materials)
				Material::sendMaterialToShader(_shader, material.second, material.first);
		}

		void            unloadMaterialFromShader()
		{
			for (std::pair<int, Material> material : _materials)
				material.second.texure.unbindTexture();
		}

		void            addMaterial(int index, Material material)
		{
			_materials.insert(std::make_pair(index, material));
		}

		void            addTextureToMaterial(int index, Texture texture)
		{
			try {
				_materials.at(index).texure = texture;
			}catch (const std::out_of_range& oor) {
				std::cerr << "Invalid index in add texture in model : " << index << std::endl;
			}

		}

		void            addBaseColorToMaterial(int index, glm::vec4 color)
		{
			try {
				_materials.at(index).base_color = color;
			}catch (const std::out_of_range& oor) {
				std::cerr << "Invalid index in add base color in model : " << index << std::endl;
			}

		}

		void            loadTextureToMaterial(int index, const char *path)
		{
			try {
				_materials.at(index).texure.loadTextureFromPath(path);
			}catch (const std::out_of_range& oor) {
				std::cerr << "Invalid index in load texture in model : " << index << std::endl;
			}

		}
};
