#include <animation.hpp>

Animation::Animation(tinygltf::Animation &animation, tinygltf::Model &model)
{
	_animationName = animation.name;
	_currentTimeStamp = 0.0f;
	_loadAnimation(animation, model);
}

Animation::Animation(const Animation &rhs)
{
	*this = rhs;
}

Animation& Animation::operator=(const Animation &rhs)
{
	if (this != &rhs)
	{
		_currentTimeStamp = rhs._currentTimeStamp;
		_animationName = rhs._animationName;
		_jointAnimations = rhs._jointAnimations;
	}
	return *this;
}

Animation::~Animation()
{
	for (std::pair<int, JointAnimation *> jointAnime : _jointAnimations)
		delete jointAnime.second;
	_jointAnimations.clear();
}

bool Animation::_loadAnimation(tinygltf::Animation &animation, tinygltf::Model &model)
{
	for (tinygltf::AnimationChannel& channel : animation.channels)
	{
		if (channel.target_path == std::string("translation"))
			_loadTranslationChannel(channel, animation.samplers[channel.sampler], model);
		else if (channel.target_path == std::string("rotation"))
			_loadRotationChannel(channel, animation.samplers[channel.sampler], model);
	}
	return true;
}

void Animation::_loadRotationChannel(tinygltf::AnimationChannel &channel, tinygltf::AnimationSampler &sampler, tinygltf::Model &model)
{
	JointAnimation          *joint;
	tinygltf::Buffer        buffer;
	tinygltf::Accessor      acc;
	tinygltf::BufferView    bufferView;

	if (_jointAnimations[channel.target_node] == nullptr)
		_jointAnimations[channel.target_node] = new JointAnimation();
	joint = _jointAnimations[channel.target_node];
	/// accessing sampler input for timestamp
	acc = model.accessors[sampler.input];
	bufferView = model.bufferViews[acc.bufferView];
	buffer = model.buffers[bufferView.buffer];
	/// adding timestamps
	//joint->rotation.timeStamps = (float *)buffer.data.data() + bufferView.byteOffset;
	auto *data = (float *)(buffer.data.data() + bufferView.byteOffset);
	joint->rotation.timeStamps.insert(joint->rotation.timeStamps.end(), data , (data + acc.count));
	//joint->rotation.timeStamps = joint->translation.timeStamps;
	/// setting maxTime, subtracting timestamps[0] to make it always have minTime of 0.0f
	joint->rotation.maxTime = (float)acc.maxValues[0] - joint->rotation.timeStamps[0];
	joint->rotation.count = acc.count;
	/// accessing sampler output for transformation data
	acc = model.accessors[sampler.output];
	bufferView = model.bufferViews[acc.bufferView];
	buffer = model.buffers[bufferView.buffer];
	/// adding the transformation quats
	auto *data1 = (glm::quat *)(buffer.data.data() + bufferView.byteOffset);
	joint->rotation.rotation.insert(joint->rotation.rotation.end(), data1, (data1 + acc.count));
	joint->matrix = glm::mat4();
	joint->trans = glm::mat4();
	joint->rot = glm::mat4();
}

void Animation::_loadTranslationChannel(tinygltf::AnimationChannel &channel, tinygltf::AnimationSampler &sampler, tinygltf::Model &model)
{
	JointAnimation          *joint;
	tinygltf::Buffer        buffer;
	tinygltf::Accessor      acc;
	tinygltf::BufferView    bufferView;

	if (_jointAnimations[channel.target_node] == nullptr)
		_jointAnimations[channel.target_node] = new JointAnimation();
	joint = _jointAnimations[channel.target_node];
	/// accessing sampler input for timestamp
	acc = model.accessors[sampler.input];
	bufferView = model.bufferViews[acc.bufferView];
	buffer = model.buffers[bufferView.buffer];
	/// adding timestamps
	//joint->translation.timeStamps = (float *)buffer.data.data() + bufferView.byteOffset;
	auto *data = (float *)(buffer.data.data() + bufferView.byteOffset);
	joint->translation.timeStamps.insert(joint->translation.timeStamps.end(), data , (data + acc.count));
	/// setting maxTime, subtracting timestamps[0] to make it always have minTime of 0.0f
	joint->translation.maxTime = (float)acc.maxValues[0] - joint->translation.timeStamps[0];
	joint->translation.count = acc.count;
	/// accessing sampler output for transformation data
	acc = model.accessors[sampler.output];
	bufferView = model.bufferViews[acc.bufferView];
	buffer = model.buffers[bufferView.buffer];
	/// adding the transformation vec3s
	auto *data1 = (glm::vec3 *)(buffer.data.data() + bufferView.byteOffset);
	joint->translation.translation.insert(joint->translation.translation.end(), data1, (data1 + acc.count));
	joint->matrix = glm::mat4();
	joint->trans = glm::mat4();
	joint->rot = glm::mat4();
	joint->jointId = channel.target_node;
}

