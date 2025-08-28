#include <stdio.h>

int main()
{
    int a;
    int b;
    char operation;
    float expected_result;
    float experimental_result;
    float error;
    float error_percentage;
    float average_error_value = 0;
    int count = 0;

    /*first operation*/
    printf("Enter an operation (Example: 3+2): ");
    scanf("%d %c %d", &a, &operation, &b);

    if (operation == '+')
    {
        expected_result = a + b;
        experimental_result = a - b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '-')
    {
        expected_result = a - b;
        experimental_result = a * b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '*')
    {
        expected_result = a * b;
        experimental_result = a + b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '/')
    {

        experimental_result = a - b;

        if (b != 0)
        {
            expected_result = a / b;

            printf("\nExpected Result: %.2f\n", expected_result);
        }

        else
        {
            printf("\nError! Division by zero is not allowed. Expected Result cannot be calculated.\n");
            
            expected_result = 0;
        }
        
    }

    else
    {
        printf("Invalid! Please choose one of the following operations (+, -, *, /).\n");
        return 1;
    }

    printf("\nExperimental Result: %.2f\n", experimental_result);

    if (expected_result > experimental_result)
    {
        error = expected_result - experimental_result;
    }

    else if (expected_result < experimental_result)
    {
        error = experimental_result - expected_result;
    }

    printf("\nError: %.2f\n", error);

    if (expected_result == 0)
    {
        printf("\nError percentage cannot be calculated.\n");
    }

    else
    {
        error_percentage = (error / expected_result) * 100;
        printf("\nError Percentage: %.2f%%\n", error_percentage);

        count += 1;
        average_error_value += error_percentage;
    }

    /*second operation*/
    printf("\nEnter an operation (Example: 3+2): ");
    scanf("%d %c %d", &a, &operation, &b);

    if (operation == '+')
    {
        expected_result = a + b;
        experimental_result = a - b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '-')
    {
        expected_result = a - b;
        experimental_result = a * b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '*')
    {
        expected_result = a * b;
        experimental_result = a + b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '/')
    {

        experimental_result = a - b;

        if (b != 0)
        {
            expected_result = a / b;

            printf("\nExpected Result: %.2f\n", expected_result);
        }

        else
        {
            printf("\nError! Division by zero is not allowed. Expected Result cannot be calculated.\n");
            
            expected_result = 0;
        }
        
    }

    else
    {
        printf("Invalid! Please choose one of the following operations (+, -, *, /).\n");
        return 1;
    }

    printf("\nExperimental Result: %.2f\n", experimental_result);

    if (expected_result > experimental_result)
    {
        error = expected_result - experimental_result;
    }

    else if (expected_result < experimental_result)
    {
        error = experimental_result - expected_result;
    }

    printf("\nError: %.2f\n", error);

    if (expected_result == 0)
    {
        printf("\nError percentage cannot be calculated.\n");
    }

    else
    {
        error_percentage = (error / expected_result) * 100;
        printf("\nError Percentage: %.2f%%\n", error_percentage);

        count += 1;
        average_error_value += error_percentage;
    }

    /*third operation*/    
    printf("\nEnter an operation (Example: 3+2): ");
    scanf("%d %c %d", &a, &operation, &b);

    if (operation == '+')
    {
        expected_result = a + b;
        experimental_result = a - b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '-')
    {
        expected_result = a - b;
        experimental_result = a * b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '*')
    {
        expected_result = a * b;
        experimental_result = a + b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '/')
    {

        experimental_result = a - b;

        if (b != 0)
        {
            expected_result = a / b;

            printf("\nExpected Result: %.2f\n", expected_result);
        }

        else
        {
            printf("\nError! Division by zero is not allowed. Expected Result cannot be calculated.\n");
            
            expected_result = 0;
        }
        
    }

    else
    {
        printf("Invalid! Please choose one of the following operations (+, -, *, /).\n");
        return 1;
    }

    printf("\nExperimental Result: %.2f\n", experimental_result);

    if (expected_result > experimental_result)
    {
        error = expected_result - experimental_result;
    }

    else if (expected_result < experimental_result)
    {
        error = experimental_result - expected_result;
    }

    printf("\nError: %.2f\n", error);

    if (expected_result == 0)
    {
        printf("\nError percentage cannot be calculated.\n");
    }

    else
    {
        error_percentage = (error / expected_result) * 100;
        printf("\nError Percentage: %.2f%%\n", error_percentage);

        count += 1;
        average_error_value += error_percentage;
    }

    /*fourth operation*/
    printf("\nEnter an operation (Example: 3+2): ");
    scanf("%d %c %d", &a, &operation, &b);

    if (operation == '+')
    {
        expected_result = a + b;
        experimental_result = a - b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '-')
    {
        expected_result = a - b;
        experimental_result = a * b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '*')
    {
        expected_result = a * b;
        experimental_result = a + b;

        printf("\nExpected Result: %.2f\n", expected_result);
    }

    else if (operation == '/')
    {

        experimental_result = a - b;

        if (b != 0)
        {
            expected_result = a / b;

            printf("\nExpected Result: %.2f\n", expected_result);
        }

        else
        {
            printf("\nError! Division by zero is not allowed. Expected Result cannot be calculated.\n");
            
            expected_result = 0;
        }
        
    }

    else
    {
        printf("Invalid! Please choose one of the following operations (+, -, *, /).\n");
        return 1;
    }

    printf("\nExperimental Result: %.2f\n", experimental_result);

    if (expected_result > experimental_result)
    {
        error = expected_result - experimental_result;
    }

    else if (expected_result < experimental_result)
    {
        error = experimental_result - expected_result;
    }

    printf("\nError: %.2f\n", error);

    if (expected_result == 0)
    {
        printf("\nError percentage cannot be calculated.\n");
    }

    else
    {
        error_percentage = (error / expected_result) * 100;
        printf("\nError Percentage: %.2f%%\n", error_percentage);

        count += 1;
        average_error_value += error_percentage;
    }
    
    /*average calculation*/
    average_error_value /= count;

    printf("\nAverage Error Value: %.2f%%\n", average_error_value);

    return 0;
}
