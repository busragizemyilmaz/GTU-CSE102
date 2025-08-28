#include <stdio.h>
#include <stdlib.h>

typedef enum { empty, user, computer } Cell;

typedef struct {
    int row;
    int col;
} Move;

typedef struct {
    Cell **cells;
    int size;
} Board;

Cell **allocate(int size) 
{
    int i;

    Cell **board = (Cell **)malloc(size * sizeof(Cell *));

    for (i = 0; i < size; i++) 
    {
        board[i] = (Cell *)malloc(size * sizeof(Cell));
    }

    return board;
}

void freeBoard(Board *board) 
{
    int i;
    
    for (i = 0; i < board->size; i++) 
    {
        free(board->cells[i]);
    }
    free(board->cells);
}

void initializeBoard(Board *board) 
{
    int mid = board->size / 2;
    int i, j;

    for (i = 0; i < board->size; i++) 
    {
        for (j = 0; j < board->size; j++) 
        {
            board->cells[i][j] = empty;  /* Initialize all cells as empty */
        }
    }

    /* Place the initial four pieces in the center */
    board->cells[mid - 1][mid - 1] = computer;
    board->cells[mid - 1][mid] = user;
    board->cells[mid][mid - 1] = user;
    board->cells[mid][mid] = computer;
}

void printBoard(Board *board) 
{
    int i, j;

    printf("\n  ");
    for (i = 0; i < board->size; i++) 
    {
        printf("%c", 'a' + i);
    }
    printf("\n");

    for (i = 0; i < board->size; i++) 
    {
        printf("%d ", i + 1);

        for (j = 0; j < board->size; j++) 
        {
            if (board->cells[i][j] == user) 
            {
                printf("O");
            } 
            
            else if (board->cells[i][j] == computer)
            {
                printf("X");
            } 
            
            else 
            {
                printf(".");
            }
        }

        printf("\n");
    }
    printf("\n");
}

int isOnBoard(int row, int col, int size) 
{
    return row >= 0 && row < size && col >= 0 && col < size;
}

int checkDirection(Board *board, int row, int col, int dx, int dy, Cell player, int flip) 
{
    int x = row + dx;
    int y = col + dy;
    Cell opponent;
    int count = 0;

    if (player == user)
    {
        opponent = computer;
    }

    else
    {
        opponent = user;
    }

    /* Check if there are opponent pieces to flip */
    while (isOnBoard(x, y, board->size) && board->cells[x][y] == opponent) 
    {
        x += dx;
        y += dy;

        count++;
    }

    /* Flip if valid move and flip == 1 */
    if (count > 0 && isOnBoard(x, y, board->size) && board->cells[x][y] == player) 
    {
        if (flip) 
        {
            x = row + dx;
            y = col + dy;

            while (count--) 
            {
                board->cells[x][y] = player;

                x += dx;
                y += dy;
            }
        }

        return 1;
    }

    return 0;
}

