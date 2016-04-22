#include "ServerGame.hpp"

#include <CheckersDefines.hpp>

using namespace CheckersDefines;

void GameControllerThread(Socket* socket){
	GameController* gameController = new GameController(socket);

	while (socket->IsOpen())
		Socket::PotentialWait();
}

GameController::Vector2 GameController::_CellToCoord(unsigned int cell){
	Vector2 coord;

	coord.x = (cell - Board::Begin) % Board::Width;
	coord.y = (unsigned int)std::floorf(((cell - Board::Begin) / Board::Width));

	return coord;
}

unsigned int GameController::_CoordToCell(Vector2 coord){
	return Board::Begin + (coord.y * Board::Width) + coord.x;
}

void GameController::_GetJumps(std::list<Vector2>& list, unsigned int player, Vector2 from, bool king){
	for (unsigned int i = 0; i < 2; i++){
		int y = i;

		if (i == 0)
			y = -1;

		for (unsigned int j = 0; j < 2; j++){
			int x = j;

			if (j == 0)
				x = -1;

			// If not king
			if ((player == 0 && y < 0 || player == 1 && y > 0) && !king)
				continue;

			Vector2 pos = from;

			pos.x += x;
			pos.y += y;

			std::string checker = _GetData(_CoordToCell(pos));

			if ((player == 0 && checker == Board::Player1Checker) || (player == 0 && checker == Board::Player1Checker) ||
				(player == 1 && checker == Board::Player0Checker) || (player == 1 && checker == Board::Player0Checker)){

				pos.x += x;
				pos.y += y;

				if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0)
					continue;

				if (_GetData(_CoordToCell(pos)) == Board::NoChecker)
					list.push_back(pos);
			}
		}
	}
}

void GameController::_GetSpaces(std::list<Vector2>& list, unsigned int player, Vector2 from, bool king){
	for (unsigned int i = 0; i < 2; i++){
		int y = i;

		if (i == 0)
			y = -1;

		for (unsigned int j = 0; j < 2; j++){
			int x = j;

			if (j == 0)
				x = -1;

			// If not king
			if ((player == 0 && y < 0 || player == 1 && y > 0) && !king)
				continue;

			Vector2 pos = from;

			pos.x += x;
			pos.y += y;

			if (pos.x > 7 || pos.x < 0 || pos.y > 7 || pos.y < 0)
				continue;

			if (_GetData(_CoordToCell(pos)) == Board::NoChecker)
				list.push_back(pos);
		}
	}
}

GameController::GameController(Socket* socket) : ViewController(Key::Game, socket){
	_SetData(Variable::WinState, State::Server);
	_SetData(Variable::GameState, State::Player0);

	unsigned int y = 0;

	bool onOdd = true;

	for (unsigned int i = Board::Begin; i <= Board::End; i++){
		Vector2 pos = _CellToCoord(i);

		if (pos.y != y){
			y = pos.y;

			if (onOdd)
				onOdd = false;
			else
				onOdd = true;
		}

		if (y > 2 && y < 5){
			_SetData(i, Board::NoChecker);
			continue;
		}

		std::string type;

		if (y < 3)
			type = Board::Player0Checker;
		else if (y > 4)
			type = Board::Player1Checker;
		else{
			_SetData(i, Board::NoChecker);
			continue;
		}

		if (onOdd && i % 2 == 1)
			_SetData(i, type);
		else if (onOdd)
			_SetData(i, Board::NoChecker);
		else if (!onOdd && i % 2 == 0)
			_SetData(i, type);
		else
			_SetData(i, Board::NoChecker);
	}

	_SendAmendments();
}

void GameController::OnData(const DataMap& data, int client){
	if (_GetData(Variable::GameState) == State::Server)
		return;

	if (client == -1)
		return;
	
	if (_GetData(Variable::GameState) == State::Player0 && client != 0)
		return;
	
	if (_GetData(Variable::GameState) == State::Player1 && client != 1)
		return;

	auto from = data.find(Variable::FromPos);
	auto to = data.find(Variable::ToPos);

	if (from == data.end() || to == data.end())
		return;

	unsigned int fromPos = std::stoi(from->second);
	unsigned int toPos = std::stoi(to->second);

	if ((((_GetData(fromPos) == Board::Player0Checker) || (_GetData(fromPos) == Board::Player0King)) && client != 0) ||
		(((_GetData(fromPos) == Board::Player1Checker) || (_GetData(fromPos) == Board::Player1King)) && client != 1))
		return;

	if (_GetData(fromPos) == Board::NoChecker || _GetData(toPos) != Board::NoChecker)
		return;
	
	Vector2 fromVec = _CellToCoord(fromPos);
	Vector2 toVec = _CellToCoord(toPos);

	bool king = false;

	if (_GetData(fromPos) == Board::Player0King || _GetData(fromPos) == Board::Player1King)
		king = true;

	if (((client == 0 && fromVec.y >= toVec.y) || (client == 1 && fromVec.y <= toVec.y)) && !king)
		return;

	_SetData(toPos, _GetData(fromPos));
	_SetData(fromPos, Board::NoChecker);

	if (client == 0)
		_SetData(Variable::GameState, State::Player1);
	else if (client == 1)
		_SetData(Variable::GameState, State::Player0);

	_SendAmendments();
}

void GameController::PrintBoard(){
	system("cls");

	unsigned int y = 0;

	for (unsigned int i = Board::Begin; i <= Board::End; i++){
		unsigned int newY = (unsigned int)std::floorf(((i - Board::Begin) / Board::Width));

		if (newY != y){
			y = newY;
			std::cout << "\n";
		}

		std::cout << _GetData(i) << " ";
	}
}