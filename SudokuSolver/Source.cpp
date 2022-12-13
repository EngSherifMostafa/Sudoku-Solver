#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include "Generator.h"
#define GRID_SIZE 9
const char fileName[]{ "SolutionFile.txt" };

bool checkUserInput(int, int board[GRID_SIZE][GRID_SIZE], int, int);
void printBoard(int board[GRID_SIZE][GRID_SIZE], ofstream&);
bool isValidNumber(int board[GRID_SIZE][GRID_SIZE], int, int, int);
bool isValidRow(int board[GRID_SIZE][GRID_SIZE], int, int);
bool isValidColumn(int board[GRID_SIZE][GRID_SIZE], int, int);
bool isValidBox(int board[GRID_SIZE][GRID_SIZE], int, int, int);
bool solveBoard(int board[GRID_SIZE][GRID_SIZE]);
void createFile(int board[GRID_SIZE][GRID_SIZE], string, double timeTaken = 0.0);

int main() {
    bool invalid = false;
    int board[GRID_SIZE][GRID_SIZE]{ 0 }, choose = 0;
    Generator gen;

    //user choose
    do {
        try
        {
            std::cout <<
                "1- Generate board.\n" <<
                "2- Write your own board.\n" <<
                "(1 / 2) : ";

            std::cin >> choose;

            if (choose != 1 && choose != 2) throw exception();
        }

        catch (...)
        {
            std::cout << "Unexpected selection !!\n\n";
            std::cin.clear();
            std::cin.ignore(1e10, '\n');
            choose = 0;
        }
    } while (choose != 1 && choose != 2);

    switch (choose) {
    case 1:
        //generate new game using Generator class
        //class ref https://github.com/vaithak/Sudoku-Generator/blob/master/sudokuGen.cpp
        gen.generate(board);
        break;

    case 2:
        int temp = 0;
        do {
            invalid = false;
            std::cout << "\n\nEnter 9 X 9 game board (valid numbers are from 0 - 9 only where 0 means empty)\n";

            for (int row = 0; row < GRID_SIZE; row++) {
                for (int column = 0; column < GRID_SIZE; column++) {
                    std::cout << "( " << row + 1 << " , " << column + 1 << " ): ";
                    cin >> temp;
                    checkUserInput(temp, board, row, column);
                }
            }
        } while (invalid);
        break;
    }

    //system("color 34");
    createFile(board, "Game befor solution");
    solveBoard(board);
    auto timeTaken = clock(); //measure time of solving
    createFile(board, "Game after solution", timeTaken / CLOCKS_PER_SEC);

    system("pause > 0");
    return 0;
}

bool checkUserInput(int userNumber, int board[GRID_SIZE][GRID_SIZE], int row, int column) {
    if (userNumber < 0 || userNumber > 9)
        return false;

    board[row][column] = userNumber;
    
    //clear buffer
    std::cin.clear();
    std::cin.ignore(1e10, '\n');
    return true;
}

void printBoard(int board[GRID_SIZE][GRID_SIZE], ofstream& SolutionFile) {
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            if (board[row][column] == 0)
                SolutionFile << "  ";
            else
                SolutionFile << board[row][column] << " ";

            if ((column + 1) % 3 == 0)
                SolutionFile << "| ";
        }
        SolutionFile << std::endl;
        if ((row + 1) % 3 == 0)
            SolutionFile << "------+-------+--------" << std::endl;
    }
}

bool isValidNumber(int board[GRID_SIZE][GRID_SIZE], int number, int row, int column) {
    return (
        isValidRow(board, number, row) &&
        isValidColumn(board, number, column) &&
        isValidBox(board, number, row, column)
        );
}

bool isValidRow(int board[GRID_SIZE][GRID_SIZE], int number, int row) {
    for (int c = 0; c < GRID_SIZE; c++) {
        if (board[row][c] == number)
            return false;
    }
    return true;
}

bool isValidColumn(int board[GRID_SIZE][GRID_SIZE], int number, int column) {
    for (int r = 0; r < GRID_SIZE; r++) {
        if (board[r][column] == number)
            return false;
    }
    return true;
}

bool isValidBox(int board[GRID_SIZE][GRID_SIZE], int number, int row, int column) {
    int localBoxRow = row - row % 3;
    int localBoxColumn = column - column % 3;

    for (int r = localBoxRow; r < localBoxRow + 3; r++)
        for (int c = localBoxColumn; c < localBoxColumn + 3; c++)
            if (board[r][c] == number) return false;

    return true;
}

bool solveBoard(int board[GRID_SIZE][GRID_SIZE]) {
    static int step = 1;
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int column = 0; column < GRID_SIZE; column++) {
            if (board[row][column] == 0) {
                for (int num = 1; num <= GRID_SIZE; num++) {
                    if (isValidNumber(board, num, row, column)) {
                        board[row][column] = num;
                        createFile(board, "Step # " + std::to_string(step++));
                        //recursion ( Backtracking Algorithm )
                        if (solveBoard(board))
                            return true;
                        else
                            board[row][column] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void createFile(int board[GRID_SIZE][GRID_SIZE], string str, double timeTaken) {
    //object dt for date and time
    SYSTEMTIME dt;
    GetLocalTime(&dt);

    //object SolutionFile for create new txt file
    ofstream SolutionFile(fileName, std::ios::out | std::ios::app);

    if (str == "Game befor solution")
        SolutionFile
        << "\tBy Eng: Sherif M.Samy\n"
        << "==========================================\n"
        << "  Game was Created @ " << dt.wDay << "/" << dt.wMonth << "/" << dt.wYear << "\t"
        << dt.wHour << ":" << dt.wMinute << ":" << dt.wSecond
        << "  \n==========================================" << "\n\n"
        << "  " << str << "\n------+-------+--------\n";

    else if (str == "Game after solution")
        SolutionFile
        << "\n==========================================\n"
        << "  Game was Solved @ " << fixed << setprecision(5) << timeTaken << " Sec"
        << "  \n==========================================" << "\n\n"
        << "  " << str << "\n------+-------+--------\n";

    else
        SolutionFile << "\n  " << str << "\n------+-------+--------\n";

    printBoard(board, SolutionFile);
    SolutionFile.close();

    //display game from txt file
    system("type SolutionFile.txt");
}