#pragma once

#include "tiny_gltf.h"
#include <map>
#include <GL/glew.h>
#include <iostream>
#include "../GraphicsEngine/glm/glm.hpp"
#include "../GraphicsEngine/glm/common.hpp"
#include "../GraphicsEngine/glm/gtc/matrix_transform.hpp"
#include "../GraphicsEngine/glm/gtx/quaternion.hpp"
#include "../GraphicsEngine/glm/gtc/matrix_transform.hpp"
#include "../GraphicsEngine/glm/gtc/type_ptr.hpp"


struct  KeyFrames
{
	std::vector<float>       timeStamps;
	float       minTime = 0.0f;
	float       maxTime;
	size_t      count;
	std::vector<glm::vec3>   translation;
	std::vector<glm::quat>   rotation;
};
struct  JointAnimation
{
	int         jointId;
	glm::mat4   trans;
	glm::mat4   rot;
	glm::mat4   matrix;
	std::vector<float>   weightMorph;
	KeyFrames   translation;
	KeyFrames   rotation;
	KeyFrames   weight;
};

class   Animation
{
	private:
		float           _currentTimeStamp;
		std::string     _animationName;
		std::map<int, JointAnimation *>   _jointAnimations;
	private:
		bool    _loadAnimation(tinygltf::Animation& animation, tinygltf::Model& model);
		void    _loadRotationChannel(tinygltf::AnimationChannel& channel, tinygltf::AnimationSampler& sampler, tinygltf::Model &model);
		void    _loadTranslationChannel(tinygltf::AnimationChannel& channel, tinygltf::AnimationSampler& sampler, tinygltf::Model &model);
		std::vector<size_t>  _getPreviousAndNextFrame(std::vector<float> timeStamps, float maxTime, size_t count);
		void    _updateJointAnimation(JointAnimation *joint);
		float   _calculateProgressionValue(float nextFrameTime, float prevFrameTime, float maxTime);
	public:
		Animation(tinygltf::Animation& animation, tinygltf::Model& model);
		Animation(const Animation & rhs);
		Animation&  operator=(const Animation & rhs);
		~Animation();

		std::string     getAnimationName() const;
		JointAnimation  *getJointAnimation(int id);
		glm::mat4       getJointAnimationMatrix(int id);
		glm::mat4       getJointTranslationMatrix(int id);
		glm::mat4       getJointRotationMatrix(int id);
		void            increaseCurrentTimeStamp(float val);
		void            update();
		void            setCurrentAnimationTime(float val);
		std::vector<float>       getWeightAnimation(int id);
};
