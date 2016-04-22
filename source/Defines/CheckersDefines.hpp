#pragma once

#include <string>

namespace CheckersDefines{
	namespace Key{
		const std::string Chat = "cht";
		const std::string Game = "gme";
	}

	namespace Variable{
		enum{
			GameState,

			A0, A1, A2, A3, A4, A5, A6, A7,
			B0, B1, B2, B3, B4, B5, B6, B7,
			C0, C1, C2, C3, C4, C5, C6, C7,
			D0, D1, D2, D3, D4, D5, D6, D7,
			E0, E1, E2, E3, E4, E5, E6, E7,
			F0, F1, F2, F3, F4, F5, F6, F7,
			G0, G1, G2, G3, G4, G5, G6, G7,
			H0, H1, H2, H3, H4, H5, H6, H7,

			FromPos,
			ToPos,

			WinState,
		};
	};

	namespace Board{
		const unsigned int Begin = Variable::A0;
		const unsigned int End = Variable::H7;

		const unsigned int Width = 8;
		const unsigned int Height = 8;

		const std::string NoChecker = "0";

		const std::string Player0Checker = "1";
		const std::string Player1Checker = "2";

		const std::string Player0King = "3";
		const std::string Player1King = "4";
	};

	namespace State{
		const std::string Player0 = "s0";
		const std::string Player1 = "s1";

		const std::string Server = "ss";
	};
};