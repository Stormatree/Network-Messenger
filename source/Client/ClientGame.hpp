#pragma once

#include <Socket.hpp>
#include <ViewController.hpp>

#include <cmath>
#include <SDL.h>

void GameViewControllerThread(Socket* socket);

class GameViewController : ViewController{
	SDL_Window* _window = nullptr;
	SDL_Surface* _rawSurface = nullptr;
	SDL_Surface* _windowSurface = nullptr;

	int _cellDown = -1;
	int _cellOver = -1;

	int _client = -1;

	SDL_Colour _background;

	struct{
		SDL_Surface* backgroundBoard = nullptr;
		SDL_Surface* redChecker = nullptr;
		SDL_Surface* blackChecker = nullptr;
		SDL_Surface* cellStroke = nullptr;
		SDL_Surface* crown = nullptr;
	} _images;

	struct Vector2{
		unsigned int x;
		unsigned int y;
	};

	unsigned int _CoordToCell(unsigned int x, unsigned int y);

	Vector2 _CellToCoord(unsigned int cell);

	void _DrawHighlight(unsigned int cell);

	void _DrawChecker(unsigned int cell, std::string type);

	void _DrawBoard();

	bool _InBoardArea(unsigned int x, unsigned int y);

	unsigned int _IncreaseCoord(unsigned int value);

public:
	GameViewController(Socket* socket);

	~GameViewController();

	void OnMessage(std::string message, int client = -1) override;

	void Update();
};