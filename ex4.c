/**************************************************************
 * Exercise: Ex4 - Recursion
 * Author: Guy Kejzman
 * ID: kejzmag
 **************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 /***************************
 ******** Menu Items ********
 ****************************/

#define REVERSE 1
#define PALINDROME 2
#define SENTENCES 3
#define ZIP 4
#define SUDOKU 5
#define EXIT 6

 /***************************
 *** DIMENSION PARAMETERS ***
 ****************************/

#define LONGEST_TERM 20
#define LONGEST_SENTENCE 62
#define MAX_NUMBER_OF_TERMS 10

#define ZIP_MAX_GRID_SIZE 20

#define SUDOKU_GRID_SIZE 9
#define SUDOKU_SUBGRID_SIZE 3

 /***************************
 ******* STYLE CONSTS ********
 ****************************/

#define NUM_DIRECTIONS 4

 /***************************
 * USER INTERFACE PROTOTYPES *
 ****************************/

void task1ReversePhrase();
void task2CheckPalindrome();
void task3GenerateSentences();
void task4SolveZipBoard();
void task5SolveSudoku();

/****************************
* IMPLEMENTATION PROTOTYPES *
*****************************/

void task1ReversePhraseImplementation();
int task2CheckPalindromeImplementation(int);
void task3GenerateSentencesImplementation(char[][LONGEST_TERM + 1], int,
    char[][LONGEST_TERM + 1], int,
    char[][LONGEST_TERM + 1], int);
int task4SolveZipBoardImplementation(int[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    char[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int, int, int, int);
int task5SolveSudokuImplementation(int[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE]);

/******************************
* HELPER FUNCTIONS PROTOTYPES *
*******************************/

int readTerms(char[][LONGEST_TERM + 1], int, const char*);
void printSudoku(int[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE]);

/* Sudoku forward decl */
int solveRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE]);

/* Palindrome helpers */
static void readNRec(char* s, int i, int n);
static int palRec(const char* s, int l, int r);

/* Sentence helpers */
static void objectsRec(char subjects[][LONGEST_TERM + 1],
    char verbs[][LONGEST_TERM + 1],
    char objects[][LONGEST_TERM + 1],
    int si, int vi, int oi, int objectsCount, int* k);
static void verbsRec(char subjects[][LONGEST_TERM + 1],
    char verbs[][LONGEST_TERM + 1],
    char objects[][LONGEST_TERM + 1],
    int si, int vi, int verbsCount, int objectsCount, int* k);
static void subjectsRec(char subjects[][LONGEST_TERM + 1],
    char verbs[][LONGEST_TERM + 1],
    char objects[][LONGEST_TERM + 1],
    int si, int subjectsCount, int verbsCount,
    int objectsCount, int* k);

/* Zip helpers */
static int inBounds(int r, int c, int size);

static int tryDirections(int dir,
    int grid[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int visited[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int size, int r, int c, int step,
    int nextNum, int highest);

static int solveZipRec(int board[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int visited[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int size, int r, int c, int step,
    int nextNum, int highest);

/* Sudoku helpers */
static int isInRowRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int r, int c, int val);
static int isInColRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int r, int c, int val);
static int isInBoxRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int startR, int startC, int dr, int dc, int val);
static int isSafe(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int r, int c, int val);
static int findEmptyRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int idx, int* outR, int* outC);
static int tryValsRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int r, int c, int val);

/******************************
********** MAIN MENU **********
*******************************/

int main()
{
    int task;
    do
    {
        printf("Please choose a task (1-5) or 6 to exit:\n");
        printf("%d. Reverse a phrase\n", REVERSE);
        printf("%d. Check Palindrome\n", PALINDROME);
        printf("%d. Generate sentences\n", SENTENCES);
        printf("%d. Solve Zip Board\n", ZIP);
        printf("%d. Solve Sudoku\n", SUDOKU);
        printf("%d. Exit\n", EXIT);

        scanf(" %d", &task);
        scanf("%*c");

        switch (task)
        {
        case REVERSE:
            task1ReversePhrase();
            break;
        case PALINDROME:
            task2CheckPalindrome();
            break;
        case SENTENCES:
            task3GenerateSentences();
            break;
        case ZIP:
            task4SolveZipBoard();
            break;
        case SUDOKU:
            task5SolveSudoku();
            break;
        case EXIT:
            printf("Goodbye!\n");
            break;
        default:
            printf("Please choose a task number from the list.\n");
            break;
        }

        printf("\n=============================\n\n");
    } while (task != EXIT);

    return 0;
}

