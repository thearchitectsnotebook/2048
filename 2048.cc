#include <iostream>
#include <conio.h>
#include <time.h>
using namespace std;

/* FUNCTIONS */

void introduction();                                            // prints introduction
void gameOver();                                                // prints game over

bool shiftGridUp();                                             // shifts entire grid up
bool shiftGridDown();                                           // shifts entire grid down
bool shiftGridLeft();                                           // shifts entire grid left
bool shiftGridRight();                                          // shifts entire grid right

bool shiftBlockUp(int i, int j);                                // shifts block to upmost square it can go OR merges if hits block of same magnitude
bool shiftBlockDown(int i, int j);                              // shifts block to downmost square it can go OR merges if hits block of same magnitude
bool shiftBlockLeft(int i, int j);                              // shifts block to leftmost square it can go OR merges if hits block of same magnitude
bool shiftBlockRight(int i, int j);                             // shifts block to rightmost square it can go OR merges if hits block of same magnitude

void mergeBlocks(int i1, int j1, int i2, int j2);               // merges two blocks, where (i1, j1) will be removed and (i2, j2) will be doubled
void generateNumberAndPrint();                                  // generates a random number at a random open location AND prints grid out
void generateNumber();                                          // generates a random number at a random open location
void printGrid();                                               // prints grid on screen
void resetGrid();                                               // resets the grid to all empty squares
bool isMergable();                                              // returns if grid is able to merge in any direction
bool gridIsFull();                                              // returns if grid is full
void newLine();                                                 // cancels user input

/* GLOBAL VARIABLES */

int _grid[4][4] = {0};


int main()
{
    srand(time(0));

    introduction();

    char c = 0;
    while(1)
    {
        c = getch();
        switch(c)
        {
            case 72: 
                if (shiftGridUp()) 
                    generateNumberAndPrint();
                break;
            case 80: 
                if (shiftGridDown())
                    generateNumberAndPrint();
                break;
            case 75: 
                if (shiftGridLeft())
                    generateNumberAndPrint();
                break;
            case 77:
                if (shiftGridRight())
                    generateNumberAndPrint();
                break;
            case 27:
                exit(1);
                break;
            default: break;
        }

        if (gridIsFull() && !isMergable())
        {
            gameOver();
            break;
        }
    }

    return 0;
}

void introduction()
{
    cout << " .----------------.  .----------------.  .----------------.  .----------------. " << endl;
    cout << "| .--------------. || .--------------. || .--------------. || .--------------. |" << endl;
    cout << "| |    _____     | || |     ____     | || |   _    _     | || |     ____     | |" << endl;
    cout << "| |   / ___ `.   | || |   .'    '.   | || |  | |  | |    | || |   .' __ '.   | |" << endl;
    cout << "| |  |_/___) |   | || |  |  .--.  |  | || |  | |__| |_   | || |   | (__) |   | |" << endl;
    cout << "| |   .'____.'   | || |  | |    | |  | || |  |____   _|  | || |   .`____'.   | |" << endl;
    cout << "| |  / /____     | || |  |  `--'  |  | || |      _| |_   | || |  | (____) |  | |" << endl;
    cout << "| |  |_______|   | || |   '.____.'   | || |     |_____|  | || |  `.______.'  | |" << endl;
    cout << "| |              | || |              | || |              | || |              | |" << endl;
    cout << "| '--------------' || '--------------' || '--------------' || '--------------' |" << endl;
    cout << " '----------------'  '----------------'  '----------------'  '----------------' " << endl << endl;
    cout << "Welcome to 2048: C++ Console Style.\nPress the Esc key anytime to quit." << endl << endl;
    generateNumberAndPrint();
}

void gameOver()
{
    char tryAgain;
    cout << "Game over! Try again? (Y/N) ";
    cin >> tryAgain;

    while (cin.fail() || (tryAgain != 'y' && tryAgain != 'Y' && tryAgain != 'n' && tryAgain != 'N'))
    {
        cout << "Invalid Input!" << endl << "Try again? (Y/N) ";
        cin.clear();
        newLine();
        cin >> tryAgain;
    }
    newLine();

    if (tryAgain == 'y' || tryAgain == 'Y')
    {
        resetGrid();
        main();
        return;
    }
    
    exit(1);
}

bool shiftGridUp()
{
    bool shifted = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (_grid[i][j] != 0)
            {
                if (shiftBlockUp(i, j)) shifted = true;
            }
        }
    }
    return shifted;
}

bool shiftGridDown()
{
    bool shifted = false;
    for (int i = 3; i >= 0; i--)
    {
        for (int j = 0; j < 4; j++)
        {
            if (_grid[i][j] != 0)
            {
                if (shiftBlockDown(i, j)) shifted = true;
            }
        }
    }
    return shifted;
}

bool shiftGridLeft()
{
    bool shifted = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (_grid[i][j] != 0)
            {
                if (shiftBlockLeft(i, j)) shifted = true;
            }
        }
    }
    return shifted;
}

bool shiftGridRight()
{
    bool shifted = false;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 3; j >= 0; j--)
        {
            if (_grid[i][j] != 0)
            {
                if (shiftBlockRight(i, j)) shifted = true;
            }
        }
    }
    return shifted;
}

