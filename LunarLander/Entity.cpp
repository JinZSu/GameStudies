#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
	this->velocity = glm::vec3(0);
	this->acceleration = glm::vec3(0);
	this->gravity = -2.37;
}

void Entity::Input()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_UP]) {
		this->velocity.y = 3.21;
	}
	if (keys[SDL_SCANCODE_LEFT]) {
		this->velocity -= 1;
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
		this->velocity += 1;
	}
}	

void Entity::Update(float deltaTime)
{

    this->velocity += this->acceleration* deltaTime;
}


bool Entity::checkCollision(Entity other) {
	float xdist = fabs(position.x - other.position.x) - (width + other.width) / 2.0f;
	float ydist = fabs(position.y - other.position.y) - (height + other.height) / 2.0f;

	if (xdist < 0 && ydist < 0) {
		return true;
	}
	return false;
}

void Entity::Render(ShaderProgram *program) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
