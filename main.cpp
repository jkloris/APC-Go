
#include <iostream>
#include <string>
#include <vector>

#define EMPTY 0
#define EMPTY_VIS 1
#define STONE_O 2
#define STONE_O_VIS 3
#define STONE_X 4
#define STONE_X_VIS 5

std::vector<std::vector<uint64_t>> initBoard(uint64_t size) {
	std::vector<std::vector<uint64_t>> board(size, std::vector<uint64_t>(size));
	return board;
}

struct Args
{
	bool board;
	bool score;
	unsigned int size;
};

void printBoard(std::vector<std::vector<uint64_t>> board) {
	for (size_t i = 0; i < board.size(); i++)
	{

		for (size_t j = 0; j < board.size(); j++) {

				
			switch (board[i][j]%10)
			{
			case EMPTY:
				std::cout << ".";
				break;
			case EMPTY_VIS:
				std::cout << ",";
				break;
			case STONE_O:
				std::cout << "O";
				break;
			case STONE_O_VIS:
				std::cout << "o";
				break;
			case STONE_X:
				std::cout << "X";
				break;
			case STONE_X_VIS:
				std::cout << "x";
				break;
			default:
				break;
			}
		}
		std::cout << '\n';
	}
}

uint64_t findFreedom(std::vector<std::vector<uint64_t>>* board, uint64_t x, uint64_t y, uint16_t stone) {
	
	(*board)[x][y]=stone+1;

	uint64_t freedom =  (x > 0 && 0 == (*board)[x - 1][y] && ++(*board)[x - 1][y])
						+ (y > 0 && 0 == (*board)[x][y - 1] && ++(*board)[x][y - 1])
						+ (x < (*board).size() - 1 && 0 == (*board)[x + 1][y] && ++(*board)[x + 1][y])
						+ (y < (*board).size() - 1 && 0 == (*board)[x][y + 1] && ++(*board)[x][y + 1]);

	if (x > 0 && stone == (*board)[x - 1][y])
		freedom += findFreedom(board, x - 1, y, stone);

	if (y > 0 && stone == (*board)[x][y-1])
		freedom += findFreedom(board, x , y-1, stone);

	if (x < (*board).size() - 1 && stone == (*board)[x + 1][y])
		freedom += findFreedom(board, x + 1, y, stone);

	if (y < (*board).size() - 1 && stone == (*board)[x][y + 1])
		freedom += findFreedom(board, x , y+1, stone);

	return freedom;
}

void clearStones(std::vector<std::vector<uint64_t>>* board, uint64_t x, uint64_t y, uint16_t stone) {
	(*board)[x][y] = 0;

	if (x > 0 && stone == (*board)[x - 1][y])
		clearStones(board, x - 1, y, stone);

	if (y > 0 && stone == (*board)[x][y - 1])
		clearStones(board, x, y - 1, stone);

	if (x < (*board).size() - 1 && stone == (*board)[x + 1][y])
		clearStones(board, x + 1, y, stone);

	if (y < (*board).size() - 1 && stone == (*board)[x][y + 1])
		clearStones(board, x, y + 1, stone);
}

void clearVisits(std::vector<std::vector<uint64_t>>* board) {
	for (size_t i = 0; i < (*board).size(); i++) {
		for (size_t j = 0; j < (*board).size(); j++) {
			if ((*board)[i][j] % 2 == 1)
				(*board)[i][j]--;
		}
	}
}



