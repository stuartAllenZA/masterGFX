#define TINYGLTF_IMPLEMENTATION
#include <Model.hpp>

Model::Model(const char *modelPath) : _shader("GraphicsEngine/shaders/basic.vert", "GraphicsEngine/shaders/basic.frag") {
	std::cout << "model constructed\n";
	loadFromFile(_shader, modelPath);
}

Model::~Model() {
	std::cout << "model destructed\n";
}

Model::Model() : _shader("GraphicsEngine/shaders/basic.vert", "GraphicsEngine/shaders/basic.frag") {
	std::cout << "model constructed\n";
}

glm::mat4	Model::makeMat() {
	mat = glm::translate(glm::mat4(), glm::vec3(1.0, 0.0, -5.0));
}

bool Model::loadFromFile(Shader &shader, const char *path)
{
	std::cout << "loading model from path: " << path << std::endl;
	bool                ret;
	std::string         err;
	tinygltf::TinyGLTF  loader;

	_shader = shader;
	_path = std::string(path);
	ret = loader.LoadASCIIFromFile(&_model, &err, path);
	if (!err.empty())
		std::cerr << "glTF parse error: " << err << std::endl;
	if (!ret)
	{
		std::cerr << "Failed to load glTF: " << path << std::endl;
		return false;
	}
	std::cout << "model loaded\n";
	return _processModel();
}

bool Model::_processModel()
{
	std::cout << "processing model\n";
	std::vector<tinygltf::Accessor>& acc = _model.accessors;
	std::vector<tinygltf::BufferView>&  bufViews = _model.bufferViews;
	std::vector<tinygltf::Buffer>& bufs = _model.buffers;

	for (tinygltf::Scene& scene : _model.scenes)
	{
		for (int i : scene.nodes)
			_processNode(i);
	}
	if (_vertex.empty())
	{
		for (tinygltf::Mesh& mesh : _model.meshes)
			_processModelMesh(mesh, 0);
	}
	_loadDataToGpu();
	_loadMaterials();
	_clearVectors();
	_loaded = true;
	for (tinygltf::Animation& animation : _model.animations)
	{
		auto *anime = new Animation(animation, _model);
		_animations.push_back(anime);
	}
	std::cout << "model processed\n";
	return true;
}

void Model::_processNode(int index)
{
	std::cout << "processing node: " << index << std::endl;
	tinygltf::Node& node = _model.nodes[index];
	/// getting mesh data
	if (node.mesh >= 0)
		_processModelMesh(_model.meshes[node.mesh], index);
	/// getting joint connections
	if (!_model.skins.empty() && node.skin >= 0)
		_processSkin(_model.skins[node.skin]);
	/// processing children nodes
	for (int i : node.children)
		_processNode(i);
	std::cout << "node processed: " << index << std::endl;
}

void Model::_processSkin(tinygltf::Skin &skin)
{
	std::cout << "processing skin\n";
	tinygltf::Accessor& acc = _model.accessors[skin.inverseBindMatrices];
	tinygltf::BufferView& bufView = _model.bufferViews[acc.bufferView];

	_hasJoint = true;
	/// array of inverse matrices for all joints in the skin
	auto *data = (glm::mat4 *)(_model.buffers[bufView.buffer].data.data() + bufView.byteOffset);
	std::vector<glm::mat4> mats;
	mats.insert(mats.end(), data, (data + acc.count));
	/// getting joints from skin (skeleton)
	for (int i : _model.nodes[skin.skeleton].children)
	{
		auto *bone = _processSkinJoints(i, mats, skin.joints[0], skin.joints[0]);
		if (bone != nullptr)
			_bones.push_back(bone);
	}
	std::cout << "skin processed\n";
}

Joint   *getJointFromBones(std::vector<Joint *> bones, int id)
{
	std::cout << "geting joint from bones\n";
	Joint *bone = nullptr;

	for (Joint *tmp : bones)
	{
		if (tmp->index == id)
		{
			bone = tmp;
			break;
		}
		if (!tmp->children.empty())
			tmp = getJointFromBones(tmp->children, id);
	}
	std::cout << "got joint from bones\n";
	return bone;
}