std::vector<size_t> Animation::_getPreviousAndNextFrame(std::vector<float> timeStamps, float maxTime, size_t count)
{
	float currentTime = _currentTimeStamp;
	if (currentTime > maxTime)
		currentTime = fmodf(currentTime, maxTime);

	size_t nextFrame = 0;
	size_t prevFrame = 0;
	for (size_t i = 0; i < count; i++)
	{
		nextFrame = i;
		float t = timeStamps[nextFrame] - timeStamps[0];
		if (t > currentTime)
			break;
		prevFrame = i;
	}
	return {prevFrame, nextFrame};
}

float Animation::_calculateProgressionValue(float nextFrameTime, float prevFrameTime, float maxTime)
{
	float   totalTime = nextFrameTime - prevFrameTime;
	float   currentTime = fmodf(_currentTimeStamp, maxTime) - prevFrameTime;
	return currentTime / totalTime;
}

void Animation::_updateJointAnimation(JointAnimation *joint)
{
	float   progressionVal;
	//std::cout << "joint id : " << joint->jointId << std::endl;
	/// translation transformation
	if (joint->translation.count > 0)
	{
		//std::cout << "  translation" << std::endl;
		auto frames = _getPreviousAndNextFrame(joint->translation.timeStamps, joint->translation.maxTime,
				joint->translation.count);
		progressionVal = _calculateProgressionValue(joint->translation.timeStamps[frames[1]]
				- joint->translation.timeStamps[0],
				joint->translation.timeStamps[frames[0]]
				- joint->translation.timeStamps[0], joint->translation.maxTime);
		glm::vec3 trans = joint->translation.translation[frames[0]] +
			(progressionVal * (joint->translation.translation[frames[1]] -
							   joint->translation.translation[frames[0]]));
		joint->trans = glm::translate(glm::mat4(), trans);
	}
	/// rotation transformation
	if (joint->translation.count > 0)
	{
		//std::cout << "  rotation" << std::endl;
		auto frames1 = _getPreviousAndNextFrame(joint->rotation.timeStamps, joint->rotation.maxTime,
				joint->rotation.count);
		progressionVal = _calculateProgressionValue(joint->rotation.timeStamps[frames1[1]]
				- joint->rotation.timeStamps[0],
				joint->rotation.timeStamps[frames1[0]]
				- joint->rotation.timeStamps[0], joint->rotation.maxTime);
		joint->rot = glm::mat4_cast(glm::normalize(glm::slerp(joint->rotation.rotation[frames1[0]],
						joint->rotation.rotation[frames1[1]], progressionVal)));
		joint->matrix = joint->trans * joint->rot;
	}
}

std::string Animation::getAnimationName() const
{
	return _animationName;
}

JointAnimation* Animation::getJointAnimation(int id)
{
	return _jointAnimations[id];
}

glm::mat4 Animation::getJointAnimationMatrix(int id)
{
	try {
		_jointAnimations.at(id);
	}catch (const std::out_of_range& oor){
		return glm::mat4();
	}
	return _jointAnimations.at(id)->matrix;
}

glm::mat4 Animation::getJointTranslationMatrix(int id)
{
	try {
		_jointAnimations.at(id);
	}catch (const std::out_of_range& oor){
		return glm::mat4();
	}
	return _jointAnimations.at(id)->trans;
}

glm::mat4 Animation::getJointRotationMatrix(int id)
{
	try {
		_jointAnimations.at(id);
	}catch (const std::out_of_range& oor){
		return glm::mat4();
	}
	return _jointAnimations.at(id)->rot;
}

void Animation::increaseCurrentTimeStamp(float val)
{
	_currentTimeStamp += val;
}

void Animation::update()
{
	for (std::pair<int, JointAnimation *> pair : _jointAnimations)
		if (pair.second != nullptr)
			_updateJointAnimation(pair.second);
}

void Animation::setCurrentAnimationTime(float val)
{
	_currentTimeStamp = val;
}

std::vector<float>  Animation::getWeightAnimation(int id)
{
	try {
		_jointAnimations.at(id);
	}catch (const std::out_of_range& oor){
		return {};
	}
	return _jointAnimations.at(id)->weightMorph;
}
