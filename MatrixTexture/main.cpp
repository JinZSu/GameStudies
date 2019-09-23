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
glm::mat4 viewMatrix, modelMatrix, modelMatrix1, modelMatrix2, modelMatrix3, modelMatrix4, modelMatrix5, modelMatrix6, projectionMatrix;

float player_x = 0;
float player_y = 0;

GLuint playerTextureID;
GLuint playerTextureID2;
GLuint playerTextureID3;
GLuint playerTextureID4;
GLuint playerTextureID5;

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
	displayWindow = SDL_CreateWindow("Homework 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	//playerTextureID = LoadTexture("me.png"); //object 1 untextured
	playerTextureID2 = LoadTexture("bird.png"); //object 2
	playerTextureID4 = LoadTexture("bird.png"); //object 2
	playerTextureID5 = LoadTexture("bird.png"); //object 2
	playerTextureID3 = LoadTexture("duck.png"); //object 3

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	modelMatrix1 = glm::mat3(1.0f);//object 1 matrix
	modelMatrix2 = glm::mat4(1.0f); //object 2 matrix
	modelMatrix3 = glm::mat4(1.0f); //object 3 matrix
	modelMatrix4 = glm::mat4(1.0f);
	modelMatrix5 = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	program.SetColor(1.0f, 1.0f, 0.0f, 1.0f);

	glUseProgram(program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glClearColor(0.3f, 0.3f, 0.0f, 0.21f);
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			gameIsRunning = false;
		}
	}
}

float lastTicks = 0;
float rotate_z = 0;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	player_x += 1.0f * deltaTime;
	player_y -= 1.0f * deltaTime;
	rotate_z += 5.0f * deltaTime;
	if (player_y < -3.0f) {
		player_y = 0.0f;
	}

	//Object 1 station
	
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix,
		glm::radians(rotate_z),
		glm::vec3(0.0f, 0.0f, 1.0f));

	modelMatrix1 = glm::mat3(1.0f);
	modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(-3.0f, -1.0f, -1.0f));

	//Object 2 object movement
	modelMatrix2 = glm::mat4(1.0f);
	modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(-3.0f, 1.0f, -1.0f));
	modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(0.9f, 0.9f, 1.0f));
	modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(player_x, 0.0f, 0.0f));

	modelMatrix4 = glm::mat4(1.0f);
	modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(-3.0f, 3.0f, 0.0f));
	modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.9f, 0.9f, 1.0f));
	modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(player_x, 0.0f, 0.0f));

	modelMatrix5 = glm::mat4(1.0f);
	modelMatrix5 = glm::translate(modelMatrix5, glm::vec3(-3.0f, 2.0f, 0.0f));
	modelMatrix5 = glm::scale(modelMatrix5, glm::vec3(0.9f, 0.9f, 1.0f));
	modelMatrix3 = glm::rotate(modelMatrix5,
		glm::radians(rotate_z),
		glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix5 = glm::translate(modelMatrix5, glm::vec3(player_x, 0.0f, 0.0f));

	//Object 3 object movement

	modelMatrix3 = glm::mat4(1.0f);
	modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(5.0f, -2.0f, 0.0f));
	modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(3.9f, 3.9f, 1.0f));
	modelMatrix3 = glm::rotate(modelMatrix3,
		glm::radians(rotate_z),
		glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(-player_y, 0.0f, 0.0f));

	modelMatrix6 = glm::mat4(1.0f);
	modelMatrix6 = glm::translate(modelMatrix6, glm::vec3(3.0f, -2.0f, 0.0f));
	modelMatrix6 = glm::scale(modelMatrix6, glm::vec3(3.9f, 3.9f, 1.0f));
	modelMatrix6 = glm::rotate(modelMatrix6,
		glm::radians(-rotate_z),
		glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix3 = glm::translate(modelMatrix6, glm::vec3(player_y, 0.0f, 0.0f));
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };


	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, NULL);
	program.SetModelMatrix(modelMatrix);//not texture
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, NULL);
	program.SetModelMatrix(modelMatrix1);//not texture
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, playerTextureID2);// yes texture
	program.SetModelMatrix(modelMatrix2);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, playerTextureID4);// yes texture
	program.SetModelMatrix(modelMatrix4);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, playerTextureID5);// yes texture
	program.SetModelMatrix(modelMatrix5);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, playerTextureID3);
	program.SetModelMatrix(modelMatrix3);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, playerTextureID3);
	program.SetModelMatrix(modelMatrix6);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);

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