Joint* Model::_processSkinJoints(int id, std::vector<glm::mat4> mats, int start, int skeleton)
{
	std::cout << "processing skin joints\n";
	tinygltf::Node& node = _model.nodes[id];

	if (id < skeleton)
		return nullptr;
	if (getJointFromBones(_bones, id) != nullptr)
		return nullptr;
	auto *bone = new Joint();
	bone->index = id;
	bone->id = id - start;
	bone->invMatrix = mats[bone->id];
	bone->name = node.name;
	/// setting max bone count
	if (bone->id > _boneCount)
		_boneCount = bone->id;
	/// getting joint children
	for (int i : node.children)
	{
		auto *childBone = _processSkinJoints(i, mats, start, skeleton);
		if (childBone != nullptr)
			bone->children.push_back(childBone);
	}
	std::cout << "skin joints processed\n";
	return bone;
}

void Model::_processModelMesh(tinygltf::Mesh& mesh, int node)
{
	std::cout << "processing model mesh\n";
	std::vector<tinygltf::Accessor>&    acc = _model.accessors;
	std::vector<tinygltf::BufferView>&  bufViews = _model.bufferViews;
	std::vector<tinygltf::Buffer>&      bufs = _model.buffers;
	std::map<std::string, int>::const_iterator it;

	tinygltf::BufferView bufView;

	for (tinygltf::Primitive& prim : mesh.primitives)
	{
		tinygltf::Accessor& Acc = acc[prim.attributes["POSITION"]];
		size_t currVecSize = _vertex.size() / 3;

		int matVal = (prim.material == -1) ? 0 : prim.material;
		/// adding material indexes to _matIndexs
		Model::loadOneManyToVector(_matIndexs, (float)matVal, (int)Acc.count);
		/// adding nodes to _nodes for animation
		Model::loadOneManyToVector(_nodes, node, (int)Acc.count);
		/// adding position vertices to _vertex array
		bufView = bufViews[Acc.bufferView];
		auto *posData = (float *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
		_vertex.insert(_vertex.end(), posData, (posData + (Acc.count * 3)));
		/// adding normals to _normals
		if ((it = prim.attributes.find("NORMAL")) != prim.attributes.end())
		{
			Acc = acc[prim.attributes["NORMAL"]];
			bufView = bufViews[Acc.bufferView];
			auto *data = (float *)&bufs[bufView.buffer].data[bufView.byteOffset];
			_normals.insert(_normals.end(), data, (data + (Acc.count * 3)));
		}
		/// adding morph target
		if (!prim.targets.empty())
		{
			int  i = 0;
			for (std::map<std::string, int>& target : prim.targets)
			{
				/// adding target morph position
				if ((it = target.find("POSITION")) != target.end())
				{
					Acc = acc[target["POSITION"]];
					bufView = bufViews[Acc.bufferView];
					auto *data = (float *)&bufs[bufView.buffer].data[bufView.byteOffset];
					if (i == 0)
						_targetPosition.insert(_targetPosition.end(), data, (data + (Acc.count * 3)));
					else if (i == 1)
						_targetPosition1.insert(_targetPosition1.end(), data, (data + (Acc.count * 3)));
				}
				/// adding target morph normal
				if ((it = target.find("NORMAL")) != target.end())
				{
					Acc = acc[target["NORMAL"]];
					bufView = bufViews[Acc.bufferView];
					auto *data = (float *)&bufs[bufView.buffer].data[bufView.byteOffset];
					if (i == 0)
						_targetNormal.insert(_targetNormal.end(), data, (data + (Acc.count * 3)));
					else if (i == 1)
						_targetNormal1.insert(_targetNormal1.end(), data, (data + (Acc.count * 3)));
				}
				i++;
			}
		}

		/// adding uv coords to _uvs
		if ((it = prim.attributes.find("TEXCOORD_0")) != prim.attributes.end())
		{
			Acc = acc[prim.attributes["TEXCOORD_0"]];
			bufView = bufViews[Acc.bufferView];
			auto *data = (float *)&bufs[bufView.buffer].data[bufView.byteOffset];
			_uvs.insert(_uvs.end(), data, (data + (Acc.count * 2)));
		}
		/// adding joint coords to _joints
		if ((it = prim.attributes.find("JOINTS_0")) != prim.attributes.end())
		{
			Acc = acc[prim.attributes["JOINTS_0"]];
			bufView = bufViews[Acc.bufferView];
			auto *data = (GLushort *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
			//_joints.insert(_joints.end(), data, (data + (Acc.count * 4)));
			size_t count = Acc.count * 4;
			for (size_t i = 0; i < count; i++)
				_joints.push_back((float)data[i]);
		}
		if ((it = prim.attributes.find("JOINTS_1")) != prim.attributes.end())
		{
			Acc = acc[prim.attributes["JOINTS_1"]];
			bufView = bufViews[Acc.bufferView];
			auto *data = (GLushort *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
			//_joints.insert(_joints.end(), data, (data + (Acc.count * 4)));
			size_t count = Acc.count * 4;
			for (size_t i = 0; i < count; i++)
				_joints.push_back((float)data[i]);
		}
		if ((it = prim.attributes.find("JOINTS_2")) != prim.attributes.end())
		{
			Acc = acc[prim.attributes["JOINTS_2"]];
			bufView = bufViews[Acc.bufferView];
			auto *data = (GLushort *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
			//_joints.insert(_joints.end(), data, (data + (Acc.count * 4)));
			size_t count = Acc.count * 4;
			for (size_t i = 0; i < count; i++)
				_joints.push_back((float)data[i]);
		}
		/// adding weight coords to _weights
		if ((it = prim.attributes.find("WEIGHTS_0")) != prim.attributes.end())
		{
			Acc = acc[prim.attributes["WEIGHTS_0"]];
			bufView = bufViews[Acc.bufferView];
			auto *data = (float *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
			_weights.insert(_weights.end(), data, (data + (Acc.count * 4)));
		}
		if ((it = prim.attributes.find("WEIGHTS_1")) != prim.attributes.end())
		{
			Acc = acc[prim.attributes["WEIGHTS_1"]];
			bufView = bufViews[Acc.bufferView];
			auto *data = (float *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
			_weights.insert(_weights.end(), data, (data + (Acc.count * 4)));
		}
		if ((it = prim.attributes.find("WEIGHTS_2")) != prim.attributes.end())
		{
			Acc = acc[prim.attributes["WEIGHTS_2"]];
			bufView = bufViews[Acc.bufferView];
			auto *data = (float *)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
			_weights.insert(_weights.end(), data, (data + (Acc.count * 4)));
		}
		/// adding indices of mesh to _indices
		Acc = acc[prim.indices];
		bufView = bufViews[Acc.bufferView];
		auto indicesData = (GLushort*)(bufs[bufView.buffer].data.data() + bufView.byteOffset);
		//std::cout << "Indices ";
		for (int i = 0; i < Acc.count; i++)
		{
			//std::cout << indicesData[i] << " ";
			_indices.push_back((GLushort)(indicesData[i] + currVecSize));
		}
		//std::cout << std::endl;
	}
	std::cout << "mesh loaded\n";
}

void Model::_loadDataToGpu()
{
	std::cout << "loading data to GPU\n";
	GLuint  vbo[11];
	GLint position = _shader.getAttribLocation((char *)"position");
	GLint targetPosition = _shader.getAttribLocation((char *)"targetPosition");
	GLint targetNormal = _shader.getAttribLocation((char *)"targetNormal");
	GLint targetPosition1 = _shader.getAttribLocation((char *)"targetPosition1");
	GLint targetNormal1 = _shader.getAttribLocation((char *)"targetNormal1");
	GLint matIndex = _shader.getAttribLocation((char *)"matIndex");
	GLint normal = _shader.getAttribLocation((char *)"normal");
	GLint joint = _shader.getAttribLocation((char *)"joint");
	GLint weight = _shader.getAttribLocation((char *)"weight");
	GLint uv = _shader.getAttribLocation((char *)"uv");
	GLint node = _shader.getAttribLocation((char *)"node");

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(9, vbo);
	_vbos.insert(_vbos.end(), vbo, vbo + 6);
	if (!_vertex.empty() && position != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, _vertex.size() * sizeof(GLfloat), _vertex.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)position);
		glVertexAttribPointer((GLuint)position, 3, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
	}
	if (!_normals.empty() && normal != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat), _normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)normal);
		glVertexAttribPointer((GLuint)normal, 3, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
	}
	if(!_targetPosition.empty() && targetPosition != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
		glBufferData(GL_ARRAY_BUFFER, _targetPosition.size() * sizeof(GLfloat), _targetPosition.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)targetPosition);
		glVertexAttribPointer((GLuint)targetPosition, 3, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
	}
	if(!_targetPosition1.empty() && targetPosition1 != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
		glBufferData(GL_ARRAY_BUFFER, _targetPosition1.size() * sizeof(GLfloat), _targetPosition1.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)targetPosition1);
		glVertexAttribPointer((GLuint)targetPosition1, 3, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
	}
	if (!_matIndexs.empty() && matIndex != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, _matIndexs.size() * sizeof(GLfloat), _matIndexs.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)matIndex);
		glVertexAttribPointer((GLuint)matIndex, 1, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
	}
	if (!_nodes.empty() && node != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
		glBufferData(GL_ARRAY_BUFFER, _nodes.size() * sizeof(GLint), _nodes.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)node);
		glVertexAttribPointer((GLuint)node, 1, GL_INT, GL_FALSE, 0, (void *)nullptr);
	}
	if (!_uvs.empty() && uv != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
		glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(GLfloat), _uvs.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)uv);
		glVertexAttribPointer((GLuint)uv, 2, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
	}
	if (!_joints.empty() && joint != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
		glBufferData(GL_ARRAY_BUFFER, _joints.size() * sizeof(GLfloat), _joints.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)joint);
		glVertexAttribPointer((GLuint)joint, 4, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
	}
	if (!_weights.empty() && weight != -1)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
		glBufferData(GL_ARRAY_BUFFER, _weights.size() * sizeof(GLfloat), _weights.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray((GLuint)weight);
		glVertexAttribPointer((GLuint)weight, 4, GL_FLOAT, GL_FALSE, 0, (void *)nullptr);
	}
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLushort), _indices.data(),
			GL_STATIC_DRAW);
	glBindVertexArray(0);
	std::cout << "data loaded to GPU\n";
}