/***************************
****** USER INTERFACE ******
****************************/

void task1ReversePhrase()
{
    printf("Please insert the phrase to reverse:\n");
    task1ReversePhraseImplementation();
    printf("\n");
}

void task2CheckPalindrome()
{
    int n;

    printf("Please insert the phrase length:\n");
    scanf("%d", &n);

    printf("Please insert the phrase to check:\n");
    scanf("%*c"); /* eat exactly one newline, don't skip leading spaces */

    if (task2CheckPalindromeImplementation(n))
        printf("The phrase is a palindrome.\n");
    else
        printf("The phrase is not a palindrome.\n");
}

void task3GenerateSentences()
{
    char subjects[MAX_NUMBER_OF_TERMS][LONGEST_TERM + 1];
    char verbs[MAX_NUMBER_OF_TERMS][LONGEST_TERM + 1];
    char objects[MAX_NUMBER_OF_TERMS][LONGEST_TERM + 1];
    int subjectsCount, verbsCount, objectsCount;

    subjectsCount = readTerms(subjects, MAX_NUMBER_OF_TERMS, "subjects");
    verbsCount = readTerms(verbs, MAX_NUMBER_OF_TERMS, "verbs");
    objectsCount = readTerms(objects, MAX_NUMBER_OF_TERMS, "objects");

    printf("List of Sentences:\n");
    task3GenerateSentencesImplementation(subjects, subjectsCount,
        verbs, verbsCount,
        objects, objectsCount);
}

void task4SolveZipBoard()
{
    int size;
    int board[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE] = { 0 };
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE] = { 0 };
    int row = 0, col = 0;
    int highest = 0;

    printf("Please enter the board size:\n");
    scanf("%d", &size);

    if (size < 1 || size > ZIP_MAX_GRID_SIZE)
    {
        printf("Invalid board size.\n");
        return;
    }

    printf("Please enter the grid:\n");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            scanf("%d", &board[i][j]);

            if (board[i][j] == 1)
            {
                row = i;
                col = j;
            }

            if (board[i][j] > highest)
                highest = board[i][j];
        }
    }

    if (task4SolveZipBoardImplementation(board, solution, size, row, col, highest))
    {
        printf("Solution:\n");
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                printf("%c ", solution[i][j] ? solution[i][j] : 'X');
            }
            printf("\n");
        }
    }
    else
    {
        printf("No solution exists.\n");
    }
}

void task5SolveSudoku()
{
    int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE] = { 0 };

    printf("Please enter the sudoku board:\n");
    for (int i = 0; i < SUDOKU_GRID_SIZE; i++)
    {
        for (int j = 0; j < SUDOKU_GRID_SIZE; j++)
        {
            scanf("%d", &board[i][j]);
        }
    }

    if (task5SolveSudokuImplementation(board))
    {
        printf("Solution found:\n");
        printSudoku(board);
    }
    else
    {
        printf("No solution exists.\n");
    }
}

/***************************
********* HELPERS **********
****************************/

/* ---------- Palindrome helpers ---------- */

static void readNRec(char* s, int i, int n)
{
    if (i == n)
        return;

    s[i] = (char)getchar();
    readNRec(s, i + 1, n);
}

static int palRec(const char* s, int l, int r)
{
    if (l >= r)
        return 1;

    if (s[l] != s[r])
        return 0;

    return palRec(s, l + 1, r - 1);
}

/* ---------- Sentences helpers ---------- */

static void objectsRec(char subjects[][LONGEST_TERM + 1],
    char verbs[][LONGEST_TERM + 1],
    char objects[][LONGEST_TERM + 1],
    int si, int vi, int oi, int objectsCount, int* k)
{
    if (oi == objectsCount)
        return;

    printf("%d. %s %s %s\n", *k, subjects[si], verbs[vi], objects[oi]);
    (*k)++;

    objectsRec(subjects, verbs, objects, si, vi, oi + 1, objectsCount, k);
}

