#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Function to print the game board */
void print_board(int board[10][10]) 
{
    int i, j;

    printf("  ");
    for (i = 0; i < 10; i++) 
    {
        printf("%d ", i);
    }

    printf("\n");
    for (i = 0; i < 10; i++) 
    {
        printf("%d ", i);

        for (j = 0; j < 10; j++) 
        {
            if (board[i][j] == -1) 
            {
                printf("O "); /* Miss */
            } 

            else if (board[i][j] == -2) 
            {
                printf("X "); /* Hit */
            }

            else 
            {
                printf("- "); /* Unknown/Empty */
            }
        }

        printf("\n");
    }
}

/* Function to randomly place ships on the board */
int random_place(int board[10][10], int size, FILE *ship_file, int ship_id) 
{
    int x, y, horizontal, i;
    int can_place = 1;

    /* Randomly decide orientation (horizontal or vertical) */
    horizontal = rand() % 2;

    /* Generate random coordinates based on orientation and ship size */
    if (horizontal) 
    {
        x = rand() % (10 - size + 1);
        y = rand() % 10;
    } 

    else 
    {
        x = rand() % 10;
        y = rand() % (10 - size + 1);
    }

    /* Check if we can place the ship at the generated coordinates */
    for (i = 0; i < size; i++) 
    {
        if (horizontal) 
        {
            if (board[y][x + i] != 0) 
            {
                can_place = 0;
                break;
            }
        }

        else 
        {
            if (board[y + i][x] != 0) 
            {
                can_place = 0;
                break;
            }
        }
    }

    /* If we can place the ship, update the board and log the ship's cells */
    if (can_place) 
    {
        for (i = 0; i < size; i++) 
        {
            if (horizontal) 
            {
                /* Store ship_id * 10 + size to distinguish between ships of same size (especially for size 3 ships) */
                board[y][x + i] = ship_id * 10 + size;

                fprintf(ship_file, "%d %d %d\n", y, x + i, size);
            }

            else 
            {
                board[y + i][x] = ship_id * 10 + size;

                fprintf(ship_file, "%d %d %d\n", y + i, x, size);
            }
        }
    }

    return can_place;
}

/* Updated function to get user input for coordinates - ANSI C compliant */
int get_user_input(int *x, int *y) 
{
    int valid_input;
    int scan_result;
    char ch;
    
    valid_input = 0;
    
    while (!valid_input) 
    {
        printf("Enter coordinates (X to quit): ");
        
        scan_result = scanf("%d %d", y, x);
        
        if (scan_result == 2) 
        {
            /* Check if coordinates are in valid range */
            if (*x >= 0 && *x <= 9 && *y >= 0 && *y <= 9) 
            {
                valid_input = 1; /* Valid input received */
                
                /* Clear input buffer */
                ch = getchar();
                while (ch != '\n' && ch != EOF) 
                {
                    ch = getchar();
                }
            } 

            else 
            {
                printf("Invalid coordinates! Please enter values between 0 and 9.\n\n");
                
                /* Clear input buffer */
                ch = getchar();
                while (ch != '\n' && ch != EOF) 
                {
                    ch = getchar();
                }
            }
        } 

        else 
        {
            /* Check if user wants to quit */
            ch = getchar();
            if (ch == 'X' || ch == 'x') 
            {
                /* Clear input buffer */
                while (ch != '\n' && ch != EOF) 
                {
                    ch = getchar();
                }
                return 0;
            } 
            
            else 
            {
                printf("Invalid input. Please enter two numbers separated by space (0-9).\n\n");
                
                /* Clear input buffer */
                while (ch != '\n' && ch != EOF) 
                {
                    ch = getchar();
                }
            }
        }
    }
    
    return 1; /* Valid input processed */
}

/* Function to check if a specific ship is sunk */
int check_ship_sunk(int board[10][10], int ship_id, int size) 
{
    int i, j;
    int id_to_check = ship_id * 10 + size;
    
    for (i = 0; i < 10; i++) 
    {
        for (j = 0; j < 10; j++) 
        {
            if (board[i][j] == id_to_check) 
            {
                return 0; /* Ship is not fully sunk */
            }
        }
    }

    return 1; /* Ship is sunk */
}

/* Function to extract ship size from board value */
int get_ship_size(int value) 
{
    return value % 10;
}

