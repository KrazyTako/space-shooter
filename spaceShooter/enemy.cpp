#include "enemy.h"



enemy::enemy()
{
}
enemy::enemy(SDL_Renderer* renderer, string path, int x, int y,bool right)
{
	xPos = x;
	yPos = y;
	loadTexture(renderer, path);
	goRight = right;
	attack = false;
	srand(time(NULL));
	lives = 5;
	hitTime = 0;
	hitFx = Mix_LoadWAV("zombie.wav");
}
enemy::~enemy()
{
}

void enemy::loadTexture(SDL_Renderer* renderer,string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "could not create enemy surface" << endl;
	}

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == NULL)
	{
		cout << "Could not create enemy texture: " << IMG_GetError<< endl;
	}
	enemyWidth = loadedSurface->w;
	enemyHeight = loadedSurface->h;
	SDL_FreeSurface(loadedSurface);
	mTexture = newTexture;
}
void enemy::render(SDL_Renderer* renderer, SDL_Rect b[])
{
	move();
	if (checkCollision(getCollider(),b) && SDL_GetTicks()-hitTime>400)
	{
		hitTime = SDL_GetTicks();
		lives--;
		Mix_PlayChannel(-1, hitFx, 0);
		cout << "Enemy hit! " << lives << endl;
	}
		SDL_Rect renderQuad = { xPos,yPos,enemyWidth,enemyHeight };
		SDL_RenderCopy(renderer, mTexture, NULL, &renderQuad);
}
void enemy::move()
{
	random = ((rand()) % 150);
	if (random == 67)
	{
		attack = true;
	}
	if (attack)
	{
		yPos += enemySpeed;
		if (yPos + enemyHeight >= 480)
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
	if (goRight)
	{
		xPos += enemySpeed;
		if (xPos + enemyWidth>=640 )
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
}
void enemy::free()
{
	SDL_DestroyTexture(mTexture);
	enemyWidth = 0;
	enemyHeight = 0;
	xPos = 0;
	yPos = 0;
}

bool enemy::checkCollision(SDL_Rect a, SDL_Rect b[])
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
			complete=false;
		}

		else if (topA >= bottomB[i])
		{
			complete=false;
		}

		else if (rightA <= leftB[i])
		{
			complete=false;
		}

		else if (leftA >= rightB[i])
		{
			complete=false;
		}
		else
		{
			return true;
		}
	}
	return complete;
}

SDL_Rect enemy::getCollider()
{
	SDL_Rect temp = { xPos,yPos,enemyWidth,enemyHeight };
	return temp;
}