static void verbsRec(char subjects[][LONGEST_TERM + 1],
    char verbs[][LONGEST_TERM + 1],
    char objects[][LONGEST_TERM + 1],
    int si, int vi, int verbsCount, int objectsCount, int* k)
{
    if (vi == verbsCount)
        return;

    objectsRec(subjects, verbs, objects, si, vi, 0, objectsCount, k);
    verbsRec(subjects, verbs, objects, si, vi + 1, verbsCount, objectsCount, k);
}

static void subjectsRec(char subjects[][LONGEST_TERM + 1],
    char verbs[][LONGEST_TERM + 1],
    char objects[][LONGEST_TERM + 1],
    int si, int subjectsCount, int verbsCount,
    int objectsCount, int* k)
{
    if (si == subjectsCount)
        return;

    verbsRec(subjects, verbs, objects, si, 0, verbsCount, objectsCount, k);
    subjectsRec(subjects, verbs, objects, si + 1, subjectsCount,
        verbsCount, objectsCount, k);
}

/* ---------- Read terms ---------- */

int readTerms(char terms[][LONGEST_TERM + 1], int maxNumOfTerms, const char* type)
{
    int termsCount;

    printf("Please insert number of %s:\n", type);
    scanf("%d", &termsCount);

    if (termsCount < 1 || termsCount > maxNumOfTerms)
        termsCount = maxNumOfTerms;

    printf("Please insert the list of %s:\n", type);
    for (int i = 0; i < termsCount; i++)
    {
        printf("%d. ", i + 1);
        scanf(" %[^\n]", terms[i]);
    }

    return termsCount;
}

/* ---------- Zip helpers ---------- */

static int inBounds(int r, int c, int size)
{
    return (r >= 0 && r < size && c >= 0 && c < size);
}

static int solveZipRec(int board[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int visited[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int size, int r, int c, int step,
    int nextNum, int highest)
{
   if (step == size * size) {
       if (nextNum == highest + 1 && board[r][c] == highest) {
           solution[r][c] = 'X';
           return 1;
       }
       return 0;
   }

    if (tryDirections(0, board, solution, visited, size,
        r, c, step, nextNum, highest))
    {
        return 1;
    }

    visited[r][c] = 0;
    return 0;
}

static int tryDirections(int dir,
    int grid[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int visited[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int size, int r, int c, int step,
    int nextNum, int highest)
{
    int dr, dc;
    char move;
    int nr, nc;
    int val;
    int newNext;

    if (dir == NUM_DIRECTIONS)
        return 0;

    if (dir == 0)
    {
        dr = -1;
        dc = 0;
        move = 'U';
    }
    else if (dir == 1)
    {
        dr = 1;
        dc = 0;
        move = 'D';
    }
    else if (dir == 2)
    {
        dr = 0;
        dc = -1;
        move = 'L';
    }
    else
    {
        dr = 0;
        dc = 1;
        move = 'R';
    }

    nr = r + dr;
    nc = c + dc;

    if (inBounds(nr, nc, size) && !visited[nr][nc])
    {
        val = grid[nr][nc];

        if (val == 0 || val == nextNum)
        {
            solution[r][c] = move;

            newNext = nextNum;
            if (val == nextNum)
                newNext++;

            visited[nr][nc] = 1;

            if (solveZipRec(grid, solution, visited, size,
                nr, nc, step + 1, newNext, highest))
            {
                return 1;
            }

            visited[nr][nc] = 0;
            solution[r][c] = 0;
        }
    }

    return tryDirections(dir + 1, grid, solution, visited,
        size, r, c, step, nextNum, highest);
}

/* ---------- Sudoku helpers ---------- */

static int isInRowRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int r, int c, int val)
{
    if (c == SUDOKU_GRID_SIZE)
        return 0;

    if (board[r][c] == val)
        return 1;

    return isInRowRec(board, r, c + 1, val);
}

static int isInColRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int r, int c, int val)
{
    if (r == SUDOKU_GRID_SIZE)
        return 0;

    if (board[r][c] == val)
        return 1;

    return isInColRec(board, r + 1, c, val);
}

static int isInBoxRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int startR, int startC, int dr, int dc, int val)
{
    if (dr == SUDOKU_SUBGRID_SIZE)
        return 0;

    if (dc == SUDOKU_SUBGRID_SIZE)
        return isInBoxRec(board, startR, startC, dr + 1, 0, val);

    if (board[startR + dr][startC + dc] == val)
        return 1;

    return isInBoxRec(board, startR, startC, dr, dc + 1, val);
}

static int isSafe(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int r, int c, int val)
{
    int boxR = (r / SUDOKU_SUBGRID_SIZE) * SUDOKU_SUBGRID_SIZE;
    int boxC = (c / SUDOKU_SUBGRID_SIZE) * SUDOKU_SUBGRID_SIZE;

    if (isInRowRec(board, r, 0, val))
        return 0;

    if (isInColRec(board, 0, c, val))
        return 0;

    if (isInBoxRec(board, boxR, boxC, 0, 0, val))
        return 0;

    return 1;
}

static int findEmptyRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int idx, int* outR, int* outC)
{
    int r;
    int c;

    if (idx == SUDOKU_GRID_SIZE * SUDOKU_GRID_SIZE)
        return 0;

    r = idx / SUDOKU_GRID_SIZE;
    c = idx % SUDOKU_GRID_SIZE;

    if (board[r][c] == 0)
    {
        *outR = r;
        *outC = c;
        return 1;
    }

    return findEmptyRec(board, idx + 1, outR, outC);
}

