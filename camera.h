#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

class Camera {
	public:
		Camera() {}
		
		Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar) {
			perspective = glm::perspective(fov, aspect, zNear, zFar);
			position = pos;
			forward = glm::vec3(0,0,1);
			up = glm::vec3(0,1,0);
		}
		
		void Set(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar) {
			perspective = glm::perspective(fov, aspect, zNear, zFar);
			position = pos;
			forward = glm::vec3(0,0,1);
			up = glm::vec3(0,1,0);
		}
		
		inline glm::mat4 GetViewProjection() const {
			return perspective * glm::lookAt(position, position + forward, up);
		}
		
		inline glm::vec3& GetPos() { return position; }
		inline glm::vec3& GetForward() { return forward; }
		inline glm::vec3& GetUp() { return up; }
		inline glm::mat4& GetPerspective() { return perspective; }
				
		inline void SetPos(glm::vec3& pos) { this->position = pos; }
		inline void SetForward(glm::vec3& forward) { this->forward = forward; }
		inline void SetUp(glm::vec3& up) { this->up = up; }
		inline void SetPerspective(float fov, float aspect, float zNear, float zFar) {
			this->perspective = glm::perspective(fov, aspect, zNear, zFar);
		}
		
	protected:
	private:
		glm::mat4 perspective;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 up;
};

#endif
