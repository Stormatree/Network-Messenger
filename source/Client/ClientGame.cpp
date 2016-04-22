#include "ClientGame.hpp"

#include <CheckersDefines.hpp>

#include <SDL_image.h>
#include <iostream>

using namespace CheckersDefines;

namespace Visual{
	const unsigned int WindowSize = 512;

	const unsigned int BoardSize = 800;
	const unsigned int CellSize = 100;
	const unsigned int Stroke = 20;

	const std::string SrcBackgroundBoard = "../data/board.png";
	const std::string SrcRedChecker = "../data/red.png";
	const std::string SrcBlackChecker = "../data/black.png";
	const std::string SrcCellStroke = "../data/stroke.png";
	const std::string SrcCrown = "../data/crown.png";

	const unsigned int HightlightOffset = 12;

	const SDL_Colour Red = { 252, 107, 118 };
	const SDL_Colour Black = { 94, 94, 94 };
};

void GameViewControllerThread(Socket* socket){
	GameViewController* gameViewController = new GameViewController(socket);

	while (socket->IsOpen()){
		Socket::PotentialWait();
		gameViewController->Update();
	}
}

unsigned int GameViewController::_CoordToCell(unsigned int x, unsigned int y){
	x = (unsigned int)std::floorf((_IncreaseCoord(x) - Visual::Stroke) / Visual::CellSize);
	y = (unsigned int)std::floorf((_IncreaseCoord(y) - Visual::Stroke) / Visual::CellSize);

	return x + (y * Board::Width) + Board::Begin;
}

GameViewController::Vector2 GameViewController::_CellToCoord(unsigned int cell){
	Vector2 value;

	value.x = (cell - Board::Begin) % Board::Width;
	value.y = (unsigned int)std::floorf(((cell - Board::Begin) / Board::Width));

	value.x *= Visual::CellSize;
	value.y *= Visual::CellSize;

	value.x += Visual::Stroke;
	value.y += Visual::Stroke;

	return value;
}

void GameViewController::_DrawHighlight(unsigned int cell){
	Vector2 position = _CellToCoord(cell);

	SDL_Rect rect;

	rect.x = position.x - Visual::HightlightOffset;
	rect.y = position.y - Visual::HightlightOffset;
	rect.w = Visual::CellSize + Visual::HightlightOffset;
	rect.h = Visual::CellSize + Visual::HightlightOffset;

	SDL_BlitSurface(_images.cellStroke, nullptr, _rawSurface, &rect);
}

void GameViewController::_DrawChecker(unsigned int cell, std::string type){
	if (type == Board::NoChecker)
		return;

	Vector2 position = _CellToCoord(cell);

	SDL_Rect rect;

	rect.x = position.x;
	rect.y = position.y;
	rect.w = Visual::CellSize;
	rect.h = Visual::CellSize;

	if (type == Board::Player0Checker || type == Board::Player0King)
		SDL_BlitSurface(_images.redChecker, nullptr, _rawSurface, &rect);
	else if (type == Board::Player1Checker || type == Board::Player1King)
		SDL_BlitSurface(_images.blackChecker, nullptr, _rawSurface, &rect);

	if (type == Board::Player0King || type == Board::Player1King)
		SDL_BlitSurface(_images.crown, nullptr, _rawSurface, &rect);
}

void GameViewController::_DrawBoard(){
	SDL_Rect rect;

	rect.x = Visual::Stroke;
	rect.y = Visual::Stroke;
	rect.w = Visual::BoardSize;
	rect.h = Visual::BoardSize;

	SDL_BlitSurface(_images.backgroundBoard, nullptr, _rawSurface, &rect);
}

bool GameViewController::_InBoardArea(unsigned int x, unsigned int y){
	x = _IncreaseCoord(x);
	y = _IncreaseCoord(y);

	if (x < Visual::Stroke || y < Visual::Stroke || x >= Visual::Stroke + Visual::BoardSize || y >= Visual::Stroke + Visual::BoardSize)
		return false;
	
	return true;
}

unsigned int GameViewController::_IncreaseCoord(unsigned int value){
	return (value * (Visual::BoardSize + Visual::Stroke * 2)) / Visual::WindowSize;
}

GameViewController::GameViewController(Socket* socket) : ViewController(Key::Game, socket){
	SDL_Init(SDL_INIT_VIDEO);

	_window = SDL_CreateWindow("Network Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Visual::WindowSize, Visual::WindowSize, SDL_WINDOW_SHOWN);
		
	_windowSurface = SDL_GetWindowSurface(_window);
	
	_rawSurface = SDL_CreateRGBSurface(0, Visual::BoardSize + Visual::Stroke * 2, Visual::BoardSize + Visual::Stroke * 2, _windowSurface->format->BitsPerPixel, _windowSurface->format->Rmask, _windowSurface->format->Gmask, _windowSurface->format->Bmask, _windowSurface->format->Amask);

	_images.backgroundBoard = IMG_Load(Visual::SrcBackgroundBoard.c_str());
	_images.redChecker = IMG_Load(Visual::SrcRedChecker.c_str());
	_images.blackChecker = IMG_Load(Visual::SrcBlackChecker.c_str());
	_images.cellStroke = IMG_Load(Visual::SrcCellStroke.c_str());
	_images.crown = IMG_Load(Visual::SrcCrown.c_str());

	_background = Visual::Red;
}

GameViewController::~GameViewController(){
	SDL_FreeSurface(_windowSurface);
	SDL_FreeSurface(_rawSurface);
	SDL_DestroyWindow(_window);

	SDL_Quit();
}

void GameViewController::OnMessage(std::string message, int client){
	if (_client == -1)
		_client = std::stoi(message);
}

void GameViewController::Update(){
	SDL_Event e;

	int clicked = -1;

	int from = -1;
	int dragged = -1;

	while (SDL_PollEvent(&e)){
		if (e.type == SDL_MOUSEMOTION){
			if (!_InBoardArea(e.motion.x, e.motion.y)){
				_cellOver = -1;
				continue;
			}

			_cellOver = _CoordToCell(e.motion.x, e.motion.y);
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN && _InBoardArea(e.button.x, e.button.y)){
			_cellDown = _CoordToCell(e.button.x, e.button.y);
		}
		else if (e.type == SDL_MOUSEBUTTONUP && _InBoardArea(e.button.x, e.button.y)){
			if (_cellDown != -1 && _cellDown == _CoordToCell(e.button.x, e.button.y)){
				clicked = _cellDown;
			}
			else{
				dragged = _CoordToCell(e.button.x, e.button.y);
				from = _cellDown;
			}
		}
	}

	if (dragged != -1 && from != -1){
		_SetAmendment(Variable::FromPos, std::to_string(from));
		_SetAmendment(Variable::ToPos, std::to_string(dragged));

		_SendAmendments();
	}

	if (_GetData(Variable::GameState) == State::Player0)
		_background = Visual::Red;
	else if (_GetData(Variable::GameState) == State::Player1)
		_background = Visual::Black;

	SDL_FillRect(_rawSurface, nullptr, SDL_MapRGB(_rawSurface->format, _background.r, _background.g, _background.b));

	_DrawBoard();
	
	for (int i = Board::Begin; i <= Board::End; i++)
		_DrawChecker(i, _GetData(i));

	if (_cellOver != -1)
		_DrawHighlight(_cellOver);
	
	SDL_BlitScaled(_rawSurface, nullptr, _windowSurface, nullptr);

	SDL_UpdateWindowSurface(_window);
}