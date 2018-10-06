#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include "player.h"
#include "enemy.h"
#include "asteroid.h"
#include "boss.h"

using namespace std;

extern int const windowWidth = 640, windowHeight = 480;

void init(SDL_Window* gWindow, SDL_Renderer*& gRenderer)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	gWindow = SDL_CreateWindow("Amazing Game!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
		cout << "could not create renderer" << endl;

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) &imgFlags))
		cout << "something stupid" << endl;

	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);
	TTF_Init();
}
void loadMap(SDL_Renderer* gRenderer, SDL_Texture*& map)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load("bg.png");
	map = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
}
SDL_Texture* loadText(SDL_Renderer * gRenderer, SDL_Color textColor, TTF_Font* font, string text)
{
	SDL_Texture* tempTexture = NULL;
	SDL_Surface* textSurface = NULL;
	textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (textSurface == NULL)
	{
		cout << "could not load text surface" << SDL_GetError << endl;
	}
	tempTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
	SDL_FreeSurface(textSurface);
	return tempTexture;
}

int main(int argc, char* argv[])
{
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Event ev;
	TTF_Font* gFont = NULL;
	SDL_Color textColor = { 255,255,255 };
	SDL_Texture* map = NULL;
	SDL_Texture* playerLives = NULL;
	int mapOffset = 0;
	SDL_Rect colliders[10];
	for (int i = 0;i < 3;i++)
	{
		colliders[i] = { 0,0,0,0 };
	}
	SDL_Rect projectiles[2];
	Mix_Music* song = song=Mix_LoadMUS("leonidas.mp3");
	SDL_Rect textRect = { 0,0,70,50 };
	SDL_Rect levelTextRec = { (windowWidth/2)-30,(windowHeight/2)-50,70,50 };
	bool level1 = true, level2 = false, level3 = false, level4 = false, victory = false;
	Uint32 timer = 0;

	stringstream lives,levelText;
	lives.str("");
	levelText.str("Level 1");
	
	init(gWindow,gRenderer);
	loadMap(gRenderer,map);

	gFont = TTF_OpenFont("SIXTY.TTF", 28);
	if (gFont == NULL)
	{
		cout << "font is NULL" << endl;
	}
	player player1;
	player1.loadTexture(gRenderer);
	enemy enemy1(gRenderer, "alien.png", 320, 0);
	enemy enemy2(gRenderer, "alien.png", 460, 0, false);

	enemy enemy3(gRenderer, "alien.png", 320, 0);
	enemy enemy4(gRenderer, "alien.png", 460, 0, false);
	enemy enemy5(gRenderer, "alien.png", 400, 0);

	boss boss1(gRenderer, windowWidth / 2, 0, true);
	enemy summoned(gRenderer, "alien.png", windowWidth/2, 0);

	asteroid rock[10];
	for (int i = 0; i < 10; i++)
	{
		rock[i].loadTexture(gRenderer);
	}

	bool quit = false;

	Mix_PlayMusic(song, -1);
	level4 = false;
	cout << "Game test:" << endl;
	cout << "move with arow keys. Hold 'a' to accelerate. spacebar to shoot" << endl;

	while (!quit)
	{
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderClear(gRenderer);

		SDL_Rect mapRect = { 0,mapOffset,windowWidth,windowHeight };
		SDL_Rect mapRect2 = { 0,mapOffset - 480,windowWidth,windowHeight };
		SDL_RenderCopy(gRenderer, map, NULL, &mapRect);
		SDL_RenderCopy(gRenderer, map, NULL, &mapRect2);
		if (player1.lives > 0)
		{
			if (player1.fire2)
				projectiles[0] = player1.getProjectile();
			else
				projectiles[0] = { 0,0,0,0 };

			if (player1.fire4)
				projectiles[1] = player1.getProjectile2();
			else
				projectiles[1] = { 0,0,0,0 };

			while (SDL_PollEvent(&ev) == 1)
			{
				if (ev.type == SDL_QUIT)
				{
					quit = true;
				}
				if (!player1.hit)
				{
					player1.handleEvent(ev);
				}
			}
			if (player1.a)
				mapOffset = mapOffset + 4;
			else
				mapOffset = mapOffset + 2;
			if (mapOffset > 480)
			{
				mapOffset = 0;
			}

			if (level1)
			{
				if (enemy1.lives == 0 && enemy2.lives == 0)
				{
					level1 = false;
					level2 = true;
					timer = SDL_GetTicks();
				}
				if (enemy1.lives > 0)
					enemy1.render(gRenderer, projectiles);
				else
					enemy1.free();

				if (enemy2.lives > 0)
					enemy2.render(gRenderer, projectiles);
				else
					enemy2.free();
				colliders[0] = enemy1.getCollider();
				colliders[1] = enemy2.getCollider();
			}
			if (level2)
			{
				if (SDL_GetTicks() - timer < 5000)
				{
					levelText.str("");
					levelText << "Level 2";
					SDL_RenderCopy(gRenderer, loadText(gRenderer, textColor, gFont, levelText.str().c_str()), NULL, &levelTextRec);
				}
				else
				{
					if (enemy3.lives == 0 && enemy4.lives == 0 && enemy5.lives == 0)
					{
						level2 = false;
						level3 = true;
						timer = SDL_GetTicks();
					}
					if (enemy3.lives > 0)
						enemy3.render(gRenderer, projectiles);
					else
						enemy3.free();

					if (enemy4.lives > 0)
						enemy4.render(gRenderer, projectiles);
					else
						enemy4.free();

					if (enemy5.lives > 0)
						enemy5.render(gRenderer, projectiles);
					else
						enemy5.free();

					colliders[0] = enemy3.getCollider();
					colliders[1] = enemy4.getCollider();
					colliders[2] = enemy5.getCollider(); 
				}
			}
			if (level3)
			{
				if (SDL_GetTicks() - timer < 5000)
				{
					levelText.str("");
					levelText << "Level 3";
					SDL_RenderCopy(gRenderer, loadText(gRenderer, textColor, gFont, levelText.str().c_str()), NULL, &levelTextRec);
				}
				if (SDL_GetTicks() - timer > 5000 && SDL_GetTicks() - timer < 10000)
				{
					SDL_Rect meteorShower = { (windowWidth / 2) - 100,(windowHeight / 2) - 20 ,250,50 };
					levelText.str("");
					levelText << "METEOR SHOWER DETECTED!!";
					SDL_RenderCopy(gRenderer, loadText(gRenderer, textColor, gFont, levelText.str().c_str()), NULL, &meteorShower);
				}
				if (SDL_GetTicks() - timer > 10000)
				{
					for (int i = 0;i < 10; i++)
					{
						rock[i].render(gRenderer);
						colliders[i] = rock[i].getCollider();
					}
				}
				if (SDL_GetTicks() - timer > 50000)
				{
					level4 = true;
					level3 = false;
					timer = SDL_GetTicks();
					for (int i = 0;i < 10; i++)
					{
						colliders[i] = { 0,0,0,0 };
					}
				}
			}
			if (level4)
			{
				if (SDL_GetTicks() - timer < 5000)
				{
					SDL_Rect level4 = { windowWidth,windowHeight,100,50 };
					levelText.str("");
					levelText << "Level 4";
					SDL_RenderCopy(gRenderer, loadText(gRenderer, textColor, gFont, levelText.str().c_str()),NULL,&level4);
				}
				if (SDL_GetTicks() - timer > 5000)
				{
					if (boss1.lives > 0 || summoned.lives>0)
					{	
						colliders[0] = boss1.getCollider();
						colliders[1] = boss1.getEnemyProjectile();
						boss1.render(gRenderer,projectiles);
						if (boss1.summon)
						{
							if (summoned.lives > 0)
							{
								summoned.render(gRenderer, projectiles);
								colliders[2] = summoned.getCollider();
							}
							else
							{
								boss1.summon = false;
								summoned.lives = 5;
								colliders[2] = { 0,0,0,0 };
							}
						}
					}
					else
					{
						colliders[0] = { 0,0,0,0 };
					}
					if (boss1.lives == 0)
					{
						level4 = false;
						victory = true;
						timer = SDL_GetTicks();
					}
				}
			}
			if (victory)
			{
				if (SDL_GetTicks() - timer < 10000)
				{
					levelText.str("");
					levelText << "You Win! :)";
					SDL_Rect winText = { (windowWidth / 2) - 50,(windowHeight / 2) + 20,50,20 };
					SDL_RenderCopy(gRenderer, loadText(gRenderer, textColor, gFont, levelText.str().c_str()), NULL, &winText);
				}
			}
			player1.move(colliders);
			player1.render(gRenderer);

			SDL_RenderDrawRect(gRenderer, &player1.getProjectile2());
			SDL_RenderDrawRect(gRenderer, &player1.getProjectile());
		}
		else
		{
			while (SDL_PollEvent(&ev) == 1)
			{
				if (ev.type == SDL_QUIT)
				{
					quit = true;
				}
			}
			player1.render(gRenderer);
			SDL_Rect deathText = { (windowWidth / 2)-50,windowHeight / 2,100,50 };
			levelText.str("");
			levelText << "Game Over!";
			SDL_RenderCopy(gRenderer, loadText(gRenderer, textColor, gFont, levelText.str().c_str()),NULL,&deathText);
		}
		lives.str("");
		lives << "Lives: " << player1.lives;
		SDL_RenderCopy(gRenderer, loadText(gRenderer, textColor, gFont, lives.str().c_str()), NULL, &textRect);
		SDL_RenderPresent(gRenderer);
	} 

	return 0;
}