static int tryValsRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
    int r, int c, int val)
{
    if (val == (SUDOKU_GRID_SIZE + 1))
        return 0;

    if (isSafe(board, r, c, val))
    {
        board[r][c] = val;

        if (solveRec(board))
            return 1;
    }
    else
    {
        return tryValsRec(board, r, c, val + 1);
    }

    board[r][c] = 0;
    return tryValsRec(board, r, c, val + 1);
}

/* ---------- Sudoku solver ---------- */

int solveRec(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE])
{
    int r, c;

    if (!findEmptyRec(board, 0, &r, &c))
        return 1;

    return tryValsRec(board, r, c, 1);
}

/* ---------- Output ---------- */

void printSudoku(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE])
{
    printf("+-------+-------+-------+\n");
    for (int i = 0; i < SUDOKU_GRID_SIZE; i++)
    {
        printf("| ");
        for (int j = 0; j < SUDOKU_GRID_SIZE; j++)
        {
            if (board[i][j] == 0)
                printf(". ");
            else
                printf("%d ", board[i][j]);

            if ((j + 1) % SUDOKU_SUBGRID_SIZE == 0)
                printf("| ");
        }
        printf("\n");

        if ((i + 1) % SUDOKU_SUBGRID_SIZE == 0)
            printf("+-------+-------+-------+\n");
    }
}

/***************************
*********** TODO ***********
****************************/

void task1ReversePhraseImplementation()
{
    int ch = getchar();

    if (ch == '\n' || ch == EOF)
    {
        printf("The reversed phrase is:\n");
        return;
    }

    task1ReversePhraseImplementation();
    putchar(ch);
}

int task2CheckPalindromeImplementation(int length)
{
    char* buf;
    int res;

    if (length <= 1)
    {
        if (length == 1)
            getchar();
        return 1;
    }

    buf = (char*)malloc((length + 1) * sizeof(char));
    if (!buf)
        return 0;

    readNRec(buf, 0, length);
    buf[length] = '\0';

    res = palRec(buf, 0, length - 1);
    free(buf);

    return res;
}

void task3GenerateSentencesImplementation(char subjects[][LONGEST_TERM + 1],
    int subjectsCount,
    char verbs[][LONGEST_TERM + 1],
    int verbsCount,
    char objects[][LONGEST_TERM + 1],
    int objectsCount)
{
    int k = 1;
    subjectsRec(subjects, verbs, objects, 0, subjectsCount,
        verbsCount, objectsCount, &k);
}

int task4SolveZipBoardImplementation(int board[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
    int size, int startR, int startC, int highest)
{
    int visited[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE] = { 0 };

    visited[startR][startC] = 1;

    return solveZipRec(board, solution, visited, size,
        startR, startC, 1, 2, highest);
}

int task5SolveSudokuImplementation(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE])
{
    return solveRec(board);
}