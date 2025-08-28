#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_MOVES 100

int move_rows[MAX_MOVES];
int move_cols[MAX_MOVES];
int move_top = 0; /* Stack top index */

/* Function to generate game board and place mines randomly */
void generate_board(int n)
{
    int i, j;
    int mines[10][10] = {0};
    int a, b;
    int placed = 0;

    FILE *file = fopen("map.txt", "w");
    
    if (file == NULL)
    {
        printf("Error! The file cannot be opened.\n");
        return;
    }

    while (placed < n)
    {
        a = rand() % (n + 1);
        b = rand() % (n + 1);

        if (mines[a][b] == 0)
        {
            mines[a][b] = 1;
            placed++;
        }
    }

    printf("  ");
    for (i = 0; i < n; i++)
    {
        printf("%d ", i);
    }
    
    printf("\n");
    for (i = 0; i < n; i++)
    {
        printf("%d ", i);
        
        for (j = 0; j < n; j++)
        {
            printf("# ");
        }
        
        printf("\n");
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (mines[i][j] == 1)
            {
                fprintf(file, "* ");
            }
            
            else
            {
                fprintf(file, ". ");
            }
        }
        
        fprintf(file, "\n");
    }

    fclose(file);
}

/* Function to check a cell and open it; recursively reveals adjacent cells if no nearby mines */
int check_and_open(int row, int column, int n, char map_board[10][10], char visible_board[10][10])
{
    int mine_count;
    int i, j;

    if (visible_board[row][column] != '#')
    {
        return 0;
    }

    mine_count = 0;
    for (i = row - 1; i <= row + 1; i++)
    {
        for (j = column - 1; j <= column + 1; j++)
        {
            if (i >= 0 && i < n && j >= 0 && j < n && map_board[i][j] == '*')
            {
                mine_count++;
            }
        }
    }

    if (mine_count == 0)
    {
        visible_board[row][column] = '0';

        for (i = row - 1; i <= row + 1; i++)
        {
            for (j = column - 1; j <= column + 1; j++)
            {
                if (i >= 0 && i < n && j >= 0 && j < n && visible_board[i][j] == '#')
                {
                    check_and_open(i, j, n, map_board, visible_board);
                }
            }
        }
    }
    
    else
    {
        visible_board[row][column] = mine_count + '0';
    }

    return 1;
}

/* Pushes move coordinates onto the stack */
void push(int row, int col)
{
    if (move_top < MAX_MOVES)
    {
        move_rows[move_top] = row;
        move_cols[move_top] = col;
        move_top++;
    }
}

/* Pops last move from the stack and undoes it */
void pop(char visible_board[10][10])
{
    int last_row;
    int last_col;

    if (move_top > 0)
    {
        move_top--;

        last_row = move_rows[move_top];
        last_col = move_cols[move_top];

        visible_board[last_row][last_col] = '#';

        printf("Last move undone.\n");
    }
    
    else
    {
        printf("No moves to undo.\n");
    }
}

/* Checks if the stack is empty */
int isEmpty()
{
    return move_top == 0;
}

/* Checks whether the move was already made */
int already_moved(int row, int col)
{
    int i;

    for (i = 0; i < move_top; i++)
    {
        if (move_rows[i] == row && move_cols[i] == col)
        {
            return 1;
        }
    }
    
    return 0;
}

/* Checks whether all non-mine cells are opened */
int check_game_over(int n, char visible_board[10][10], char map_board[10][10])
{
    int i, j;
    
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (map_board[i][j] != '*' && visible_board[i][j] == '#')
            {
                return 0;
            }
        }
    }
    
    return 1;
}

/* Handles player's game loop */
void player_move(int n)
{
    int row, column;
    int i, j;
    char map_board[10][10];
    char visible_board[10][10];
    int status = 1;

    FILE *map_file = fopen("map.txt", "r");
    
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            fscanf(map_file, " %c", &map_board[i][j]);
            visible_board[i][j] = '#';
        }
    }
    
    fclose(map_file);

    while (status)
    {
        printf("Enter move (row col) or 'undo': ");
        
        if (scanf("%d %d", &row, &column) != 2)
        {
            char input[10];
            scanf("%s", input);

            if (input[0] == 'u' && input[1] == 'n' && input[2] == 'd' && input[3] == 'o' && input[4] == '\0')
            {
                if (!isEmpty())
                {
                    pop(visible_board);
                    
                    printf("   ");
                    for (i = 0; i < n; i++)
                    {
                        printf("%d ", i);
                    }
                    
                    printf("\n");
                    for (i = 0; i < n; i++)
                    {
                        printf("%d ", i);
                        
                        for (j = 0; j < n; j++)
                        {
                            printf(" %c", visible_board[i][j]);
                        }
                        
                        printf("\n");
                    }
                    
                    continue;
                }
                
                else
                {
                    printf("You cannot undo because the stack is empty.\n");
                    continue;
                }
            }
            
            else
            {
                printf("Invalid value! Try again.\n");
                continue;
            }
        }

        if (row < 0 || row >= n || column < 0 || column >= n)
        {
            printf("Invalid move! Try again.\n");
            continue;
        }

        if (already_moved(row, column))
        {
            printf("You already tried this cell. Try a different move.\n");
            continue;
        }

        push(row, column);

        if (map_board[row][column] == '*')
        {
            visible_board[row][column] = 'X';
            
            printf("   ");
            for (i = 0; i < n; i++)
            {
                printf("%d ", i);
            }
            
            printf("\n");
            for (i = 0; i < n; i++)
            {
                printf("%d ", i);
                
                for (j = 0; j < n; j++)
                {
                    printf("%c ", visible_board[i][j]);
                }
                
                printf("\n");
            }
            
            printf("BOOM! You hit a mine. Game Over.\n");
            status = 0;
        }
        
        else
        {
            check_and_open(row, column, n, map_board, visible_board);
            
            printf("   ");
            for (i = 0; i < n; i++)
            {
                printf("%d ", i);
            }
            
            printf("\n");
            for (i = 0; i < n; i++)
            {
                printf("%d ", i);
                
                for (j = 0; j < n; j++)
                {
                    printf(" %c", visible_board[i][j]);
                }
                
                printf("\n");
            }
        }

        if (check_game_over(n, visible_board, map_board))
        {
            printf("Congratulations!! YOU WON!\n");
            status = 0;
        }
    }
}

/* Outputs all player moves to a file */
void output_file()
{
    int i;
    
    FILE *output_file = fopen("moves.txt", "w");
    
    if (output_file == NULL)
    {
        return;
    }

    fprintf(output_file, "--- Game Moves ---\n");
    for (i = 0; i < move_top; i++)
    {
        fprintf(output_file, "Move %d: (Row %d, Col %d)\n", i + 1, move_rows[i], move_cols[i]);
    }
    
    fprintf(output_file, "\nTotal Moves: %d\n", move_top);
    fclose(output_file);
}

/* Main function: initializes game */
int main(void)
{
    int size;
    srand((unsigned int)time(NULL));
    size = rand() % (10 - 2 + 1) + 2; /* Random size between 2 and 10 */

    generate_board(size);
    player_move(size);
    output_file();

    return 0;
}

