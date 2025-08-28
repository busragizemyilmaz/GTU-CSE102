#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function for setting game configurations
void admin_mode() 
{
    int code_length;
    int min_digit; 
    int max_digit; 
    int max_attempts;
    int allow_duplicates;
    int points_correct_position;
    int points_correct_digit;
    int points_wrong_digit;

    FILE *file = fopen("vault_config.txt", "w");
    
    printf("\n===== ADMIN MODE =====\n");

    // Get and validate code length input
    printf("Enter code length: ");
    while (scanf("%d", &code_length) != 1) 
    {
        printf("Invalid input. Please enter an integer for code length: ");
        while (getchar() != '\n');
    }
    fprintf(file, "CODE_LENGTH=%d\n", code_length);
    
    // Get and validate min digit
    printf("Enter minimum digit: ");
    while (scanf("%d", &min_digit) != 1)
    {
      printf("Invalid input. Please enter an integer for minimum digit: ");
      while (getchar() != '\n');
    }
    fprintf(file, "DIGIT_MIN=%d\n", min_digit);
    
    // Get and validate max digit
    printf("Enter maximum digit: ");
    while (scanf("%d", &max_digit) != 1)
    {
      printf("Invalid input. Please enter an integer for maximum digit: ");
      while (getchar() != '\n');
    }
    fprintf(file, "DIGIT_MAX=%d\n", max_digit);

    // Get and validate max attempts
    printf("Enter maximum number of attempts: ");
    while (scanf("%d", &max_attempts) != 1)
    {
      printf("Invalid input. Please enter an integer for maximum attempts: ");
      while (getchar() != '\n');
    }
    fprintf(file, "MAX_ATTEMPTS=%d\n", max_attempts);

    // Get and validate duplicate setting
    printf("Allow duplicates? (0 = No, 1 = Yes): ");
    while (scanf("%d", &allow_duplicates) != 1 || (allow_duplicates != 0 && allow_duplicates != 1))
    {
        printf("Invalid input. Please enter 0 (No) or 1 (Yes) for allow duplicates: ");
        while (getchar() != '\n');
    }
    fprintf(file, "ALLOW_DUPLICATES=%d\n", allow_duplicates);
    
    // Get and validate points
    printf("Enter points for correct digit in correct place: ");
    while (scanf("%d", &points_correct_position) != 1) 
    {
        printf("Invalid input. Please enter an integer for points correct place: ");
        while (getchar() != '\n');
    }
    fprintf(file, "POINTS_CORRECT=%d\n", points_correct_position);

    printf("Enter points for correct digit in wrong place: ");
    while (scanf("%d", &points_correct_digit) != 1) 
    {
        printf("Invalid input. Please enter an integer for points wrong place: ");
        while (getchar() != '\n');
    }
    fprintf(file, "POINTS_MISPLACED=%d\n", points_correct_digit);

    printf("Enter penalty for wrong digit: ");
    while (scanf("%d", &points_wrong_digit) != 1) 
    {
        printf("Invalid input. Please enter an integer for wrong digit: ");
        while (getchar() != '\n');
    }
    fprintf(file, "PENALTY_WRONG=%d\n", points_wrong_digit);

    printf("\nAdmin configuration saved to vault_config.txt\n");
    fclose(file);
}

// Function to generate the secret code
int *generate_code()
{
    int code_length;
    int min_digit; 
    int max_digit; 
    int max_attempts;
    int allow_duplicates;
    int points_correct_position;
    int points_correct_digit;
    int points_wrong_digit;
    int *code;
    int i, j;
    int random_digit;
    int repetition;

    FILE *file = fopen("vault_config.txt", "r");

    if (file == NULL)
    {
        printf("Error opening configuration file!\n");
        return NULL;
    }

    // Read configuration values from file
    fscanf(file, "CODE_LENGTH=%d\n", &code_length);
    fscanf(file, "DIGIT_MIN=%d\n", &min_digit);
    fscanf(file, "DIGIT_MAX=%d\n", &max_digit);
    fscanf(file, "MAX_ATTEMPTS=%d\n", &max_attempts);
    fscanf(file, "ALLOW_DUPLICATES=%d\n", &allow_duplicates);
    fscanf(file, "POINTS_CORRECT=%d\n", &points_correct_position);
    fscanf(file, "POINTS_MISPLACED=%d\n", &points_correct_digit);
    fscanf(file, "PENALTY_WRONG=%d\n", &points_wrong_digit);
    fclose(file);  

    code = malloc(code_length * sizeof(int)); // Allocate memory for code
    if (code == NULL)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Generate each digit of the secret code
    for (i = 0; i < code_length; i++)
    {
        do
        {
            random_digit = (rand() % (max_digit - min_digit + 1)) + min_digit;
            repetition = 0;
            
            for (j = 0; j < i; j++)
            {
                if (code[j] == random_digit)
                {
                    repetition = 1;
                    break;
                }
            }
        }         
        while (allow_duplicates == 0 && repetition == 1); // Repeat if no duplicates allowed and digit is repeated
    
        code[i] = random_digit;
    }   

    return code;
}

