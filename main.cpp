
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define EMPTY 0
#define EMPTY_VIS 1
#define STONE_O 2
#define STONE_O_VIS 3
#define STONE_X 4
#define STONE_X_VIS 5

std::vector<std::vector<uint8_t>> initBoard(uint32_t size) {
	std::vector<std::vector<uint8_t>> board(size, std::vector<uint8_t>(size));
	return board;
}

struct Args
{
	bool board;
	bool score;
	unsigned int size;
};

struct Points {
	int X = 0;
	int O = 0;
};

void printBoard(std::vector<std::vector<uint8_t>> board) {
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

uint32_t findFreedom(std::vector<std::vector<uint8_t>>* board, uint32_t x, uint32_t y, uint8_t stone) {
	
	(*board)[x][y]=stone+1;

	uint32_t freedom =  (x > 0 && 0 == (*board)[x - 1][y] && ++(*board)[x - 1][y])
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



int clearStones(std::vector<std::vector<uint8_t>>* board, uint32_t x, uint32_t y, uint8_t stone) {
	(*board)[x][y] = EMPTY;
	int sum = 1;

	if (x > 0 && stone == (*board)[x - 1][y])
		sum +=  clearStones(board, x - 1, y, stone) ;

	if (y > 0 && stone == (*board)[x][y - 1])
		sum += clearStones(board, x, y - 1, stone) ;

	if (x < (*board).size() - 1 && stone == (*board)[x + 1][y])
		sum += clearStones(board, x + 1, y, stone) ;

	if (y < (*board).size() - 1 && stone == (*board)[x][y + 1])
		sum += clearStones(board, x, y + 1, stone) ;

	return sum ;
}


int countGroupePoints(std::vector<std::vector<uint8_t>>* board, size_t x, size_t y, bool *pX, bool *pO) {
	(*board)[x][y] = EMPTY_VIS;
	int sum = 1;

	if (x > 0 && EMPTY == (*board)[x - 1][y])
		sum += countGroupePoints(board, x - 1, y, pX, pO);
	else if (x > 0 && STONE_O == (*board)[x - 1][y])
		*pO = true;
	else if (x > 0 && STONE_X == (*board)[x - 1][y])
		*pX = true;

	if (y > 0 && EMPTY == (*board)[x][y - 1])
		sum += countGroupePoints(board, x, y - 1, pX, pO);
	else if (y > 0 && STONE_O == (*board)[x][y - 1])
		*pO = true;
	else if (y > 0 && STONE_X == (*board)[x][y - 1])
		*pX = true;

	if (x < (*board).size() - 1 && EMPTY == (*board)[x + 1][y])
		sum += countGroupePoints(board, x + 1, y, pX, pO);
	else if (x < (*board).size() - 1 && STONE_O == (*board)[x + 1][y])
		*pO = true;
	else if (x < (*board).size() - 1 && STONE_X == (*board)[x + 1][y])
		*pX = true;

	if (y < (*board).size() - 1 && EMPTY == (*board)[x][y + 1])
		sum += countGroupePoints(board, x, y + 1, pX, pO);
	else if (y < (*board).size() - 1 && STONE_O == (*board)[x][y + 1])
		*pO = true;
	else if (y < (*board).size() - 1 && STONE_X == (*board)[x][y + 1])
		*pX = true;

	return sum;
}

void countPoints(std::vector<std::vector<uint8_t>>* board, Points *points) {
	bool pX = false, pO = false;
	int pts = 0;
	for (size_t i = 0; i < (*board).size(); i++) {
		for (size_t j = 0; j < (*board).size(); j++) {
			
			if ((*board)[i][j] == EMPTY) {
				pX = false;
				pO = false;

				pts = countGroupePoints(board, i, j, &pX, &pO);
				if (pX && !pO) {
					points->X += pts;
				}
				else if (!pX && pO) {
					points->O += pts;
				}

			}
				
		}
	}
}

void clearVisits(std::vector<std::vector<uint8_t>>* board) {
	for (size_t i = 0; i < (*board).size(); i++) {
		for (size_t j = 0; j < (*board).size(); j++) {
			if ((*board)[i][j] % 2 == 1)
				(*board)[i][j]--;
		}
	}
}



bool place(std::vector<std::vector<uint8_t>>* board, uint32_t x, uint32_t y, uint8_t stone, Points *points) {
	
	if ((*board)[x][y] != EMPTY) {
		//std::cout << "Spot is not	 empty\n";
		return false;
	}

	uint8_t invStone = stone == STONE_O ? STONE_X : STONE_O;
	std::vector<std::vector<uint8_t>> boardCpy = ( * board), boardCpy2 = (*board);
	int pts = 0;

	boardCpy[x][y] = stone + 1;

	bool cleared = false;


	if (x > 0 && boardCpy[x - 1][y] == invStone && findFreedom(&boardCpy, x - 1, y, invStone) == 0) {
		pts += clearStones(&boardCpy, x - 1,y, invStone+1) ;
		cleared = true;
	}
	clearVisits(&boardCpy);

	if (y > 0 && boardCpy[x][y - 1] == invStone && findFreedom(&boardCpy, x, y - 1, invStone) == 0) {
		pts += clearStones(&boardCpy, x, y-1, invStone+1) ;
		cleared = true;
	}
	clearVisits(&boardCpy);

	if (x < boardCpy.size() - 1 && boardCpy[x + 1][y] == invStone && findFreedom(&boardCpy, x + 1, y, invStone) == 0) {
		pts += clearStones(&boardCpy, x + 1,y, invStone+1) ;
		cleared = true;
	}
	clearVisits(&boardCpy);

	if (y < boardCpy.size() - 1 && boardCpy[x][y + 1] == invStone && findFreedom(&boardCpy, x, y + 1, invStone) == 0) {
		pts += clearStones(&boardCpy, x , y+1, invStone+1) ;
		cleared = true;
	}
	clearVisits(&boardCpy);

	if (!cleared && findFreedom(&boardCpy2, x, y, stone) == 0) {
		//std::cout << "Cant place stone\n";
		return false;
	}
	boardCpy[x][y] = stone;
	*board = boardCpy;
	if (stone == STONE_O)
		points->O += pts;
	if (stone == STONE_X)
		points->X += pts;

	return true;
}

size_t hash(std::vector<std::vector<uint8_t>> board) {
	size_t sum = 0;
	for (size_t i = 0; i < board.size(); i++) {
		for (size_t j = 0; j < board.size(); j++) {
			sum += (board[i][j]) * (i * board.size() + (j+1) );
		}
	}
	return sum % (board.size()* board.size() * 2 * (board.size() *board.size() + board.size()));
}


bool boardIsEq(std::vector<std::vector<uint8_t>> b1, std::vector<std::vector<uint8_t>> b2) {
	for (size_t i = 0; i < b1.size(); i++) {
		for (size_t j = 0; j < b2.size(); j++) {
			if (b1[i][j] != b2[i][j])
				return false;
		}
	}
	return true;
}



int main(int argc, char* argv[])
//int main()
{
	//Arguments handling
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
	catch (const std::exception&) {
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

	// game params
	std::string cmd;
	uint16_t end = 0;
	bool turn = true;
	unsigned int num = 0;
	Points points = {  };
	std::vector<int> pos = { 0,0 };

	std::vector<std::vector<uint8_t>> board = initBoard(arguments.size), boardCpy;


	// hash params
	std::vector<std::vector<std::vector<uint8_t>>> boardHash;
	std::map<size_t, std::vector<std::vector<std::vector<uint8_t>>>> hashTbl = { {hash(board) ,{ board}} };
	size_t hashVal = 0;
	bool hashrep = false;

	// game run
	while (end < 2) {

		std::cin >> cmd;

		if (cmd=="pass" && num == 0) {
			end += 1;
			num = 0;
			turn = !turn;
		}
		else {
			boardCpy = board;
			try
			{
				pos[num] = std::stoi(cmd);
			}
			catch (const std::exception&)
			{
				return EXIT_FAILURE;
			}
			if (pos[num] < 0 || pos[num] >= static_cast<int>( arguments.size)) {
				return EXIT_FAILURE;
			}

			if (num == 0) {
				num++;

			}else {
				num = 0;
				if (turn) {

					if (!place(&boardCpy, pos[0], pos[1], STONE_X, &points)) {
						//std::cout << pos[0] << " " << pos[1] << " " << turn << '\n';
						continue;
					}
				}
				else {
					if (!place(&boardCpy, pos[0], pos[1], STONE_O, &points)) {
						//std::cout << pos[0] << " " << pos[1] << " " << turn << '\n';
						continue;
					}
				}
				
				


				// repetition handling
				hashVal = hash(boardCpy);
				boardHash = hashTbl[hashVal];
				if (boardHash.size() <= 0) {
					hashTbl[hashVal].push_back( boardCpy);
				}
				else {
					hashrep = false;
					for (size_t b = 0; b < boardHash.size(); b++) {

						if (boardIsEq(boardCpy, boardHash[b])) {
							hashrep = true;
							break;
						}	
					}
					if (hashrep) continue;
					
					hashTbl[hashVal].push_back(boardCpy);
				}

				// successful turn
				//std::cout << pos[0] << " " << pos[1] << " " << turn << '\n';
				end = 0;
				turn = !turn;
				board = boardCpy;
				//printBoard(board);
				
			}
			

		}
	}

	if (arguments.board) {
		printBoard(board);
		
	}
	if (arguments.score) {

		countPoints(&board, &points);
		std::cout << points.X << " " << points.O << '\n';
	}



	return 0;
}

