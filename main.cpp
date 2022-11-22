
#include <iostream>
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

void printBoard(std::vector<std::vector<uint64_t>> board) {
	for (size_t i = 0; i < board.size(); i++)
	{
		/*if (i == 0) {
			std::cout << " ";
		}
		else {
			std::cout << i;
		}*/

		for (size_t j = 0; j < board.size(); j++) {
			/*if (i == 0) {
				std::cout << " " << j;
				continue;
			}*/
				
			switch (board[i][j]%10)
			{
			case EMPTY:
				std::cout << " .";
				break;
			case EMPTY_VIS:
				std::cout << " ,";
				break;
			case STONE_O:
				std::cout << " O";
				break;
			case STONE_O_VIS:
				std::cout << " o";
				break;
			case STONE_X:
				std::cout << " X";
				break;
			case STONE_X_VIS:
				std::cout << " x";
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



void place(std::vector<std::vector<uint64_t>>* board, uint64_t x, uint64_t y, uint16_t stone) {
	
	if ((*board)[x][y] != EMPTY) {
		std::cout << "Spot is not empty\n";
		return;
	}

	uint16_t invStone = stone == STONE_O ? STONE_X : STONE_O;
	(*board)[x][y] = stone + 1;
	bool cleared = false;

	if (x > 0 && (*board)[x - 1][y] == invStone && findFreedom(board, x - 1, y, invStone) == 0) {
		std::cout << "DELx-1\n";
		clearStones(board, x - 1,y, invStone+1);
		cleared = true;
	}
		

	if (y > 0 && (*board)[x][y - 1] == invStone && findFreedom(board, x, y - 1, invStone) == 0) {
		std::cout << "DELy-1\n";
		clearStones(board, x, y-1, invStone+1);
		cleared = true;
	}

	if (x < (*board).size() - 1 && (*board)[x + 1][y] == invStone && findFreedom(board, x + 1, y, invStone) == 0) {
		std::cout << "DELx+1\n";
		clearStones(board, x + 1,y, invStone+1);
		cleared = true;
	}

	if (y < (*board).size() - 1 && (*board)[x][y + 1] == invStone && findFreedom(board, x, y + 1, invStone) == 0) {
		std::cout << "DELy+1\n";
		clearStones(board, x , y+1, invStone+1);
		cleared = true;
	}

	if (!cleared && findFreedom(board, x, y, stone) == 0) {
		(*board)[x][y] = EMPTY;
		std::cout << "Cant place stone\n";
		return;
	}
}


//int main(int argc, char* argv[])
int main()
{
	std::vector<std::vector<uint64_t>> test(6, std::vector<uint64_t>(6));
	test[0] = { 2, 2, 2, 2, 2, 2 };
	test[1] = { 2, 4, 4, 4, 4, 2 };
	test[2] = { 2, 4, 2, 2, 4, 2 };
	test[3] = { 2, 4, 0, 2, 4, 2 };
	test[4] = { 2, 4, 4, 4, 4, 2 };
	test[5] = { 2, 2, 0, 2, 2, 2 };
	//printBoard(test);

	std::vector<std::vector<uint64_t>> board = initBoard(7), boardCpy;

	boardCpy = test;
	printBoard(boardCpy);
	place(&boardCpy, 3, 2, STONE_O);
	printBoard(boardCpy);

	/*
	board[0][1] = STONE_O;
	board[2][4] = STONE_X;
	board[1][6] = STONE_O;
	board[2][5] = STONE_O;
	board[3][5] = STONE_X;
	board[1][5] = STONE_X;
	board[4][5] = STONE_X;
	board[5][6] = STONE_X;
	board[4][6] = STONE_O;
	board[3][6] = STONE_O;*/

	//printBoard(board);
	//boardCpy = board;
	//std::cout<< findFreedom(&boardCpy, 0, 0 , STONE_X) << '\n';
	//printBoard(boardCpy);
	//boardCpy = board;
	//std::cout << findFreedom(&boardCpy, 2, 4, STONE_X) << '\n';
	//printBoard(boardCpy);
	//std::cout << findFreedom(&boardCpy, 2, 5, STONE_O) <<'\n';
	



	return 0;
}

