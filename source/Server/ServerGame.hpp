#pragma once

#include <Socket.hpp>
#include <ViewController.hpp>

#include <iostream>

void GameControllerThread(Socket* socket);

class GameController : ViewController{
	struct Vector2{
		unsigned int x;
		unsigned int y;
	};

	int _streak = -1;

	Vector2 _CellToCoord(unsigned int cell);

	unsigned int _CoordToCell(Vector2 coord);

	void _GetJumps(std::list<Vector2>& list, unsigned int player, Vector2 from, bool king = false);

	void _GetSpaces(std::list<Vector2>& list, unsigned int player, Vector2 from, bool king = false);

public:
	GameController(Socket* socket);

	void OnData(const DataMap& data, int client = -1) override;

	void PrintBoard();
};