bool place(std::vector<std::vector<uint64_t>>* board, uint64_t x, uint64_t y, uint16_t stone) {
	
	if ((*board)[x][y] != EMPTY) {
		//std::cout << "Spot is not empty\n";
		return false;
	}

	uint16_t invStone = stone == STONE_O ? STONE_X : STONE_O;
	std::vector<std::vector<uint64_t>> boardCpy = ( * board), boardCpy2 = (*board);
	//printBoard(*board);

	boardCpy[x][y] = stone + 1;
	//std::cout << "board:\n";
	//printBoard(*board);
	//std::cout << "cpy:\n";
	//printBoard(boardCpy);
	bool cleared = false;


	if (x > 0 && boardCpy[x - 1][y] == invStone && findFreedom(&boardCpy, x - 1, y, invStone) == 0) {
		clearStones(&boardCpy, x - 1,y, invStone+1);
		cleared = true;
	}
	clearVisits(&boardCpy);

	if (y > 0 && boardCpy[x][y - 1] == invStone && findFreedom(&boardCpy, x, y - 1, invStone) == 0) {
		clearStones(&boardCpy, x, y-1, invStone+1);
		cleared = true;
	}
	clearVisits(&boardCpy);

	if (x < boardCpy.size() - 1 && boardCpy[x + 1][y] == invStone && findFreedom(&boardCpy, x + 1, y, invStone) == 0) {
		clearStones(&boardCpy, x + 1,y, invStone+1);
		cleared = true;
	}
	clearVisits(&boardCpy);

	if (y < boardCpy.size() - 1 && boardCpy[x][y + 1] == invStone && findFreedom(&boardCpy, x, y + 1, invStone) == 0) {
		clearStones(&boardCpy, x , y+1, invStone+1);
		cleared = true;
	}
	clearVisits(&boardCpy);

	if (!cleared && findFreedom(&boardCpy2, x, y, stone) == 0) {
		//std::cout << "Cant place stone\n";
		//boardCpy[x][y] = EMPTY;
		return false;
	}
	boardCpy[x][y] = stone;
	*board = boardCpy;
	
	return true;
}




int main(int argc, char* argv[])
//int main()
{
	if (argc < 2 || argc > 4)
		return EXIT_FAILURE;

	Args arguments = {};

	uint32_t a;
	try {
		for (char* c = argv[1]; *c; c++)
			if (!std::isdigit(*c))
				throw(std::runtime_error(argv[3]));

		a = std::stoul(argv[1]);
		if (a <= 0)
			throw(std::runtime_error(argv[3]));
	}
	catch (std::exception) {
		return EXIT_FAILURE;
	}
	arguments.size = a;


	for (int i = 2; i < argc; i++) {
		std::string brd = "--board";
		std::string scr = "--score";
		if (brd.compare(argv[i])==0 && !arguments.board) {
			arguments.board = true;
		}
		else if (scr.compare(argv[i]) == 0 && !arguments.score) {
			arguments.score = true;
		}
		else {
			return EXIT_FAILURE;
		}
	}


	
	std::string cmd;
	uint16_t end = 0;
	bool turn = true;
	unsigned int num = 0;
	std::vector<unsigned int> pos = { 0,0 };

	std::vector<std::vector<uint64_t>> board = initBoard(arguments.size);

	while (end < 2) {

		std::cin >> cmd;

		if (cmd=="pass" && num == 0) {
			end += 1;
			num = 0;
			turn = !turn;
		}
		else {
			

			try
			{
				pos[num] = std::stoi(cmd);
			}
			catch (const std::exception&)
			{
				return EXIT_FAILURE;
			}
			if (pos[num] < 0 || pos[num] >= arguments.size) {
				return EXIT_FAILURE;
			}


			if (num == 0) {
				num++;

			}else {
				num = 0;
				if (turn) {

					if (!place(&board, pos[0], pos[1], STONE_X)) {
						//turn = !turn;

						//std::cout << pos[0] << " " << pos[1] << " " << turn << '\n';
						continue;
					}
				}
				else {
					if (!place(&board, pos[0], pos[1], STONE_O)) {
						//turn = !turn;
						//std::cout << pos[0] << " " << pos[1] << " " << turn << '\n';
						continue;
					}
				}
				turn = !turn;
				//std::cout << pos[0] << " " << pos[1] << " " << turn << '\n';
				//printBoard(board);

				end = 0;
			}
			

		}
	}
	if (arguments.board) {
		printBoard(board);
	}



	return 0;
}

