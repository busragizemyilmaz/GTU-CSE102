#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int width;
    int height;
    int playerX;
    int playerY;
    int doorX;
    int doorY;
    char move;

    FILE *game_file = fopen("game_state.txt", "r");

    /* If the game_state file doesn't exist, create a new game board */
    if (game_file == NULL)
    {
        srand(time(NULL)); /* Initialize random number generator */

        printf("Generating a new board...\n");
        printf("Enter the width and height of board: \n");
        scanf("%d %d", &width, &height);

        /* Randomly place player and door on the board */
        playerX = rand() % width;
        playerY = rand() % height;
        doorX = rand() % width;
        doorY = rand() % height;

        /* Ensure that the player and door are not at the same position */
        if (playerX == doorX && playerY == doorY)
        {
            doorX = (doorX + 1) % width;
        }

        /* Open the file to save the new game state */
        game_file = fopen("game_state.txt", "w");
        fprintf(game_file, "%d %d %d %d %d %d", width, height, playerX, playerY, doorX, doorY);
        fclose(game_file);

        printf("Width: %d, Height: %d Player: (%d,%d), Door: (%d,%d)\n", width, height, playerX, playerY, doorX, doorY);
        return 1;
      
    }

    else
    {
        /* If the game state file exists, read the current game state */
        fscanf(game_file, "%d %d %d %d %d %d", &width, &height, &playerX, &playerY, &doorX, &doorY);
        fclose(game_file);
    }

    printf("Enter move (WASD): ");
    scanf(" %c", &move);

    /* Check if the input is a valid move */
    if (move != 'W' && move != 'A' && move != 'S' && move != 'D')
    {
        printf("Invalid option! Please choose one of them W, A, S or D.\n");
        return 1;
    }

    /* Move the player only if within bounds */
    if (move == 'W' && playerY > 0)
    {
        playerY -= 1;
    }
 
    else if (move == 'A' && playerX > 0)
    {
        playerX -= 1;
    }
       
    else if (move == 'S' && playerY < height - 1)
    {
        playerY += 1;
    }

    else if (move == 'D' && playerX < width - 1)
    {
        playerX += 1;
    }
       
    else
    {
        printf("You hit a wall! Try a different move.\n");
        return 1;
    }

    printf("Player moves to (%d,%d)\n", playerX, playerY);

    /* Open the file again to update the game state */
    game_file = fopen("game_state.txt", "w");
    fprintf(game_file, "%d %d %d %d %d %d", width, height, playerX, playerY, doorX, doorY);
    fclose(game_file);

    /* Check if the player has reached the door */
    if (playerX == doorX && playerY == doorY)
    {
        printf("Congratulations! You escaped!\n");
        printf("Generating a new board...\n");
        
        remove("game_state.txt");
        return 1;    
    }

    else
    {
        printf("Game continues...\n"); /* Continue the game if the player hasn't reached the door */
    }

    return 0;
}