int isValidMove(Board *board, int row, int col, Cell player) 
{
    int dx;
    int dy;
    
    if (!isOnBoard(row, col, board->size) || board->cells[row][col] != empty)
    {
        return 0;
    }

    /* Check all directions for a valid move */
    for (dx = -1; dx <= 1; dx++) 
    {
        for (dy = -1; dy <= 1; dy++) 
        {
            if (dx != 0 || dy != 0) 
            {
                if (checkDirection(board, row, col, dx, dy, player, 0))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void applyMove(Board *board, int row, int col, Cell player) 
{
    int dx;
    int dy;
    
    board->cells[row][col] = player;

    for (dx = -1; dx <= 1; dx++) 
    {
        for (dy = -1; dy <= 1; dy++) 
        {
            if (dx != 0 || dy != 0) 
            {
                checkDirection(board, row, col, dx, dy, player, 1);
            }
        }
    }
}

int hasAnyValidMove(Board *board, Cell player) 
{
    int i, j;
    
    for (i = 0; i < board->size; i++) 
    {
        for (j = 0; j < board->size; j++) 
        {
            if (isValidMove(board, i, j, player)) 
            {
                return 1;
            }
        }
    }

    return 0;
}

int countFlips(Board *board, int row, int col, Cell player) 
{
    int total = 0;
    int x;
    int y;
    int count;
    Cell opponent;
    int dx;
    int dy;

    for (dx = -1; dx <= 1; dx++) 
    {
        for (dy = -1; dy <= 1; dy++) 
        {
            if (dx != 0 || dy != 0) 
            {
                x = row + dx;
                y = col + dy;
                count = 0;

                if (player == user)
                {
                    opponent = computer;
                }

                else
                {
                    opponent = user;
                }

                /* Count how many opponent pieces can be flipped */
                while (isOnBoard(x, y, board->size) && board->cells[x][y] == opponent) 
                {
                    x += dx;
                    y += dy;
                    count++;
                }

                if (count > 0 && isOnBoard(x, y, board->size) && board->cells[x][y] == player) 
                {
                    total += count;
                }
            }
        }
    }

    return total;
}

Move getBestMove(Board *board, Cell player) 
{
    Move best;
    best.row = -1;
    best.col = -1;
    int maxFlips = -1;
    int flips;
    int i, j;

    /* Try all valid moves and choose the one that flips the most */
    for (i = 0; i < board->size; i++) 
    {
        for (j = 0; j < board->size; j++) 
        {
            if (isValidMove(board, i, j, player))
            {
                flips = countFlips(board, i, j, player);

                if (flips > maxFlips) 
                {
                    maxFlips = flips;
                    best.row = i;
                    best.col = j;
                }
            }
        }
    }

    return best;
}

void getUserMove(Board *board, int *row, int *col) 
{
    char input[10];
    char ch;
    int r, c;

    do 
    {
        printf("Enter move (e.g., 3c): ");
        scanf("%s", input);
        sscanf(input, "%d%c", &r, &ch);

        r--;
        c = ch - 'a';

    } while (!isValidMove(board, r, c, user));

    *row = r;
    *col = c;
}

void countScores(Board *board, int *userCount, int *computerCount) 
{
    int i, j;
    
    *userCount = 0;
    *computerCount = 0;

    /* Count number of user and computer pieces */
    for (i = 0; i < board->size; i++) 
    {
        for (j = 0; j < board->size; j++) 
        {
            if (board->cells[i][j] == user)
            {
                (*userCount)++;
            }
    
            else if (board->cells[i][j] == computer)
            {
                (*computerCount)++;
            }      
        }
    }
}

int isBoardFull(Board *board) 
{
    int i, j;
    
    for (i = 0; i < board->size; i++) 
    {
        for (j = 0; j < board->size; j++)
        {
            if (board->cells[i][j] == empty)
            {
                return 0;
            }
        }
    }

    return 1;
}

int main() 
{
    int size;
    Board board;
    int row, col;
    int userScore, compScore;
    Move best;

    do 
    {
        printf("Enter board size (even number between 4 and 20): ");
        scanf("%d", &size);

    } while (size < 4 || size > 20 || size % 2 != 0);

    board.size = size;
    board.cells = allocate(size);
    initializeBoard(&board);

    while (1) 
    {
        printBoard(&board);

        if (hasAnyValidMove(&board, user)) 
        {
            getUserMove(&board, &row, &col);
            applyMove(&board, row, col, user);
        } 
        
        else 
        {
            printf("User has no valid moves.\n");

            if (!hasAnyValidMove(&board, user)) 
            {
                break;
            }
        }

        if (hasAnyValidMove(&board, computer)) 
        {
            best = getBestMove(&board, computer);

            if (best.row != -1) 
            {
                printf("Computer plays %d%c\n", best.row + 1, best.col + 'a');
                applyMove(&board, best.row, best.col, computer);
            }
        } 
        
        else 
        {
            printf("Computer has no valid moves.\n");

            if (!hasAnyValidMove(&board, user)) 
            {
                break;
            }
        }

        if (isBoardFull(&board)) 
        {
            break;
        }
    }

    printBoard(&board);
    countScores(&board, &userScore, &compScore);

    printf("Final Score:\nUser (O): %d\nComputer (X): %d\n", userScore, compScore);

    if (userScore > compScore)
    {
        printf("You win!\n");
    }

    else if (userScore < compScore)
    {
        printf("Computer wins!\n");
    }
        
    else
    {
        printf("It's a tie!\n");
    }
        
    freeBoard(&board);

    return 0;
}
