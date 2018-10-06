#include "asteroid.h"



asteroid::asteroid()
{
	srand(time(NULL));
	fall = false;
	mWidth = 0;
	mHeight = 0;
	xPos = 0;
	yPos = 0;
}
asteroid::~asteroid()
{
}

void asteroid::loadTexture(SDL_Renderer* gRenderer)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load("asteroid.png");
	if (loadedSurface == NULL)
	{
		cout << "could not create asteroid surface" << endl;
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
void asteroid::render(SDL_Renderer* gRenderer)
{
	if (!fall && rand() % 50 == 30)
	{
		fall = true;
		mWidth = 20 + (rand() % 40);
		mHeight = mWidth;
		yPos = 0;
		xPos = rand() % (640 - mWidth);
		speed = 6 + rand() % 7;
	}
	if (fall)
	{
		move();
		SDL_Rect renderQuad = { xPos,yPos,mWidth,mHeight };
		SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
	}
}
void asteroid::move()
{
	yPos += speed;
	if (yPos >= 480)
	{
		fall = false;
	}  
}
SDL_Rect asteroid::getCollider()
{
	SDL_Rect temp = { xPos,yPos,mWidth,mHeight };
	return temp;
}