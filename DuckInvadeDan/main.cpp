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

#include "Entity.h"
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

Entity::Entity()
{
	entityType = PLATFORM;
	isStatic = true;
	isActive = true;
	killed = false;
	position = glm::vec3(0);
	speed = 0;
	width = 1;
	height = 1;
}

bool Entity::CheckCollision(Entity other, int num)
{
	if (killed == true) return false;
	if (isActive == false || other.isActive == false) return false;

	float xdist = fabs(position.x - other.position.x) - ((width + other.width) / 2.0f);
	float ydist = fabs(position.y - other.position.y) - ((height + other.height) / 2.0f);

	if (xdist < num && ydist < num)
	{

		return true;
	}

	return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity object = objects[i];

		if (CheckCollision(object,0) && object.entityType == PLATFORM)
		{
			float ydist = fabs(position.y - object.position.y);
			float penetrationY = fabs(ydist - (height / 2) - (object.height / 2));
			if (velocity.y > 0) {
				position.y -= penetrationY;
				velocity.y = 0;
				collidedTop = true;
			}
			else if (velocity.y < 0) {
				position.y += penetrationY;
				velocity.y = 0;
				collidedBottom = true;
			}
		}
		else if (CheckCollision(object, 0) && object.entityType != PLATFORM && entityType != PLAYER) {
			float ydist = fabs(position.y - object.position.y);
			float penetrationY = fabs(ydist - (height / 2) - (object.height / 2));
			if (velocity.y > 0) {
				position.y -= penetrationY;
				velocity.y = 0;
				collidedTop = true;
				this->isActive = false;
				this->killed = true;
				this->isStatic = true;
			}
			else if (velocity.y < 0) {
				position.y += penetrationY;
				velocity.y = 0;
				collidedBottom = true;
				this->isActive = false;
				this->killed = true;
				this->isStatic = true;
			}
		}
	}
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity object = objects[i];
		if (CheckCollision(object, 0) && object.entityType == PLATFORM)
			{
				float xdist = fabs(position.x - object.position.x);
				float penetrationX = fabs(xdist - (width / 2) - (object.width / 2));
				if (velocity.x > 0) {
					position.x -= penetrationX;
					velocity.x = 0;
					collidedRight = true;
				}
				else if (velocity.x < 0) {
					position.x += penetrationX;
					velocity.x = 0;
					collidedLeft = true;
				}
			}
		else if (CheckCollision(object, 0) && object.entityType != PLATFORM){
			float xdist = fabs(position.x - object.position.x);
			float penetrationX = fabs(xdist - (width / 2) - (object.width / 2));
			if (velocity.x > 0) {
				position.x -= penetrationX;
				velocity.x = 0;
				collidedRight = true;
				this->isActive = false;
				this->killed = true;
				this->isStatic = true;
				object.isActive = false;
				object.killed = true;
				object.isStatic = true;
			}
			else if (velocity.x < 0) {
				position.x += penetrationX;
				velocity.x = 0;
				collidedLeft = true;
				this->isActive = false;
				this->killed = true;
				this->isStatic = true;
				object.isActive = false;
				object.killed = true;
				object.isStatic = true;
			}
		}
		if (CheckCollision(object, 1) && (object.entityType != PLATFORM)) {
			object.isStatic = false;
		}
	}
}


void Entity::Jump()
{
	if (collidedBottom)
	{
		velocity.y = 6.0f;
	}
}




void Entity::Update(float deltaTime, Entity* objects, int objectCount)
{
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	if (killed) {
		position = glm::vec3(-9.0f,-9.0f,1.0f);
	}

	if (this->entityType == PLAYER && objects[0].entityType == PLATFORM) {
		velocity += acceleration * deltaTime;

		position.y += velocity.y * deltaTime;        // Move on Y
		CheckCollisionsY(objects, objectCount);    // Fix if needed

		position.x += velocity.x * deltaTime;        // Move on X
		CheckCollisionsX(objects, objectCount);    // Fix if needed
	}
	else if (this->entityType == PLAYER && objects[0].entityType != PLATFORM) {
		velocity += acceleration * deltaTime;
		position.y += velocity.y * deltaTime;        // Move on Y
		CheckCollisionsY(objects, objectCount);    // Fix if needed

		position.x += velocity.x * deltaTime;        // Move on X
		CheckCollisionsX(objects, objectCount);    // Fix if needed
	}
	if (this->entityType == ENEMY1 && objects[0].entityType == PLATFORM) {
		velocity += acceleration * deltaTime;
		position.y += velocity.y * deltaTime;        // Move on Y
		CheckCollisionsY(objects, objectCount);    // Fix if needed

		position.x += velocity.x * deltaTime;        // Move on X
		CheckCollisionsX(objects, objectCount);    // Fix if needed
	}
	if (this->entityType == ENEMY2 && objects[0].entityType == PLATFORM) {
		velocity += acceleration * deltaTime;
		position.y += velocity.y * deltaTime;        // Move on Y
		CheckCollisionsY(objects, objectCount);    // Fix if needed

		position.x += velocity.x * deltaTime;        // Move on X
		CheckCollisionsX(objects, objectCount);    // Fix if needed
	}
	if (this->entityType == ENEMY3 && objects[0].entityType == PLATFORM) {
		velocity += acceleration * deltaTime;
		position.y += velocity.y * deltaTime;        // Move on Y
		CheckCollisionsY(objects, objectCount);    // Fix if needed

		position.x += velocity.x * deltaTime;        // Move on X
		CheckCollisionsX(objects, objectCount);    // Fix if needed
	}
}



