
#include <iostream>
#include <vector>

#define EMPTY 0
#define STONE_O 1
#define STONE_O_VIS 2
#define STONE_X 3
#define STONE_X_VIS 4

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
			case STONE_O:
				std::cout << " O";
				break;
			case STONE_O_VIS:
				std::cout << " O";
				break;
			case STONE_X:
				std::cout << " X";
				break;
			case STONE_X_VIS:
				std::cout << " X";
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

	uint64_t freedom =  (x > 0 && 0 == (*board)[x - 1][y])
						+ (y > 0 && 0 == (*board)[x][y - 1])
						+ (x < (*board).size() - 1 && 0 == (*board)[x + 1][y])
						+ (y < (*board).size() - 1 && 0 == (*board)[x][y + 1]);

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



//int main(int argc, char* argv[])
int main()
{
	std::vector<std::vector<uint64_t>> board = initBoard(7), boardCpy = board;
	
	board[0][1] = STONE_O;
	board[2][4] = STONE_X;
	board[2][5] = STONE_X;
	printBoard(board);
	std::cout<< findFreedom(&boardCpy, 0, 0 , STONE_X) << '\n';
	printBoard(boardCpy);
	boardCpy = board;
	std::cout << findFreedom(&boardCpy, 2, 4, STONE_X) << '\n';
	printBoard(boardCpy);
	boardCpy = board;
	std::cout << findFreedom(&boardCpy, 2, 6, STONE_X) <<'\n';
	printBoard(boardCpy);


	return 0;
}