void Model::_clearVectors()
{
	std::cout << "clearing vectors\n";
	_indicesCount = (GLushort)_indices.size();
	_indices.clear();
	_vertex.clear();
	_matIndexs.clear();
	_normals.clear();
	_uvs.clear();
	_nodes.clear();
}

void Model::_loadMaterials()
{
	std::cout << "loading materials\n";
	int     index = 0;

	for (tinygltf::Material& mat : _model.materials)
	{
		Material newMat;
		bool    foundMat = false;
		for (std::pair<std::string, tinygltf::Parameter> val : mat.values)
		{
			if (val.first == std::string("baseColorFactor") && val.second.number_array.size() == 4)
			{
				foundMat = true;
				newMat.base_color = glm::vec4(
						(float)val.second.number_array[0],
						(float)val.second.number_array[1],
						(float)val.second.number_array[2],
						(float)val.second.number_array[3]
						);
			}
			if (val.first == std::string("baseColorTexture") && !val.second.json_double_value.empty())
			{
				for (std::pair<std::string, double> ind : val.second.json_double_value)
				{
					foundMat = true;
					tinygltf::Image& image = _model.images[_model.textures[(int)ind.second].source];
					//tinygltf::Sampler& sampler = _model.samplers[_model.textures[(int)ind.second].sampler];

					if (image.uri.empty())
						newMat.texure.loadTextureFromData(image.image.data(),
								_model.bufferViews[image.bufferView].byteLength);
					else
					{
						std::size_t found = _path.find_last_of("/");
						if (found == std::string::npos)
							newMat.texure.loadTextureFromPath(image.uri.c_str());
						else
							newMat.texure.loadTextureFromPath
								((_path.substr(0, found+1) + image.uri).c_str());
					}
					break;
				}
			}
		}
		if (foundMat)
			addMaterial(index++, newMat);
	}
	std::cout << "materials loaded\n";
}

