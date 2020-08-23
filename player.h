#ifndef PLAYER_H
#define PLAYER_H

#include "camera.h"

class Player {
	public:
		Player(glm::vec3 pos);
		
		inline glm::vec3& GetPos() { return pos; }
		inline glm::vec3& GetVel() { return vel; }
		inline glm::vec3& GetAcc() { return acc; }
		inline glm::vec3& GetLooking() { return looking; }
		inline float& GetYaw() { return yaw; }
		inline float& GetPitch() { return pitch; }
		inline Camera& GetCam() { return camera; }
		
		inline void SetCam(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar) { camera.Set(pos, fov, aspect, zNear, zFar); }
		
		void updateCam();
		void update(int dTime);
		void move(int dTime);
		
		~Player();
	protected:
	private:
		glm::vec3 pos;
		glm::vec3 vel;
		glm::vec3 acc;
		glm::vec3 looking;
		Camera camera;
		float yaw;
		float pitch;
};

#endif