bool shiftBlockUp(int i, int j)
{
    if (i == 0) return false;

    int finalRow = i - 1;
    bool shift = false;
    bool merge = false;

    // if two blocks are the same when right next to each other, merge them
    if (_grid[finalRow][j] == _grid[i][j])
    {
        merge = true;
        mergeBlocks(i, j, finalRow, j);
    }

    // but if the block above is empty, continue going up until a non-empty block is found or you hit the last row
    while (_grid[finalRow][j] == 0)
    {
        shift = true;
        if (finalRow == 0) break;
        if (_grid[finalRow-1][j] == _grid[i][j])
        {
            mergeBlocks(i, j, finalRow-1, j);
            break;
        }
        if (_grid[finalRow-1][j] != 0) break;
        else finalRow--;
    }

    // perform the shift
    if (shift)
    {
        _grid[finalRow][j] = _grid[i][j];
        _grid[i][j] = 0;
    }

    // if a shift or merge were performed, return true. else, false.
    if (shift || merge) return true;
    return false;
}

bool shiftBlockDown(int i, int j)
{
    if (i == 3) return false;

    int finalRow = i + 1;
    bool shift = false;
    bool merge = false;

    // if two blocks are the same when right next to each other, merge them
    if (_grid[finalRow][j] == _grid[i][j])
    {
        merge = true;
        mergeBlocks(i, j, finalRow, j);
    }

    // but if the block below is empty, continue going down until a non-empty block is found or you hit the last row
    while (_grid[finalRow][j] == 0)
    {
        shift = true;
        if (finalRow == 3) break;
        if (_grid[finalRow+1][j] == _grid[i][j])
        {
            mergeBlocks(i, j, finalRow+1, j);
            break;
        }
        if (_grid[finalRow+1][j] != 0) break;
        else finalRow++;
    }

    // perform the shift
    if (shift)
    {
        _grid[finalRow][j] = _grid[i][j];
        _grid[i][j] = 0;
    }

    // if a shift or merge were performed, return true. else, false.
    if (shift || merge) return true;
    return false;
}

bool shiftBlockLeft(int i, int j)
{
    if (j == 0) return false;

    int finalCol = j - 1;
    bool shift = false;
    bool merge = false;

    // if two blocks are the same when right next to each other, merge them
    if (_grid[i][finalCol] == _grid[i][j])
    {
        merge = true;
        mergeBlocks(i, j, i, finalCol);
    }

    // but if the block to the left is empty, continue going left until a non-empty block is found or you hit the last col
    while (_grid[i][finalCol] == 0)
    {
        shift = true;
        if (finalCol == 0) break;
        if (_grid[i][finalCol-1] == _grid[i][j])
        {
            mergeBlocks(i, j, i, finalCol-1);
            break;
        }
        if (_grid[i][finalCol-1] != 0) break;
        else finalCol--;
    }

    // perform the shift
    if (shift)
    {
        _grid[i][finalCol] = _grid[i][j];
        _grid[i][j] = 0;
    }

    // if a shift or merge were performed, return true. else, false.
    if (shift || merge) return true;
    return false;
}

bool shiftBlockRight(int i, int j)
{
    if (j == 3) return false;

    int finalCol = j + 1;
    bool shift = false;
    bool merge = false;

    // if two blocks are the same when right next to each other, merge them
    if (_grid[i][finalCol] == _grid[i][j])
    {
        merge = true;
        mergeBlocks(i, j, i, finalCol);
    }

    // but if the block to the right is empty, continue going right until a non-empty block is found or you hit the last col
    while (_grid[i][finalCol] == 0)
    {
        shift = true;
        if (finalCol == 3) break;
        if (_grid[i][finalCol+1] == _grid[i][j])
        {
            mergeBlocks(i, j, i, finalCol+1);
            break;
        }
        if (_grid[i][finalCol+1] != 0) break;
        else finalCol++;
    }

    // perform the shift
    if (shift)
    {
        _grid[i][finalCol] = _grid[i][j];
        _grid[i][j] = 0;
    }

    // if a shift or merge were performed, return true. else, false.
    if (shift || merge) return true;
    return false;
}

void mergeBlocks(int i1, int j1, int i2, int j2)
{
    _grid[i2][j2] *= 2;
    _grid[i1][j1] = 0;
}

void generateNumberAndPrint()
{
    generateNumber();
    printGrid();
}

void generateNumber()
{
    int row = (rand() % 4);
    int col = (rand() % 4);

    while (_grid[row][col] != 0)
    {
        row = (rand() % 4);
        col = (rand() % 4);
    }

    _grid[row][col] = ((rand() % 2) + 1) * 2;
}

void printGrid()
{
    for (int i = 0; i < 4; i++)
    {
        cout << "-----------------------------\n|      |      |      |      |" << endl;
        for (int j = 0; j < 4; j++)
        {
            cout << "| ";
            if (_grid[i][j] == 0) cout << "     ";
            else if (_grid[i][j] >= 1000) cout << _grid[i][j] << " ";
            else if (_grid[i][j] >= 100) cout << " " << _grid[i][j] << " ";
            else if (_grid[i][j] >= 10) cout << " " << _grid[i][j] << "  ";
            else cout << "  " << _grid[i][j] << "  ";
        }
        cout << "|\n|      |      |      |      |" << endl;
    }
    cout << "-----------------------------" << endl;
}

void resetGrid()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            _grid[i][j] = 0;
        }
    }
}

bool isMergable()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (_grid[i][j] == _grid[i][j+1]) return true;
        }
    }

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 3; i++)
        {
            if (_grid[i][j] == _grid[i+1][j]) return true;
        }
    }

    return false;
}

bool gridIsFull()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (_grid[i][j] == 0) return false;
        }
    }
    return true;
}

void newLine()
{
    char c;
    do
    {
        cin.get(c);
    } while (c != '\n');
}