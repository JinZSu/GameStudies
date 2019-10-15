#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdbool.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"


class Entity {
public:
	float height;
	float width;
	float circle;
	float speed;
	float gravity;

	bool thrust;
	bool landed;

	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	GLuint textureID;


	Entity();
	bool checkCollision(Entity other);
	void Update(float deltaTime, Entity* objects, int objectcount);
	void checkCollisionsY(Entity* objects, int objectcount);
	void checkCollisionsX(Entity* objects, int objectcount);
	void Render(ShaderProgram* program);
};