void Model::_loadMatrices(Joint *bone, glm::mat4 parentTransform)
{
	std::cout << "loading matrices\n";
	std::string str = std::string("jointMat[") + std::to_string(bone->id) + std::string("]");

	glm::mat4 currentLocalTransform = _animations[0]->getJointAnimationMatrix(bone->index);
	glm::mat4 currentTransform = parentTransform * _animations[0]->getJointTranslationMatrix(bone->index)
		* _animations[0]->getJointRotationMatrix(bone->index);

	for (Joint *child : bone->children)
		_loadMatrices(child, currentTransform);
	_shader.setUniformMat4((GLchar *)str.c_str(), currentTransform * bone->invMatrix);
	std::cout << "matrices loaded\n";
}

bool Model::loadAnimationMatrix(int animeType, float time)
{
	std::cout << "loading animation matrix\n";
	if (animeType >= _animations.size())
		return false;
	_animations[animeType]->setCurrentAnimationTime(time);
	_animations[animeType]->update();
	if (_hasJoint){
		for (Joint *bone : _bones)
			_loadMatrices(bone, glm::mat4());
		_animeMatrice.clear();
	}else
	{
		std::vector<float> weights = _animations[0]->getWeightAnimation(0);
		_shader.setUniform1i((GLchar *)"count", (int)weights.size());
		for (size_t i = 0; i < weights.size(); i++){
			std::string str = std::string("weights[") + std::to_string(i) + std::string("]");
			_shader.setUniform1f((GLchar *)str.c_str(), weights[i]);
		}
		_shader.setUniformMat4((GLchar *)"animeMat", _animations[0]->getJointAnimationMatrix(0));
		_animeMatrice.clear();
	}
	std::cout << "animation matrix loaded\n";
	return true;
}

