#include "player.h"



player::player()
{
	mTexture = NULL;

	xPos = 640/2;
	yPos = 460;
	textureWidth = 0;
	textureHeight = 0;
	lives =8;
	speed = 5;
	frame = 0;

	xOffset = 9;
	yOffset = 1;

	a = false;
	keyUp = false;
	keyDown = false;
	keyLeft = false;
	keyRight = false;
	fire = false;
	fire2 = false;
	fire3 = false;
	fire4 = false;
	fire5 = true;
	hit = false;
	timer = 0;
	timer2 = 0;
	lastHit = 0;
	laser = Mix_LoadWAV("laser.wav");

	//official width is 3 and height is 15
	projectile = { xPos+xOffset,yPos+yOffset,3,15 };
	projectile2 = { xPos + xOffset,yPos + yOffset,3,15 };

	textureWidth = 0;
	textureHeight = 0;
}
player::~player()
{
}

void player::createClips()
{
	clips[0].x = 0;
	clips[0].y = 0;
	clips[0].w = textureWidth;
	clips[0].h = textureHeight;

	clips[1].x = textureWidth;
	clips[1].y = 0;
	clips[1].w = textureWidth;
	clips[1].h = textureHeight;

	clips[2].x = 0;
	clips[2].y = textureHeight;
	clips[2].w = textureWidth;
	clips[2].h = textureHeight;

	clips[3].x = textureWidth;
	clips[3].y = textureHeight;
	clips[3].w = textureWidth;
	clips[3].h = textureHeight;
}
void player::loadTexture(SDL_Renderer* gRenderer)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = NULL;
	loadedSurface = IMG_Load("spaceship.png");

	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if (newTexture == NULL)
	{
		cout << "Could not create texture: " << endl;
	}
	imageWidth = loadedSurface->w;
	imageHeight = loadedSurface->h;

	textureWidth = loadedSurface->w/2;
	textureHeight = loadedSurface->h/2;
	createClips();
	mTexture = newTexture;

	loadedSurface = IMG_Load("spaceshiphit.png");
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	hitTexture = newTexture;
}
void player::render(SDL_Renderer* renderer)
{
	SDL_Rect* currentClip = &clips[frame / 4];

	if (!hit)
	{
		SDL_Rect renderQuad = { xPos,yPos,textureWidth,textureHeight };
		if (currentClip != NULL)
		{
			renderQuad.w = currentClip->w;
			renderQuad.h = currentClip->h;
		}
		SDL_RenderCopy(renderer, mTexture, currentClip, &renderQuad);
	}
	else
	{
		SDL_Rect renderQuad = { xPos,yPos,textureWidth,textureHeight };
		if (currentClip != NULL)
		{
			renderQuad.w = currentClip->w;
			renderQuad.h = currentClip->h;
		}
		SDL_RenderCopy(renderer, hitTexture, currentClip, &renderQuad);
	}
	frame++;
	if (frame / 4 >= 4)
	{
		frame = 0;
	}
}
void player::handleEvent(SDL_Event ev)
{
	if (ev.type == SDL_KEYDOWN)
	{
		switch (ev.key.keysym.sym)
		{
		case SDLK_UP:
			keyUp = true;
			break;
		case SDLK_DOWN:
			keyDown = true;
			break;
		case SDLK_LEFT:
			keyLeft = true;
			break;
		case SDLK_RIGHT:
			keyRight = true;
			break;
		case SDLK_SPACE:
			fire = true;
			break;
		case SDLK_a:
			speed = 8;
			a = true;
			break;
		}
	}
	if(ev.type==SDL_KEYUP)
	{
		switch (ev.key.keysym.sym)
		{
		case SDLK_UP:
			keyUp = false;
			break;
		case SDLK_DOWN:
			keyDown = false;
			break;
		case SDLK_LEFT:
			keyLeft = false;
			break;
		case SDLK_RIGHT:
			keyRight = false;
			break;
		case SDLK_SPACE:
			fire = false;
			break;
		case SDLK_a:
			speed = 5;
			a = false;
			break;
		}
	}
}
void player::move(SDL_Rect b[])
{
	if (!hit)
	{
		if (keyUp)
		{
			yPos -= speed;
			if (yPos - speed <= 0)
			{
				yPos = 0;
			}
		}
		if (keyDown)
		{
			yPos += speed;
			if (yPos + speed >= 480 - textureHeight)
			{
				yPos = 480 - textureHeight;
			}
		}
		if (keyLeft)
		{
			xPos -= speed;
			if (xPos - speed <= 0)
			{
				xPos = 0;
			}
		}
		if (keyRight)
		{
			xPos += speed;
			if (xPos + speed >= 640 - textureWidth)
			{
				xPos = 640 - textureWidth;
			}
		}
	}
	projectileMove();
	if (!hit && SDL_GetTicks() - lastHit>1000)
	{
		if (checkCollision(getPlayerCollider(), b))
		{
			keyUp = false;
			keyDown = false;
			keyLeft = false;
			keyRight = false;
			fire = false;  
			speed = 5;

			hit = true;
			lives--;
		}
	}
	if (hit)
	{
		yPos += speed + speed;
		if (yPos + speed >= 480 - textureHeight)
		{
			yPos = 480 - textureHeight;
			lastHit = SDL_GetTicks();
			hit = false;
		}
	}
}
void player::projectileMove()
{
	if (!fire2 && !fire4)
	{
		projectile.x = xPos + xOffset;
		projectile.y = yPos + yOffset;
		projectile2.x = xPos + xOffset;
		projectile2.y = yPos + yOffset;
	}
	if (fire && fire5 && projectile.y == yPos + yOffset)
	{
		Mix_PlayChannel(-1, laser, 0);
		timer = SDL_GetTicks();
		fire2 = true;
	}
	if (fire2)
	{
		projectile.y -= projectileSpeed;
		if (!fire4)
		{
			projectile2.x = xPos + xOffset;
			projectile2.y = yPos + yOffset;
		}
		if (SDL_GetTicks() - timer > 550)
		{
			fire3 = true;
		}
		else
		{
			fire3 = false;
		}
		if (SDL_GetTicks() - timer > 1100)
		{
			projectile.x = xPos + xOffset;
			projectile.y = yPos + yOffset;
			fire2 = false;
			fire5 = true;
			timer = 0;
		}
	}
	if (fire && fire3 && projectile2.y==yPos+yOffset)
	{
		Mix_PlayChannel(-1, laser, 0);
		timer2 = SDL_GetTicks();
		fire4 = true;
	} 
	if (fire4)
	{
		projectile2.y -= projectileSpeed;
		if (!fire2)
		{
			projectile.x = xPos + xOffset;
			projectile.y = yPos + yOffset;
		}
		if (SDL_GetTicks() - timer2 > 550)
		{
			fire5 = true;
		}
		else
		{
			fire5 = false;
		}
		if (SDL_GetTicks() - timer2 > 1100)
		{
			projectile2.x = xPos + xOffset;
			projectile2.y = yPos + yOffset;
			fire4 = false;
			timer2 = 0;
		}
	}
}

bool player::checkCollision(SDL_Rect a, SDL_Rect b[])
{
	bool complete = false;
	int leftA, leftB[10];
	int rightA, rightB[10];
	int topA, topB[10];
	int bottomA, bottomB[10];

	for (int i = 0;i < 10; i++)
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

SDL_Rect player::getPlayerCollider()
{
	SDL_Rect temp = { xPos,yPos,textureWidth,textureHeight };
	return temp;
}
SDL_Rect player::getProjectile()
{
	return projectile;
}
SDL_Rect player::getProjectile2()
{
	return projectile2;
}