void Entity::Render(ShaderProgram* program) {
	if (!this->killed) {
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
}



SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

#define PLATFORM_COUNT 20

struct GameState {
    Entity player;
	Entity enemy[3];
    Entity platforms[PLATFORM_COUNT];
};

GameState state;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("AI!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
	state.player.killed = false;
    state.player.width = 1.0f;
    state.player.position = glm::vec3(-4, 3, 0);
    state.player.acceleration = glm::vec3(0, -9.81f, 0);
    state.player.textureID = LoadTexture("duck.png");


	for (int i = 0; i < 3; i++)
	{
		if (i == 2) {
			state.enemy[i].entityType = ENEMY1;
			state.enemy[i].isStatic = true;
			state.enemy[i].width = 1.0f;
			state.enemy[i].position = glm::vec3(i, -4 + i, 0);
			state.enemy[i].acceleration = glm::vec3(0, -9.81f, 0);
			state.enemy[i].textureID = LoadTexture("enemy.png");
		}
		else {
			state.enemy[i].entityType = ENEMY1;
			state.enemy[i].isStatic = true;
			state.enemy[i].width = 1.0f;
			state.enemy[i].position = glm::vec3(i, 1 + i, 0);
			state.enemy[i].acceleration = glm::vec3(0, -9.81f, 0);
			state.enemy[i].textureID = LoadTexture("enemy.png");
		}
	}


    
    GLuint dirtTextureID = LoadTexture("Dan.png");
    GLuint grassTextureID = LoadTexture("Dan.png");
    
    for (int i = 0; i < 11; i++)
    {
        state.platforms[i].textureID = dirtTextureID;
        state.platforms[i].position = glm::vec3(i - 5.0f, -3.25f, 0);
		state.platforms[i].entityType = PLATFORM;
    }
    
    state.platforms[11].textureID = grassTextureID;
    state.platforms[11].position = glm::vec3(-5.0f, -2.25f, 0);
	state.platforms[11].entityType = PLATFORM;
    
    state.platforms[12].textureID = grassTextureID;
    state.platforms[12].position = glm::vec3(-4.0f, -2.25f, 0);
	state.platforms[12].entityType = PLATFORM;

	state.platforms[13].textureID = grassTextureID;
	state.platforms[13].position = glm::vec3(-4.0f, -1.25f, 0);
	state.platforms[13].entityType = PLATFORM;

	for (int i = 14, j =1; i < 21; i++,j++)
	{
		state.platforms[i].textureID = dirtTextureID;
		state.platforms[i].position = glm::vec3(j - 4.0f, -0.25f, 0);
		state.platforms[i].entityType = PLATFORM;
	}



    
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
                    case SDLK_SPACE:
                        state.player.Jump();
                        break;
                        
                }
                break;
        }
    }
    
    state.player.velocity.x = 0;
    
    // Check for pressed/held keys below
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_A])
    {
        state.player.velocity.x = -3.0f;
    }
    else if  (keys[SDL_SCANCODE_D])
    {
        state.player.velocity.x = 3.0f;
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
		state.player.Update(FIXED_TIMESTEP, state.enemy, 3);
		for (int i = 0; i < 3; i++)
		{
			state.enemy[i].Update(FIXED_TIMESTEP, &state.player, 1);
			state.enemy[i].Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
		}
		state.player.Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    state.player.Render(&program);
    
	for (int i = 0; i < 3; i++)
	{
		state.enemy[i].Render(&program);
	}

	for (int i = 0; i < PLATFORM_COUNT; i++)
	{
		state.platforms[i].Render(&program);
	}
    
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
