#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>

using namespace std;

class enemy
{
public:
	int const enemySpeed = 5;
	enemy();
	enemy(SDL_Renderer* renderer, string path, int x, int y, bool right = true);
	~enemy();

	void loadTexture(SDL_Renderer* gRenderer,string path);
	void render(SDL_Renderer* renderer,SDL_Rect b[]);
	void move();
	void free();

	bool checkCollision(SDL_Rect a, SDL_Rect b[]);

	SDL_Rect getCollider();

	int lives;
private:
	int enemyWidth;
	int enemyHeight;
	int xPos;
	int yPos;
	int enemyLives;
	int random;
	Uint32 hitTime;
	Mix_Chunk* hitFx;

	bool goRight;
	bool attack = true;

	SDL_Texture* mTexture;
};