void Model::render(glm::mat4 matrix)
{
	std::cout << "rendering model\n";
	_shader.enable();
	_shader.setUniformMat4((GLchar *)"model_matrix", matrix);
	if (!_animations.empty())
	{
		_shader.setUniform1i((GLchar *)"hasAnime", (int)true);
		_animations[0]->update();
		for (Joint *bone : _bones)
			_loadMatrices(bone, glm::mat4());
		_animations[0]->increaseCurrentTimeStamp(0.002f);
	}
	for (std::pair<int, Material> material : _materials)
		Material::sendMaterialToShader(_shader, material.second, material.first);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, (GLsizei)_indicesCount, GL_UNSIGNED_SHORT, (const GLvoid *)nullptr);
	glBindVertexArray(0);
	for (std::pair<int, Material> material : _materials)
		material.second.texure.unbindTexture();
	_shader.disable();
	_animeMatrice.clear();
	std::cout << "model rendered\n";
}

void Model::simpleRender(glm::mat4 matrix)
{
	std::cout << "simple render begin\n";
	_shader.setUniformMat4((GLchar *)"model_matrix", matrix);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, (GLsizei)_indicesCount, GL_UNSIGNED_SHORT, (const GLvoid *)nullptr);
	glBindVertexArray(0);
	std::cout << "simple render end\n";
}
