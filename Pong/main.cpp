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

ShaderProgram program;
ShaderProgram program1;

glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

bool pause = false;

GLuint playerTextureID;
GLuint playerTextureID2;
GLuint playerTextureID3;

class Player{
public:
	Player(float pos,float height, float width, float speed);
	Player(float pos, float height, float width, float movement, float speed);
	Player(float pos, float speed, GLuint textureID);
	glm::vec3 getPosition(void);
	glm::vec3 getMovement(void);
	float getHeight(void);
	float getWidth(void);
	void updateHeight(float x);
	void updateWidth(float x);
	float getSpeed(void);
	void updatePosition(glm::vec3 update);
	void updateMovement(glm::vec3 update);
	void updateSpeed(float speed);
	void Update(float deltaTime, float height, float bottom);
	void Render(ShaderProgram* program, float vertices[], float texCoords[]);

private:
	glm::vec3 position;
	glm::vec3 movement;
	float speed;
	float height;
	float width;
	GLuint textureID;
};

Player::Player(float pos, float height, float width, float speed) {
	this->position = glm::vec3(pos, 0, 0);
	this->movement = glm::vec3(0, 0, 0);
	this->speed = speed;
	this->height = height;
	this->width = width;
	this->textureID = NULL;
}
Player::Player(float pos, float height, float width, float movement,float speed) {
	this->position = glm::vec3(pos, 0, 0);
	this->movement = glm::vec3(movement, movement, 0);
	this->speed = speed;
	this->height = height;
	this->width = width;
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
float Player::getHeight(void) {
	return this->height;
}
float Player::getWidth(void) {
	return this->width;
}
glm::vec3 Player::getPosition(void) {
	return this->position;
}
glm::vec3 Player::getMovement(void) {
	return this->movement;
}
void Player::updateHeight(float x) {
	this->height = x;
}
void Player::updateWidth(float x) {
	this->width = x;
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

void Player::Update(float deltaTime,float height, float bottom) {
	if (this->position.y <= height and this->position.y >= bottom) {
		glm::vec3 temp = this->movement * this->speed * deltaTime;
		if (temp.y + this->position.y > height) {
			this->position.y = height;
		}
		else if (temp.y + this->position.y < bottom) {
			this->position.y = bottom;
		}
		else {
			this->position += temp;
		}
	}

}

void Player::Render(ShaderProgram* program, float vertices[], float texCoords[]) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, this->position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(this->width,this-> height, 1.0f));
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


Player player1(-3.0f, 0.42f, 0.3f, 1.0f);
Player player2(3.0f, 0.42f, 0.3f, 1.0f);
Player ball(0.0f, 0.1f, 0.15f, 0.9f ,1.7f);

void celebration(int g) {
	glClear(GL_COLOR_BUFFER_BIT);
	float leftx = -3.0;
	float rightx = 3.0f;
	float topy = -1.0f;
	float boty = 1.0f;
	float vertices[] = { leftx, boty, rightx, boty, rightx , topy, leftx, boty, rightx, topy,leftx ,topy };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };


	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);
	modelMatrix = glm::mat4(1.0f);
	if (g == 1) {
		glBindTexture(GL_TEXTURE_2D, playerTextureID);
		program.SetModelMatrix(modelMatrix);//not texture
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, playerTextureID2);
		program.SetModelMatrix(modelMatrix);//not texture
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);

	SDL_GL_SwapWindow(displayWindow);
	while (pause and gameIsRunning) {
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
					break;
				}
				break;
			}
		}
	}
	pause = true;
	glClear(GL_COLOR_BUFFER_BIT);


};

void Border(void) {
	float leftx = -3.0;
	float rightx = 3.0f;
	float topy = -1.0f;
	float boty = 1.0f;
	float vertices[] = { leftx, boty, rightx, boty, rightx , topy, leftx, boty, rightx, topy,leftx ,topy };
	glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program1.positionAttribute);

	//Up
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(leftx, boty, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.9f, 0.1f, 1.0f));
	program1.SetModelMatrix(modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Down
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(leftx, topy, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.9f, 0.1f, 1.0f));
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

	glViewport(0, 0, 640, 360);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	program1.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

	playerTextureID = LoadTexture("me.png"); //object 1 untextured
	playerTextureID2 = LoadTexture("bird.png"); //object 2
	//playerTextureID3 = LoadTexture("bird.png"); //object 2

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-3.0f, 3.0f, -1.0f, 1.0f, -1.0f, 1.0f); //box side

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 0.0f, 1.0f);

	program1.SetProjectionMatrix(projectionMatrix);
	program1.SetViewMatrix(viewMatrix);
	program1.SetColor(0.0f, 1.0f, 1.0f, 1.0f);

	glUseProgram(program.programID);
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
		temp.y += 0.03;
		//temp.x = 1.777;
		player1.updateMovement(temp);
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		glm::vec3 temp = player1.getMovement();
		temp.y += -0.03;
		//temp.x = 1.777;
		player1.updateMovement(temp);
	}
	// W/S to move left
	if (keys[SDL_SCANCODE_W]) {
		glm::vec3 temp = player2.getMovement();
		temp.y += 0.03;
		//temp.x = -1.777;
		player2.updateMovement(temp);
	}
	if (keys[SDL_SCANCODE_S]) {
		glm::vec3 temp = player2.getMovement();
		temp.y += -0.03;
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

	//Ball
}
// 55041-OEM-8992671-00437