// Function to get the player's guess as an array
int *get_guess()
{
    int code_length;
    int min_digit;
    int max_digit;
    int guess;
    int *guess_array;
    int i;

    FILE *file = fopen("vault_config.txt", "r");

    if (file == NULL)
    {
        printf("Error opening configuration file!\n");
        return NULL;
    }

    // Read config values needed for guess validation
    fscanf(file, "CODE_LENGTH=%d\n", &code_length);
    fscanf(file, "DIGIT_MIN=%d\n", &min_digit);
    fscanf(file, "DIGIT_MAX=%d\n", &max_digit);
    fclose(file);    

    // Prompt player for their guess
    printf("Enter your guess (digits between %d and %d): ", min_digit, max_digit);
    scanf("%d", &guess);

    guess_array = malloc(code_length * sizeof(int)); // Allocate memory
    if (guess_array == NULL)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    // Convert input number to array of digits
    for (i = code_length - 1; i >= 0; i--)
    {
        guess_array[i] = guess % 10;
        guess = guess / 10;
    }

    return guess_array;
}

// Function to determine score rank title
char *score_range(int score) 
{
    if (score < 10)
        return "Code Potato 🥔";
    else if (score <= 29)
        return "Lucky Breaker 🍀";
    else if (score <= 49)
        return "Safe Kicker 💨";
    else if (score <= 69)
        return "Number Sleuth 🕵️‍♂️";
    else if (score <= 89)
        return "Cipher Hunter 🔍";
    else
        return "Code Master 🧠";
}

// Main player game loop
void player_mode()
{
    int code_length;
    int min_digit; 
    int max_digit; 
    int max_attempts;
    int allow_duplicates;
    int points_correct_position;
    int points_correct_digit;
    int points_wrong_digit;
    int *secret_code;
    int i, j;
    int *guess;
    int count = 0;
    int correct_position_count = 0;
    int score = 0;
    int found;
    time_t now = time(NULL);
    char time_str[30];

    FILE *config_file = fopen("vault_config.txt", "r");
    FILE *code_file = fopen("vault_code.txt", "w");
    FILE *log_file = fopen("game_log.txt", "w");

    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now)); // Get current time

    if (config_file == NULL)
    {
        printf("Error opening configuration file!\n");
        return;
    }

    // Load game settings
    fscanf(config_file, "CODE_LENGTH=%d\n", &code_length);
    fscanf(config_file, "DIGIT_MIN=%d\n", &min_digit);
    fscanf(config_file, "DIGIT_MAX=%d\n", &max_digit);
    fscanf(config_file, "MAX_ATTEMPTS=%d\n", &max_attempts);
    fscanf(config_file, "ALLOW_DUPLICATES=%d\n", &allow_duplicates);
    fscanf(config_file, "POINTS_CORRECT=%d\n", &points_correct_position);
    fscanf(config_file, "POINTS_MISPLACED=%d\n", &points_correct_digit);
    fscanf(config_file, "PENALTY_WRONG=%d\n", &points_wrong_digit);
    fclose(config_file);    

    printf("\n===== PLAYER MODE =====\n");
    fprintf(log_file, "--- Vault Codebreaker Game Log ---\n");
    fprintf(log_file, "Game Date: %s\n", time_str);

    secret_code = generate_code(); // Create secret code

    // Log and save secret code
    fprintf(log_file, "Secret Code: ");
    for (i = 0; i < code_length; i++)
    {   
        fprintf(code_file, "%d", secret_code[i]);
        fprintf(log_file, "%d", secret_code[i]);
    }
    fprintf(log_file, "\n");
    fclose(code_file);

    // Log game parameters
    fprintf(log_file, "Code Lenght: %d\n", code_length);
    fprintf(log_file, "Digit Range: %d-%d\n", min_digit, max_digit);
    fprintf(log_file, "Duplicates Allowed: %d\n", allow_duplicates);
    fprintf(log_file, "Max Attempts: %d\n\n", max_attempts);

    // Main guessing loop
    while (count < max_attempts && correct_position_count != code_length)
    {
        guess = get_guess(); // Get player guess
        correct_position_count = 0;

        fprintf(log_file, "Attempt %d: ", count + 1);
        for (i = 0; i < code_length; i++)
            fprintf(log_file, "%d", guess[i]);
    
        fprintf(log_file, " => Feedback: ");
    
        // Scoring and feedback logic
        for (i = 0; i < code_length; i++)
        {
            if (guess[i] == secret_code[i])
            {
                fprintf(log_file, "C ");
                printf("C ");
                score += points_correct_position;
                correct_position_count++;
            }
            else
            {
                found = 0;
                for (j = 0; j < code_length; j++)
                {
                    if (guess[i] == secret_code[j] && i != j)
                    {
                        found = 1;
                        break;
                    }
                }

                if (found)
                {
                    fprintf(log_file, "M ");
                    printf("M ");
                    score += points_correct_digit;
                }

                else
                {
                    fprintf(log_file, "W ");
                    printf("W ");
                    score -= points_wrong_digit;
                }
            }
        }

        printf("\n");
        fprintf(log_file, "| Score: %d\n", score);

        count++;
        free(guess);
    }

    // Final score and rank
    fprintf(log_file, "\nFinal Score: %d\n", score);
    printf("\nFinal Score: %d\n", score);
    fprintf(log_file, "Rank: %s\n", score_range(score));
    printf("Rank: %s\n", score_range(score));
    fprintf(log_file, "-----------------------------------\n");
    printf("-----------------------------------\n");

    free(secret_code);
    fclose(log_file);
}

// Main function
int main() 
{
    char choice;

    srand(time(NULL)); // Seed random generator
    
    printf("========== CODEBREAKER GAME ==========\n");
    printf("Enter 'A' for Admin Mode or 'P' for Player Mode: ");
    scanf(" %c", &choice);

    switch(choice)
    {
        case 'A':
            admin_mode();  // Admin sets game settings
            break;

        case 'P':
            player_mode(); // Player starts guessing
            break;
            
        default:
            printf("Invalid choice. Exiting program.\n");
            return 1;
    }
    
    return 0;
}
