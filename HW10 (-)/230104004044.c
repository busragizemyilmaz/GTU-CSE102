#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() 
{
    char line[50000];
    int max_embedding_length;
    int word_embedding_dimension;
    int num_qa_pairs;
    int unique_count;
    char unique_chars[128];
    int i, j = 0;
    char questions[200][20000];
    char answers[200][20000];
    int question_index = 0;
    int answer_index = 0;
    int count = 0;

    FILE *file = fopen("embeddings.txt", "r");

    if (file == NULL) 
    {
        printf("Error opening file.\n");
        return 1;
    }

    //Metadata
    fgets(line, sizeof(line), file);

    // Metadata oku
    fgets(line, sizeof(line), file);
    sscanf(line, "Maximum embedding length: %d", &max_embedding_length);

    fgets(line, sizeof(line), file);
    sscanf(line, "Word embedding dimension: %d", &word_embedding_dimension);

    fgets(line, sizeof(line), file);
    sscanf(line, "Number of question-answer pairs: %d", &num_qa_pairs);

    fgets(line, sizeof(line), file);
    sscanf(line, "Unique count: %d", &unique_count);

    fgets(line, sizeof(line), file); // Unique chars line

    // Unique karakterleri al
    for (i = 13; line[i] != '\0'; i++) 
    {
        if (line[i] != '\n' && line[i] != '\r') 
        {
            unique_chars[j++] = line[i];
        }
    }

    printf("Metadata Read:\n");
    printf("- Maximum embedding length: %d\n", max_embedding_length);
    printf("- Word embedding dimension: %d\n", word_embedding_dimension);
    printf("- Number of question-answer pairs: %d\n", num_qa_pairs);
    printf("- Unique count: %d\n", unique_count);
    printf("- Unique chars: ");
    for (i = 0; i < unique_count; i++) 
    {
        printf("%c", unique_chars[i]);
    }
    printf("\n\n");

    fgets(line, sizeof(line), file); //###

    while (fgets(line, sizeof(line), file) != NULL) 
    {
        if (strncmp(line, "Question: ", 10) == 0) 
        {
            fgets(line, sizeof(line), file);

            printf("--> %d", sizeof(line));
            for (i = 0; i < 65; i++)
            {
                int a = line[i];
                count++;

                if (a == 1)
                {
                    printf("count: %d", count);

                }
            }



        } 
        
        else if (strncmp(line, "Answer: ", 8) == 0) 
        {
            
        }
    }

    printf("Questions and Answers Decoded:\n\n");
    for (i = 0; i < question_index && i < answer_index; i++) 
    {
        printf("Q%d: %s\n", i + 1, questions[i]);
        printf("A%d: %s\n\n", i + 1, answers[i]);
    }

    fclose(file);
    return 0;
}
