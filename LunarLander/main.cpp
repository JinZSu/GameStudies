#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLuint LoadTexture(const char* filePath) {
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(image);
	return textureID;
}

class Entity {
public:
	float height;
	float width;
	float circleDirection;
	float circle;
	float circleVelocity;
	float ss;
	float otherss;

	float speed;
	float gravity;
	float thrust;
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
	void TRender(ShaderProgram* program);
};


Entity::Entity()
{
	position = glm::vec3(0);
	this->velocity = glm::vec3(0);
	this->acceleration = glm::vec3(0);
	this->gravity = 0.45f;
	this->height = 0.0f;
	this->width = 0.0f;
	this->landed = false;
	this->thrust = 0.0f;
	this->circleDirection = 0.0f;
	this->circle = 0.0f;
	this->circleVelocity = glm::radians(75.0f);

}

void Entity::checkCollisionsY(Entity* objects, int objectcount)
{
	for (int i = 0; i < objectcount; i++)
	{
		Entity object = objects[i];
		if (checkCollision(object))
		{
			this->landed = true;
			this->otherss = object.ss;
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
	float x = glm::sin(this->circle) * thrust;
	float y = glm::cos(this->circle) * thrust;
	this->acceleration += glm::vec3(-1 * x, y - this->gravity, 0.0f);
	this->velocity += this->acceleration * deltaTime;
	this->position.y += this->velocity.y * deltaTime;
	checkCollisionsY(objects, objectcount);
	this->position.x += this->velocity.x * deltaTime;
	checkCollisionsX(objects, objectcount);
	this->circle += circleDirection * (circleVelocity * deltaTime);
	this->acceleration = glm::vec3(0.0f);
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
	if (!landed) {
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, this->circle, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 1.0f));
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
	else {
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 9.0f, 1.0f));
		program->SetModelMatrix(modelMatrix);
		float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
		if (this->otherss == 2)
			this->textureID = LoadTexture("win.png");
		else
			this->textureID = LoadTexture("lose.png");
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);

		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program->positionAttribute);

		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program->texCoordAttribute);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);
	}
}
void Entity::TRender(ShaderProgram* program) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, this->circle, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 1.0f));
	program->SetModelMatrix(modelMatrix);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Entity player;
Entity Mountain;
Entity Mountain1;
Entity Bed;
Entity Mountain2;

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Physics!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	player.textureID = LoadTexture("duck.png"); //load text 
	player.position = glm::vec3(0.0f,4.20f,1.0f);
	player.height = 1.0f;
	player.width = 2.0f;
	player.ss = 1;

	Bed.textureID = LoadTexture("bed.png");
	Bed.height = 1.0f;
	Bed.width = 3.0f;
	Bed.position = glm::vec3(0.0f, -3.0f, 1.0f);
	Bed.ss = 2;

	Mountain.textureID = LoadTexture("Mountain.png"); //load text 
	Mountain.position = glm::vec3(4.0f, -3.0f, 1.0f);
	Mountain.height = 4.0f;
	Mountain.width = 4.0f;
	Mountain.ss = 3;

	Mountain1.textureID = LoadTexture("Mountain.png"); //load text 
	Mountain1.position = glm::vec3(-4.0f, -3.5f, 1.0f);
	Mountain1.height = 4.0f;
	Mountain1.width = 4.0f;
	Mountain1.ss = 3;

	Mountain2.textureID = LoadTexture("Mountain.png"); //load text 
	Mountain2.position = glm::vec3(-4.0f, 3.0f, 1.0f);
	Mountain2.height = 4.0f;
	Mountain2.width = 4.0f;
	Mountain2.ss = 3;

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	glUseProgram(program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				player.thrust = 1.30f;
				break;
			case SDLK_LEFT:
				player.circle = 1.0f;
				break;
			case SDLK_RIGHT:
				player.circle = -1.0f;
				break;

			}
			break;

		}
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		if (!keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT]) {
			player.circle = 0.0f;
		}
		if (!keys[SDL_SCANCODE_UP]) {
			player.thrust = 0.3f;
		}
	}
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {
		// Update. Notice it's FIXED_TIMESTEP. Not deltaTime
		player.Update(FIXED_TIMESTEP, &Mountain, 1);
		player.Update(FIXED_TIMESTEP, &Bed, 1);
		player.Update(FIXED_TIMESTEP, &Mountain1, 1);
		player.Update(FIXED_TIMESTEP, &Mountain2, 1);

		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;
}


void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	player.Render(&program);
	Mountain.Render(&program);
	Mountain1.Render(&program);
	Mountain2.Render(&program);
	Bed.Render(&program);
	SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		ProcessInput();
		Update();
		Render();
	}

	Shutdown();
	return 0;
}