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

class asteroid
{
public:
	asteroid();
	~asteroid();

	void loadTexture(SDL_Renderer* gRenderer);
	void render(SDL_Renderer* gRenderer);
	void move();

	SDL_Rect getCollider();

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
	int xPos;
	int yPos;
	int speed;

	bool fall;
	SDL_Rect collider;
};