/* Function to extract ship ID from board value */
int get_ship_id(int value) 
{
    return value / 10;
}

int main() 
{
    int board[10][10] = {0};
    int player_board[10][10] = {0}; /* Board shown to the player */
    int x, y, shots_taken = 0;
    int ship_2_sunk = 0;
    int ship_3_1_sunk = 0, ship_3_2_sunk = 0;
    int ship_4_sunk = 0;
    int hit_ship_size, hit_ship_id;
    int original_value;
    char choice;
    
    /* Open files for logging */
    FILE *ship_file = fopen("ships.txt", "w");
    FILE *log_file = fopen("battleship_log.txt", "w");
    
    if (!ship_file || !log_file) 
    {
        printf("Error opening files!\n");
        return 1;
    }
    
    srand((time(NULL)));
    
    /* Place ships randomly on the board */
    while (!random_place(board, 4, ship_file, 1)); /* 1 ship of size 4 (ID: 1) */
    while (!random_place(board, 3, ship_file, 2)); /* 1st ship of size 3 (ID: 2) */
    while (!random_place(board, 3, ship_file, 3)); /* 2nd ship of size 3 (ID: 3) */
    while (!random_place(board, 2, ship_file, 4)); /* 1 ship of size 2 (ID: 4) */
    
    fclose(ship_file);
    
    printf("Welcome to Battleship Game!\n\n");
    
    print_board(player_board);
    
    /* Main game loop */
    while (1) 
    {
        /* Get user input */
        if (!get_user_input(&x, &y)) 
        {
            printf("Exiting the game.\n");
            break;
        }
        
        /* Skip if already targeted */
        if (player_board[y][x] == -1 || player_board[y][x] == -2) 
        {
            printf("Already targeted! Please choose another point.\n\n");
            continue;
        }
        
        shots_taken++;
        
        /* Check if it's a hit or miss */
        if (board[y][x] > 0) 
        {
            hit_ship_size = get_ship_size(board[y][x]);
            hit_ship_id = get_ship_id(board[y][x]);
            
            printf("HIT!\n");
            fprintf(log_file, "Shot: %d %d - HIT\n", x, y);
            
            /* Store the original value before marking as hit */
            original_value = board[y][x];
            
            /* Mark as hit on both boards */
            player_board[y][x] = -2;
            board[y][x] = -2;
            
            /* Check if the specific ship is fully sunk */
            if (check_ship_sunk(board, hit_ship_id, hit_ship_size)) 
            {
                printf("Congratulations! You have sunk a %d-cell ship!\n", hit_ship_size);
                
                /* Update ship status based on ID */
                if (hit_ship_id == 1) 
                    ship_4_sunk = 1;

                else if (hit_ship_id == 2) 
                    ship_3_1_sunk = 1;

                else if (hit_ship_id == 3) 
                    ship_3_2_sunk = 1;

                else if (hit_ship_id == 4) 
                    ship_2_sunk = 1; 
            }
        } 

        else 
        {
            printf("MISS!\n");
            fprintf(log_file, "Shot: %d %d - MISS\n", x, y);
            player_board[y][x] = -1;
        }
        
        printf("\n");
        print_board(player_board);

        /* Check if all ships are sunk */
        if (ship_2_sunk && ship_3_1_sunk && ship_3_2_sunk && ship_4_sunk) 
        {
            printf("\nAll ships are sunk! Total shots: %d\n", shots_taken);
            
            /* Ask if player wants to play again */
            printf("\nPress 'N' to play again or 'X' to exit: ");
            scanf(" %c", &choice);
            
            if (choice == 'X' || choice == 'x') 
            {
                return 1;
            } 

            else if (choice == 'N' || choice == 'n') 
            {   
                /* Reset board manually */
                int i, j;

                for (i = 0; i < 10; i++) 
                {
                    for (j = 0; j < 10; j++) 
                    {
                        board[i][j] = 0;
                        player_board[i][j] = 0;
                    }
                }

                /* Reset game variables */
                ship_2_sunk = 0;
                ship_3_1_sunk = 0;
                ship_3_2_sunk = 0;
                ship_4_sunk = 0;
                shots_taken = 0;

                main(); /* Start a new game */
                
                return 0;
            }

            else 
            {
                printf("Invalid option! Exiting game.\n");
                return 1;
            }
        }
    }
    
    fclose(log_file);
    return 0;
}
