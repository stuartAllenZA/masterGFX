#pragma once

#include "../GraphicsEngine/glm/glm.hpp"
#include "../GraphicsEngine/glm/gtc/matrix_transform.hpp"

class   Camera
{
	private:
		glm::vec3   _position;
		glm::vec3   _front;
		glm::vec3   _right;
		glm::vec3   _worldUp;
		glm::vec3   _target;
		glm::vec3   Up;
		float       _pitch;
		float       _yaw;
		float       _roll;
		glm::mat4   _viewMatrix;
	private:
		void        _updateCamera();
	public:
		Camera(glm::vec3 position = glm::vec3(0, 0, 0), float pitch = 0, float yaw = -90.0f);
		Camera(const Camera & rhs);
		Camera&     operator=(const Camera& rhs);
		~Camera() = default;

		glm::mat4       getViewMatrix() const;
		glm::vec3       getCameraPosition();
		void            moveLeft(float val, float);
		void            moveRight(float val, float);
		void            moveUp(float val, float);
		void            moveDown(float val, float);
		void            moveForward(float val, float);
		void            moveBackward(float val, float);
		void            changeCameraXPos(float x, float);
		void            changeCameraYPos(float y, float);
		void            changeCameraZPos(float z, float);
		void            setCameraPosition(glm::vec3 pos);
		void            setCameraTarget(glm::vec3 target);
		void            setCameraUp(glm::vec3 up);
};