float lastTicks = 0;
float rotate_z = 0;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;
	float top = 1.0f - player1.getHeight()/2.0f - 0.1f;
	float top2 = 1.0f - player2.getHeight()/2.0f - 0.1f;
	player1.Update(deltaTime, top, -1 * top);
	player2.Update(deltaTime, top2, -1 * top2);
	float bottom = 0.05f;
	//top bottom
	if ((1.0f - abs(ball.getPosition().y) - ball.getHeight() <= 0.05f)) {
		glm::vec3 temp = ball.getMovement();
		temp.y *= -1.0f;
		ball.updateMovement(temp);
	}
	//right side
	float left = fabs(player1.getPosition().x - ball.getPosition().x) - ((player1.getWidth() + ball.getWidth()) / 2.0f);
	float uleft = fabs(player1.getPosition().y - ball.getPosition().y) - ((player1.getHeight() + ball.getHeight()) / 2.0f);
	float right = fabs(player2.getPosition().x - ball.getPosition().x) - ((player2.getWidth() + ball.getWidth()) / 2.0f);
	float uright = fabs(player2.getPosition().y - ball.getPosition().y) - ((player2.getHeight() + ball.getHeight()) / 2.0f);


	if (left <= 0.0f && uleft <= 0.0f) {
		glm::vec3 temp = ball.getMovement();
		temp.x *= -1.0f;
		ball.updateMovement(temp);
	}
	else if (right <= 0.0f && uright <= 0.0f) {
		glm::vec3 temp = ball.getMovement();
		temp.x *= -1.0f;
		ball.updateMovement(temp);

	}
	else if (ball.getPosition().x - ball.getWidth() - player1.getWidth() < -3.5f ) { //game over
		celebration(1);
		glm::vec3 temp = ball.getMovement();
		temp.y = 0.9f;
		temp.x = 0.9f;
		ball.updateMovement(temp);
		temp.y = 0.0f;
		temp.x = 0.0f;
		ball.updatePosition(temp);

	}
	else if (ball.getPosition().x + ball.getWidth() + player2.getWidth() > 3.5f) { //game over
		celebration(2);
		glm::vec3 temp = ball.getMovement();
		temp.y = 0.9f;
		temp.x = 0.9f;
		ball.updateMovement(temp);
		temp.y = 0.0f;
		temp.x = 0.0f;
		ball.updatePosition(temp);

	}
	/*
	if ((abs(ball.getPosition().y - player1.getPosition().y) <= player1.getHeight() + ball.getHeight())  && (abs(ball.getPosition().x - player1.getPosition().x) <= ball.getWidth() + player1.getWidth())) {
		glm::vec3 temp = ball.getMovement();
		temp.x *= -1.0f;
		ball.updateMovement(temp);
	}
	//left side
	else if ((abs(ball.getPosition().y  - player2.getPosition().y) <=  player2.getHeight() + ball.getHeight()) && (abs(ball.getPosition().x - player2.getPosition().x) <=  player2.getWidth() + ball.getWidth())) {
		glm::vec3 temp = ball.getMovement();
		temp.x *= -1.0f;
		ball.updateMovement(temp);
	}
	//pass
	else if (ball.getPosition().x - ball.getWidth() - player1.getWidth() < -3.0f || ball.getPosition().x  + ball.getWidth() + player2.getWidth() > 3.0f) { //game over
		glm::vec3 temp = ball.getMovement();
		temp.y = 0.9f;
		temp.x = 0.9f;
		ball.updateMovement(temp);
		temp.y = 0.0f;
		temp.x = 0.0f;
		ball.updatePosition(temp);
	*/
	/*
	}
	else if ((abs(ball.getPosition().x - player1.getPosition().x) < ball.getWidth() + player1.getWidth()) || (abs(ball.getPosition().x - player2.getPosition().x) <= player2.getWidth() + ball.getWidth())) {
		glm::vec3 temp = ball.getMovement();
		temp.y *= -1.0f;
		ball.updateMovement(temp);
	}*/
	ball.Update(deltaTime, 1.0f - ball.getHeight(), -1.0f + ball.getHeight());
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	Border();
	float leftx = -3.0f;
	float rightx = 3.0f;
	float topy = -1.0f;
	float boty = 1.0f;

	
	//float boxes[] = { leftx, boty, rightx, boty, rightx , topy, leftx, boty, rightx, topy,leftx ,topy };
	float boxes[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };


	//glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	//glEnableVertexAttribArray(program.positionAttribute);
	//glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	//glEnableVertexAttribArray(program.texCoordAttribute);


	glVertexAttribPointer(program1.positionAttribute, 2, GL_FLOAT, false, 0, boxes);
	glEnableVertexAttribArray(program1.positionAttribute);

	player1.Render(&program1,boxes,NULL);
	player2.Render(&program1, boxes, NULL);

	//float ballpos[] = { -0.05f,-0.05f, 0.05f,-0.05f, 0.05f,0.05f, -0.05f,-0.05f, 0.05f,0.05f, -0.05f,0.05f };

	ball.Render(&program1, boxes, NULL);

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