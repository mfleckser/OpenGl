#include "player.h"
#include <iostream>

Player::Player(glm::vec3 pos) {
	this->pos = pos;
	this->vel = glm::vec3(0, 0, 0);
	this->acc = glm::vec3(0, 0, 0);
	this->looking = glm::vec3(0, 0, 1);
	this->yaw = atan2(1, 0);
	this->pitch = 0;
}

Player::~Player() {
	
}

void Player::updateCam() {
	camera.GetPos() = pos;
	camera.GetForward() = looking;
}

void Player::update(int dTime) {
	vel += acc * ((float)dTime / 10);
	this->move(dTime);
	updateCam();
}

void Player::move(int dTime) {
	glm::vec3 temp = glm::normalize(looking);
	temp.y = 0;
	pos += vel.z * temp * ((float)dTime / 10) * 0.05f;
	pos += glm::normalize(glm::cross(temp, glm::vec3(0, 1, 0))) * vel.x * ((float)dTime / 10) * 0.05f;
}
