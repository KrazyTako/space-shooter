#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>

using namespace std;

class player
{
public:
	const int projectileSpeed = 7;
	
	player();
	~player();
	
	void createClips();
	void loadTexture(SDL_Renderer* gRenderer);
	void render(SDL_Renderer* renderer);
	void handleEvent(SDL_Event ev);
	void move(SDL_Rect b[]);
	void projectileMove();

	bool checkCollision(SDL_Rect a, SDL_Rect b[]);

	SDL_Rect getPlayerCollider();
	SDL_Rect getProjectile();
	SDL_Rect getProjectile2();

	bool a;
	bool hit;
	bool fire2;
	bool fire4;
	int lives;

private:
	SDL_Texture* mTexture;
	SDL_Texture* hitTexture;
	SDL_Rect clips[4];
	SDL_Rect projectile;
	SDL_Rect projectile2;
	Mix_Chunk* laser;

	int xPos;
	int yPos;
	int imageWidth;
	int imageHeight;
	int textureWidth;
	int textureHeight;
	int frame;
	int xOffset;
	int yOffset;
	int speed;
	Uint32 timer;
	Uint32 timer2;
	Uint32 lastHit;

	bool keyUp;
	bool keyDown;
	bool keyLeft;
	bool keyRight;

	bool fire;
	bool fire3;
	bool fire5;
};