#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int max_operation(int *chunk, int *corresponding_chunk_part, int chunk_size)
{
    int i;
    int max;
    int product;

    max = chunk[0] * corresponding_chunk_part[0];

    for (i = 0; i < chunk_size; i++)
    {
        product = chunk[i] * corresponding_chunk_part[i];

        if (product > max)
        {
            max = product;
        }
    }

    return max;
}

int avg_operation(int *chunk, int *corresponding_chunk_part, int chunk_size)
{
    int i;
    int sum = 0;

    for (i = 0; i < chunk_size; i++)
    {
        sum += chunk[i] * corresponding_chunk_part[i];

    }

    return (sum / chunk_size);
}

void print_output_file(int *chunk, int *line1, int *line2, int *line3, int chunk_size, int (*operation1)(int*, int*, int), int (*operation2)(int*, int*, int),
                        int (*operation3)(int*, int*, int), int line_size, char *mode)
{
    int i;
    int result1[80], result2[80], result3[80];
    
    FILE *outfi = fopen("output.txt", "w");

    for (i = 0; i <= line_size - chunk_size; i++)
    {
        result1[i] = operation1(chunk, &line1[i], chunk_size);
        result2[i] = operation2(chunk, &line2[i], chunk_size);
        result3[i] = operation3(chunk, &line3[i], chunk_size);
    }

    if (strcmp(mode, "rgb") == 0)
    {
        fprintf(outfi, "RGB: -> ");
        for (i = 0; i < line_size - chunk_size; i++)
        {
            if (i > 0)
            {
                fprintf(outfi, ",");
            }

            fprintf(outfi, "[%d, %d, %d]", result1[i], result2[i], result3[i]);
        }
        fprintf(outfi, "\n");
    }

    else if (strcmp(mode, "bgr") == 0)
    {
        fprintf(outfi, "BGR: -> ");
        for (i = 0; i < line_size - chunk_size; i++)
        {
            if (i > 0)
            {
                fprintf(outfi, ",");
            }

            fprintf(outfi, "[%d, %d, %d]", result3[i], result2[i], result1[i]);
        }
        fprintf(outfi, "\n");
    }

    else
    {
        fprintf(outfi, "GRAY: -> ");
        fprintf(outfi, "[");
        for (i = 0; i < line_size - chunk_size; i++)
        {
            if (i > 0)
            {
                fprintf(outfi, ",");
            }

            fprintf(outfi, "%d", result1[i]);
        }
        fprintf(outfi, "]\n");

        fprintf(outfi, "[");
        for (i = 0; i < line_size - chunk_size; i++)
        {
            if (i > 0)
            {
                fprintf(outfi, ",");
            }

            fprintf(outfi, "%d", result2[i]);
        }
        fprintf(outfi, "]\n");

        fprintf(outfi, "[");
        for (i = 0; i < line_size - chunk_size; i++)
        {
            if (i > 0)
            {
                fprintf(outfi, ",");
            }

            fprintf(outfi, "%d", result3[i]);
        }
        fprintf(outfi, "]\n");
    }

    fclose(outfi);
  
}

void push(char stack[], int *top, char op)
{
    if (*top < 2)
    {
        (*top)++;
        stack[*top] = op;
    }
}

char pop(char stack[], int *top)
{
    if (*top >= 0)
    {
        return stack[(*top)--];
    }

    else
    {
        printf("Stack is empty.");
        return '\0';
    }
}

int main()
{
    int i;
    char stack[3];
    char temp[10];
    int top = -1;
    char temp2[10];
    char buffer[256];
    char *token;
    int chunk[9], line1[80], line2[80], line3[80];
    int chunk_size = 0;
    int line_size = 0;
    int n;
    char op1, op2, op3;
    int (*operation1)(int*, int*, int);
    int (*operation2)(int*, int*, int);
    int (*operation3)(int*, int*, int);

    FILE *infi = fopen("input.txt", "r");

    if (infi == NULL)
    {
        printf("The file does not exist.\n");
        return 1;
    }
   
    fgets(buffer, 256, infi); /*for operation input line*/
    fgets(buffer, 256, infi); /*for blank line*/
    fgets(buffer, 256, infi); /*for chunk line*/

    token = strtok(buffer, ",\n");
    while (token != NULL)
    {
        n = 0;

        while (*token)
        {
            n = n * 10 + (*token - '0');
            token++;
        }

        chunk[chunk_size++] = n;

        token = strtok(NULL, ",\n");
    }

    fgets(buffer, 256, infi); /*for line 1*/
    token = strtok(buffer, ",\n");
    while (token != NULL)
    {
        n = 0;

        while (*token)
        {
            n = n * 10 + (*token - '0');
            token++;
        }

        line1[line_size++] = n;

        token = strtok(NULL, ",\n");
    }
    line_size = 0;

    fgets(buffer, 256, infi); /*for line 2*/
    token = strtok(buffer, ",\n");
    while (token != NULL)
    {
        n = 0;

        while (*token)
        {
            n = n * 10 + (*token - '0');
            token++;
        }

        line2[line_size++] = n;

        token = strtok(NULL, ",\n");
    }
    line_size = 0;

    fgets(buffer, 256, infi); /*for line 3*/
    token = strtok(buffer, ",\n");
    while (token != NULL)
    {
        n = 0;

        while (*token)
        {
            n = n * 10 + (*token - '0');
            token++;
        }

        line3[line_size++] = n;

        token = strtok(NULL, ",\n");
    }

    for (i = 0; i < 3; i++)
    {
        printf("Enter 'max' or 'avg' for line %d: ", 4 - i);
        scanf("%s", temp);

        while (strcmp(temp, "max") != 0 && strcmp(temp, "avg") != 0)
        {
            printf("Invalid input. Please enter 'max' or 'avg': ");
            while(getchar() != '\n');
            scanf("%s", temp);
        }

        if (strcmp(temp, "max") == 0)
        {
            push(stack, &top, 'm');
        }

        else if (strcmp(temp, "avg") == 0)
        {
            push(stack, &top, 'a');
        }
    }

    printf("\nYou choose one of 'rgb', 'bgr' or 'gray': ");
    scanf("%s", temp2);

    while (strcmp(temp2, "rgb") != 0 && strcmp(temp2, "bgr") != 0 && strcmp(temp2, "gray") != 0)
    {
        printf("Invalid input. Please enter 'rgb', 'bgr' or 'gray': ");
        while(getchar() != '\n');
        scanf("%s", temp2);
    }

    op1 = pop(stack, &top); /*for line 2*/
    if (op1 == 'm')
        operation1 = max_operation;
    else 
        operation1 = avg_operation;

    op2 = pop(stack, &top); /*for line 3*/
    if (op2 == 'm')
        operation2 = max_operation;
    else 
        operation2 = avg_operation;

    op3 = pop(stack, &top); /*for line 4*/
    if (op3 == 'm')
        operation3 = max_operation;
    else 
        operation3 = avg_operation;
    
    
    print_output_file(chunk, line1, line2, line3, chunk_size, operation1, operation2, operation3, line_size, temp2);

    fclose(infi);
    return 0;
}
