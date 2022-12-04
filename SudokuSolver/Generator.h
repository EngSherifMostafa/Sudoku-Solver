#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define GRID_SIZE 9
#define UNASSIGNED 0

using namespace std;

class Generator {
private:
	bool grid_status;
	int grid[GRID_SIZE][GRID_SIZE];
	int solnGrid[9][9];
	int guessNum[9];
	int gridPos[81];
	int difficultyLevel;

	Generator(string, bool row_major = true);
	void fillEmptyDiagonalBox(int);
	void createSeed();
	void printGrid();
	bool solveGrid();
	string getGrid();
	void countSoln(int& number);
	void genPuzzle();
	bool verifyGridStatus();
	void printSVG(string);
	void calculateDifficulty();
	int  branchDifficultyScore();

public:
	Generator();
	void generate(int board[GRID_SIZE][GRID_SIZE]);
};