Entity::Entity()
{
	position = glm::vec3(0);
	this->velocity = glm::vec3(0);
	this->acceleration = glm::vec3(0);
	this->gravity = -2.37f;
	this->height = 0.0f;
	this->width = 0.0f;
	this->landed = false;
	this->thrust = false;
	this->circle = 0.0f;
	this->speed = 0.0f;
}

void Entity::checkCollisionsY(Entity* objects, int objectcount)
{
	for (int i = 0; i < objectcount; i++)
	{
		Entity object = objects[i];
		if (checkCollision(object))
		{
			float ydist = fabs(this->position.y - object.position.y);
			float penetrationY = fabs(ydist - (this->height / 2) - (object.height / 2));
			if (this->velocity.y > 0)
			{
				this->position.y -= penetrationY;
				this->velocity.y = 0;
			}
			else if (this->velocity.y < 0)
			{
				this->position.y += penetrationY;
				this->velocity.y = 0;
			}
		}
	}
}

void Entity::checkCollisionsX(Entity* objects, int objectcount)
{
	for (int i = 0; i < objectcount; i++)
	{
		Entity object = objects[i];
		if (checkCollision(object))
		{
			float xdist = fabs(this->position.x - object.position.x);
			float penetrationX = fabs(xdist - (this->width / 2) - (object.width / 2));
			if (this->velocity.x > 0)
			{
				this->position.x -= penetrationX;
				this->velocity.x = 0;
			}
			else if (this->velocity.x < 0)
			{
				this->position.x += penetrationX;
				this->velocity.x = 0;
			}
		}
	}
}

void Entity::Update(float deltaTime, Entity* objects, int objectcount)
{
	this->velocity += this->acceleration * deltaTime;
	this->position.y += this->velocity.y * deltaTime;
	checkCollisionsY(objects, objectcount);
	this->position.x += this->velocity.x * deltaTime;
	checkCollisionsX(objects, objectcount);
}


bool Entity::checkCollision(Entity other) {
	float xdist = fabs(position.x - other.position.x) - (width + other.width) / 2.0f;
	float ydist = fabs(position.y - other.position.y) - (height + other.height) / 2.0f;

	if (xdist < 0 && ydist < 0) {
		return true;
	}
	return false;
}

void Entity::Render(ShaderProgram* program) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
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
