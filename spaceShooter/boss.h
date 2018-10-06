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

class boss
{
public:
	int const enemySpeed = 5;

	boss();
	boss(SDL_Renderer* gRenderer, int x, int y,bool right=true);
	~boss();

	void loadTexture(SDL_Renderer* gRenderer);
	void render(SDL_Renderer* gRenderer, SDL_Rect projectiles[]);
	void move(SDL_Renderer* gRenderer);
	void actions(SDL_Renderer* gRenderer);
	void loadWarpTexture(SDL_Renderer* gRenderer);
	bool checkCollision(SDL_Rect a, SDL_Rect b[]);
	SDL_Rect getCollider();
	SDL_Rect getEnemyProjectile();

	int lives;
	bool summon;
private:
	SDL_Texture* mTexture;
	SDL_Texture* warpTexture;

	int xPos;
	int yPos;
	int hitTime;
	int xRand;

	int mWidth;
	int mHeight;
	int warpWidth;
	int warpHeight;
	int random;
	int frame;
	int warpFrame;
	Uint32 timer;

	bool goRight;
	bool attack;
	bool fire;
	bool warpIn;

	SDL_Rect currentClip;
	SDL_Rect bossClips[4];
	SDL_Rect enemyProjectile;
	SDL_Rect warpClips[6];
	SDL_Rect currentWarpClip;
};

