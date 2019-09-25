#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

//ShaderProgram program;
ShaderProgram program1;

glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

bool pause = false;

GLuint playerTextureID;
GLuint playerTextureID2;
GLuint playerTextureID3;

class Player{
public:
	Player(float pos,float speed);
	Player(float pos, float speed, GLuint textureID);
	glm::vec3 getPosition(void);
	glm::vec3 getMovement(void);
	float getSpeed(void);
	void updatePosition(glm::vec3 update);
	void updateMovement(glm::vec3 update);
	void updateSpeed(float speed);
	void Update(float deltaTime);
	void Render(ShaderProgram* program, float vertices[], float texCoords[]);



private:
	glm::vec3 position;
	glm::vec3 movement;
	float speed;
	GLuint textureID;
};

Player::Player(float pos, float speed) {
	this->position = glm::vec3(pos, 0, 0);
	this->movement = glm::vec3(0, 0, 0);
	this->speed = speed;
	this->textureID = NULL;
}

Player::Player(float pos, float speed, GLuint ID) {
	this->position = glm::vec3(pos, 0, 0);
	this->movement = glm::vec3(0, 0, 0);
	this->speed = speed;
	this->textureID = ID;
}

float Player::getSpeed(void) {
	return this->speed;
}

glm::vec3 Player::getPosition(void) {
	return this->position;
}

glm::vec3 Player::getMovement(void) {
	return this->movement;
}

void Player::updatePosition(glm::vec3 update) {
	this->position = update;
}

void Player::updateSpeed(float speed) {
	this->speed = speed;
}

void Player::updateMovement(glm::vec3 update) {
	this->movement = update;
}

void Player::Update(float deltaTime) {
	this->position += this->movement * this->speed * deltaTime;
}

void Player::Render(ShaderProgram* program, float vertices[], float texCoords[]) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, this->position);
	program->SetModelMatrix(modelMatrix);

	if (this->textureID) {
		glBindTexture(GL_TEXTURE_2D,this->textureID);
	}

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	//glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, vertices);
	//glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0 ,6);

	glDisableVertexAttribArray(program1.positionAttribute);
	//glDisableVertexAttribArray(program1.texCoordAttribute);
}


Player player1(1.777f, 0.7f);
Player player2(-1.777f, 0.7f);

void Border(void) {
	float vertices[] = { -2.0f,-.1f, 2.0f,-0.1f, 2.0f,0.1f, -2.0f,-0.1f,2.0f,0.1f,-2.0f,0.1f };
	glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program1.positionAttribute);

	//Up
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
	program1.SetModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Down
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
	program1.SetModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

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

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Ping Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	//program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	program1.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

	playerTextureID = LoadTexture("me.png"); //object 1 untextured
	playerTextureID2 = LoadTexture("bird.png"); //object 2
	playerTextureID3 = LoadTexture("bird.png"); //object 2

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-1.777f, 1.777f, -1.0f, 1.0f, -1.0f, 1.0f);

	//program.SetProjectionMatrix(projectionMatrix);
	//program.SetViewMatrix(viewMatrix);
	//program.SetColor(1.0f, 1.0f, 0.0f, 1.0f);

	program1.SetProjectionMatrix(projectionMatrix);
	program1.SetViewMatrix(viewMatrix);
	program1.SetColor(0.0f, 1.0f, 1.0f, 1.0f);

	//glUseProgram(program.programID);
	glUseProgram(program1.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glClearColor(1.0f, 1.0f,1.0f, 1.0f);
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			gameIsRunning = false;
			break;

		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				if (pause) {
					pause = false;
					break;
				}
				pause = true;
				break;
			}
			break;
		}
	}
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	// ^  |
	// |   ^
	if (keys[SDL_SCANCODE_UP]) {
		glm::vec3 temp = player1.getMovement();
		temp.y += 0.3;
		//temp.x = 1.777;
		player1.updateMovement(temp);
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		glm::vec3 temp = player1.getMovement();
		temp.y += -0.3;
		//temp.x = 1.777;
		player1.updateMovement(temp);
	}
	// W/S to move left
	if (keys[SDL_SCANCODE_W]) {
		glm::vec3 temp = player2.getMovement();
		temp.y += 0.3;
		//temp.x = -1.777;
		player2.updateMovement(temp);
	}
	if (keys[SDL_SCANCODE_S]) {
		glm::vec3 temp = player2.getMovement();
		temp.y += -0.3;
		//temp.x = -1.777;
		player2.updateMovement(temp);
	}
	//norm
	if (glm::length(player1.getMovement()) > 1.00f) {
		player1.updateMovement(glm::normalize(player1.getMovement()));
	}
	if (glm::length(player2.getMovement()) > 1.00f) {
		player2.updateMovement(glm::normalize(player2.getMovement()));
	}
}

float lastTicks = 0;
float rotate_z = 0;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	player1.Update(deltaTime);
	player2.Update(deltaTime);



}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	Border();
	
	float boxes[] = { -0.05f,-0.25f, 0.05f,-0.25f, 0.05f,0.25f, -0.05f,-0.25f, 0.05f,0.25f, -0.05f,0.25f };
	//float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };


	//glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	//glEnableVertexAttribArray(program.positionAttribute);
	//glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	//glEnableVertexAttribArray(program.texCoordAttribute);


	glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, boxes);
	glEnableVertexAttribArray(program1.positionAttribute);

	player1.Render(&program1,boxes,NULL);
	player2.Render(&program1, boxes, NULL);

	float ball[] = { -0.05f,-0.05f, 0.05f,-0.05f, 0.05f,0.05f, -0.05f,-0.05f, 0.05f,0.05f, -0.05f,0.05f };
	glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, ball);
	glEnableVertexAttribArray(program1.positionAttribute);
	
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	program1.SetModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program1.positionAttribute);

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