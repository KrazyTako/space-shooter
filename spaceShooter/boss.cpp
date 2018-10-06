#include "boss.h"



boss::boss()
{

}
boss::boss(SDL_Renderer* gRenderer,int x, int y,bool right)
{
	xPos = x;
	yPos = y;
	lives = 30;
	mWidth = 0;
	mHeight = 0;
	frame = 0;
	warpFrame = 0;
	hitTime = 0;
	loadTexture(gRenderer);
	loadWarpTexture(gRenderer);
	srand(time(NULL));

	goRight = right;
	attack = false;
	fire = false;
	summon = false;

	enemyProjectile = { xPos,yPos,8,8 };
	bossClips[0] = { 0,0,mWidth / 2,mHeight / 2 };
	bossClips[1] = { mWidth / 2,0,mWidth / 2,mHeight / 2 };
	bossClips[2] = { 0,mHeight / 2,mWidth / 2,mHeight / 2 };
	bossClips[3] = { mWidth / 2,mHeight / 2,mWidth / 2,mHeight / 2 };

	warpClips[0] = { 0,0,warpWidth / 3,warpHeight / 2, };
	warpClips[1] = { warpWidth / 3,0,warpWidth / 3,warpHeight / 2 };
	warpClips[2] = { 2 * warpWidth / 3,0,warpWidth / 3,warpHeight / 2 };
	warpClips[3] = { 0,warpHeight / 2,warpWidth / 3,warpHeight / 2 };
	warpClips[4] = { warpWidth / 3,warpHeight / 2,warpWidth / 3,warpHeight / 2 };
	warpClips[5] = { 2 * warpWidth / 3,warpHeight / 2,warpWidth / 3,warpHeight / 2 };
}
boss::~boss()
{
}

void boss::loadTexture(SDL_Renderer* gRenderer)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load("alienboss4.png");
	if (loadedSurface == NULL)
	{
		cout << "could not create enemy surface" << endl;
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if (newTexture == NULL)
	{
		cout << "Could not create enemy texture: " << IMG_GetError << endl;
	}
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	SDL_FreeSurface(loadedSurface);
	mTexture = newTexture;
}
void boss::loadWarpTexture(SDL_Renderer* gRenderer)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load("warp.png");
	if (loadedSurface == NULL)
	{
		cout << "could not create enemy surface" << endl;
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if (newTexture == NULL)
	{
		cout << "Could not create enemy texture: " << IMG_GetError << endl;
	}
	warpWidth = loadedSurface->w;
	warpHeight = loadedSurface->h;
	SDL_FreeSurface(loadedSurface);
	warpTexture = newTexture;
}
void boss::render(SDL_Renderer* gRenderer, SDL_Rect projectiles[])
{
	move(gRenderer);
	SDL_Rect renderQuad = { xPos,yPos,mHeight,mWidth };
	if (checkCollision(renderQuad, projectiles) && SDL_GetTicks() - hitTime>400)
	{
		hitTime = SDL_GetTicks();
		lives--;
		//Mix_PlayChannel(-1, hitFx, 0);
		cout << "Enemy hit! " << lives << endl;
	}
	currentClip = bossClips[frame / 4];
	SDL_RenderCopy(gRenderer, mTexture, &currentClip, &renderQuad);
	frame++;
	if (frame / 4 >= 4)
	{
		frame = 0;
	}
}
void boss::move(SDL_Renderer* gRenderer)
{
	actions(gRenderer);

	if (goRight)
	{
		xPos += enemySpeed;
		if (xPos + mWidth >= 640)
		{
			goRight = false;
		}
	}
	else
	{
		xPos -= enemySpeed;
		if (xPos <= 0)
		{
			goRight = true;
		}
	}
	if (!fire)
	{
		enemyProjectile.x = xPos+(mWidth/2);
		enemyProjectile.y = yPos+(mHeight-20);
	}
}
void boss::actions(SDL_Renderer* gRenderer)
{
	random = ((rand()) % 150);
	if (random == 67)
	{
		attack = true;
	}
	if (attack)
	{
		yPos += enemySpeed;
		if (yPos + mHeight >= 480)
		{
			attack = false;
		}
	}
	else
	{
		yPos -= enemySpeed;
		if (yPos <= 0)
		{
			yPos = 0;
		}
	}
	if (rand() % 50 == 33 && yPos <= 240)
	{
		fire = true;
	}
	if (fire)
	{
		enemyProjectile.y += 10;
		SDL_RenderDrawRect(gRenderer, &enemyProjectile);
		if (enemyProjectile.y >= 480)
		{
			fire = false;
			enemyProjectile.y = yPos;
		}
	}
	if (!warpIn && !summon && rand() % 100 == 50)
	{
		timer = SDL_GetTicks();
		warpIn = true;
		xRand = rand() % 640;
	}
	if (warpIn)
	{
		SDL_Rect renderQuad = { xRand,0,warpWidth/5,warpHeight/4 };
		currentWarpClip = warpClips[warpFrame / 6];
		SDL_RenderCopy(gRenderer, warpTexture, &currentWarpClip, &renderQuad);
		if (warpFrame / 6 >= 6)
		{
			warpFrame = 0;
		}
		warpFrame++;
		if (SDL_GetTicks() - timer >= 5000)
		{
			timer = 0;
			warpIn = false;
			summon = true;
		}
	}
}
bool boss::checkCollision(SDL_Rect a, SDL_Rect b[])
{
	bool complete = false;
	int leftA, leftB[2];
	int rightA, rightB[2];
	int topA, topB[2];
	int bottomA, bottomB[2];

	for (int i = 0; i < 2; i++)
	{
		leftA = a.x;
		rightA = a.x + a.w;
		topA = a.y;
		bottomA = a.y + a.h;

		leftB[i] = b[i].x;
		rightB[i] = b[i].x + b[i].w;
		topB[i] = b[i].y;
		bottomB[i] = b[i].y + b[i].h;

		if (bottomA <= topB[i])
		{
			complete = false;
		}

		else if (topA >= bottomB[i])
		{
			complete = false;
		}

		else if (rightA <= leftB[i])
		{
			complete = false;
		}

		else if (leftA >= rightB[i])
		{
			complete = false;
		}
		else
		{
			return true;
		}
	}
	return complete;
}
SDL_Rect boss::getCollider()
{
	SDL_Rect temp = { xPos,yPos,mWidth,mHeight };
	return temp;
}
SDL_Rect boss::getEnemyProjectile()
{
	return enemyProjectile;
}