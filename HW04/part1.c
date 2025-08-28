#include <stdio.h>
#include <math.h>

/* Displays the main menu options */
void menu()
{    
    printf("Welcome to the Rocket Launch Simulator!\n");
    printf("---------------------------------------\n");
    printf("1. Enter launch parameters\n");
    printf("2. Simulate rocket trajectory\n");
    printf("3. Save trajectory data\n");
    printf("4. Exit\n");
    printf("Choice: ");
}

/* Collects launch parameters from the user and saves them to a file */
void launch_parameters()
{
    double velocity;
    double gravity; 
    double height;

    printf("\nEnter initial velocity (m/s): ");
    while (scanf("%lf", &velocity) != 1) 
    {
        while (getchar() != '\n');
        printf("Invalid input! Please enter a valid number for velocity: ");
    }

    printf("Enter gravity (m/s^2, default 9.8): ");
    while (scanf("%lf", &gravity) != 1 || gravity <= 0) 
    {
        while (getchar() != '\n');
        printf("Invalid input! Please enter a valid number for gravity (positive value): ");
    }

    printf("Enter launch height (m): ");
    while (scanf("%lf", &height) != 1 || height < 0) 
    {
        while (getchar() != '\n'); 
        printf("Invalid input! Please enter a valid number for height (positive value): ");
    }

    FILE *file = fopen("rocket_data.txt", "w");
    fprintf(file, "%f %f %f", velocity, gravity, height);
    fclose(file);

    printf("Rocket parameters saved to rocket_data.txt!\n\n");
}

/* Calculates height based on the physics equation */
double calculate_height(double velocity, double gravity, double height, double time)
{
    return ((-0.5 * gravity * time * time) + (velocity * time) + height);
}

/* Calculates the maximum height reached by the rocket */
double calculate_max_height(double velocity, double gravity, double height)
{
    return (((velocity * velocity) / (2 * gravity)) + height);
}

/* Calculates the total flight duration until the rocket hits the ground */
double calculate_total_flight(double velocity, double gravity, double height)
{
    return ((velocity + sqrt((velocity * velocity) + (2 * gravity * height))) / gravity);
}

/* Simulates and displays the rocket's trajectory graph */
void simulate_trajectory()
{
    double velocity;
    double gravity; 
    double height;
    double time;
    double h;
    double landing_time;
    double max_height;
    int i, j, k, a;

    FILE *file = fopen("rocket_data.txt", "r");
    printf("Reading rocket parameters from file...\n");
    fscanf(file, "%lf %lf %lf", &velocity, &gravity, &height);
    fclose(file);

    printf("Equation: h(t) = %.1f * t^2 + %.0f * t + %.0f\n", -0.5 * gravity, velocity, height);
    printf("Simulating trajectory...\n");

    landing_time = calculate_total_flight(velocity, gravity, height);
    max_height = calculate_max_height(velocity, gravity, height);

    for (i = (int)ceil(max_height / 10) * 10; i > 0; i -= 10)
    {
        printf("%3d |", i);
        for (j = 0; j <= landing_time; j ++)
        {
            time = (j * landing_time) / (landing_time);
            h = calculate_height(velocity, gravity, height, time);
            if (h >= i && h < i + 10) 
            {
                printf("# ");
            }
            
            else
            {
                printf("  ");
            }
        }
        printf("\n\n");
    }

    printf("  0 |");
    for (k = 0; k < (int)ceil(landing_time / 2) * 2; k += 2)
    {
        printf("---|");
    }
    printf("\n");

    printf("    ");
    for (a = 0; a <= (int)ceil(landing_time / 2) * 2; a += 2)
    {
        printf("%-4d", a);
    }

    printf("\n\n");
}

/* Saves the rocket's trajectory graph to a file */
void save_trajectory()
{
    double velocity;
    double gravity;
    double height;
    double max_altitude;
    double total_flight;
    double time;
    double h;
    int i, j, k, a;

    FILE *file = fopen("rocket_data.txt", "r");
    fscanf(file, "%lf %lf %lf", &velocity, &gravity, &height);
    fclose(file);

    FILE *f = fopen("trajectory.txt", "w");
    max_altitude = calculate_max_height(velocity, gravity, height);
    total_flight = calculate_total_flight(velocity, gravity, height);

    printf("Saving trajectory data...\n");
    printf("Maximum altitude: %.2f meters\n", max_altitude);
    printf("Total flight duration: %.2f seconds\n", total_flight);

    for (i = (int)ceil(max_altitude / 10) * 10; i > 0; i -= 10)
    {
        fprintf(f, "%3d |", i);
        for (j = 0; j <= total_flight; j ++)
        {
            time = (j * total_flight) / (total_flight);
            h = calculate_height(velocity, gravity, height, time);
            if (h >= i && h < i + 10) 
            {
                fprintf(f, "# ");
            }
            
            else
            {
                fprintf(f, "  ");
            }
        }
        fprintf(f, "\n\n");
    }

    fprintf(f, "  0 |");
    for (k = 0; k < (int)ceil(total_flight / 2) * 2; k += 2)
    {
        fprintf(f, "---|");
    }
    fprintf(f, "\n");

    fprintf(f, "    ");
    for (a = 0; a <= (int)ceil(total_flight / 2) * 2; a += 2)
    {
        fprintf(f, "%-4d", a);
    }

    fprintf(f, "\n");
    fprintf(f, "Maximum altitude: %.2f meters\n", max_altitude);
    fprintf(f, "Total flight duration: %.2f seconds\n", total_flight);
    fclose(f);

    printf("Graph saved to trajectory.txt!\n\n");
}

int main()
{
    int choice;

    while (1)
    {
        menu();
        
        while (scanf("%d", &choice) != 1) 
        {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a integer number for choice: ");
        }
        
        switch(choice)
        {
            case 1:
                launch_parameters();
                break;
                
            case 2:
                simulate_trajectory();
                break;
                
            case 3:
                save_trajectory();
                break;
                
            case 4:
                printf("Goodbye!\n\n");
                return 1;
                
            default:
                printf("Invalid option! Please enter a valid option (1, 2, 3 or 4).\n\n");
                break;
        }
    }

    return 0;
}

