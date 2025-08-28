#include <stdio.h>
#include <string.h>

void read_data(char qtemp[200][1000], char atemp[200][1000], int *count)
{
    char line[1000];
    int len;

    FILE *data = fopen("database.txt", "r");

    if (data == NULL)
    {
        printf("The file does not exist.");
        return;
    }

    *count = 0;
    while (fgets(line, 1000, data) != NULL)
    {
        len = strlen(line);

        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
            len--;
        }

        if (len > 0 && line[len - 1] == '\r')
        {
            line[len - 1] = '\0';
        }

        if (strcmp(line, "---") == 0)
        {
            continue;
        }

        if (strncmp(line, "Q: ", 3) == 0)
        {
            strcpy(qtemp[*count], &line[3]);
        }

        else if (strncmp(line, "A: ", 3) == 0)
        {
            strcpy(atemp[*count], &line[3]);
            (*count)++;
        }
    }

    fclose(data);
}

void tokenize(const char *sentence, char tokens[100][100], int *token_count)
{
    int i = 0;
    char temp[1000];
    char *token;

    strcpy(temp, sentence);

    token = strtok(temp, " ");
    while (token != NULL)
    {
        strcpy(tokens[i], token);

        i++;
        token = strtok(NULL, " ");
    }

    *token_count = i;
}

int is_in_array(char ch, char characters[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (characters[i] == ch)
        {
            return 1;
        }
    }

    return 0;
}

void build_unique_chars(char unique_chars[], int *unique_count)
{
    int i;
    char line[1000];
    int len;
    char ch;
    
    FILE *data = fopen("database.txt", "r");

    if (data == NULL)
    {
        printf("Error: Cannot open dataset.txt\n");
        return;
    }

    *unique_count = 0;
    while (fgets(line, 1000, data) != NULL)
    {
        len = strlen(line);

        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
            len--;
        }

        if (len > 0 && line[len - 1] == '\r')
        {
            line[len - 1] = '\0';
        }

        for (i = 3; line[i] != '\0'; i++)
        {
            ch = line[i];

            if (ch == ' ' || ch == '\r' || ch == '\n')
            {
                continue;
            }
            
            if (!is_in_array(ch, unique_chars, *unique_count))
            {
                unique_chars[*unique_count] = ch;
                (*unique_count)++;
            }
        }
    }

    fclose(data);
}

void one_hot_encode_word(char *word, char unique_chars[], int unique_count, int embedding[], int *embedding_pos, int max_word_len)
{
    int i, j;
    int word_len = strlen(word);
    int pad = max_word_len - word_len;
    char ch;

    *embedding_pos = 0;

    for (i = 0; i < pad; i++)
    {
        for (j = 0; j < unique_count; j++)
        {
            embedding[*embedding_pos] = 0;
            (*embedding_pos)++;
        }
    }

    for (i = 0; word[i] != '\0'; i++)
    {
        ch = word[i];

        for (j = 0; j < unique_count; j++)
        {
            if (unique_chars[j] == ch)
            {
                embedding[*embedding_pos] = 1;
            }

            else
            {
                embedding[*embedding_pos] = 0;
            }

            (*embedding_pos)++;
        }
    }
}

void one_hot_encode_sentence(char *sentence, char unique_chars[], int unique_count, int max_word_len, int embedding[], int *embedding_len, int max_embedding_length)
{
    int i, j;
    char tokens[100][100];
    int token_count = 0;
    int temp_embedding[100000];
    int temp_len = 0;
    int pad_len;

    tokenize(sentence, tokens, &token_count);

    for (i = 0; i < token_count; i++)
    {
        one_hot_encode_word(tokens[i], unique_chars, unique_count, &temp_embedding[temp_len], &j, max_word_len);
        temp_len += j;
    }

    pad_len = max_embedding_length - temp_len;

    for (i = 0; i < pad_len; i++)
    {
        embedding[i] = 0;
    }

    for (i = 0; i < temp_len; i++)
    {
        embedding[pad_len + i] = temp_embedding[i];
    }

    *embedding_len = max_embedding_length;
}


int find_max_word_length(char questions[200][1000], char answers[200][1000], int count)
{
    int i, j;
    int max_len = 0;
    char tokens[100][100];
    int token_count;
    int len;

    for (i = 0; i < count; i++)
    {
        tokenize(questions[i], tokens, &token_count);
        for (j = 0; j < token_count; j++)
        {
            len = strlen(tokens[j]);
            if (len > max_len)
            {
                max_len = len;
            }
        }

        tokenize(answers[i], tokens, &token_count);
        for (j = 0; j < token_count; j++)
        {
            len = strlen(tokens[j]);
            if (len > max_len)
            {
                max_len = len;
            }
        }
    }

    return max_len;
}

int find_max_embedding_length(char questions[200][1000], char answers[200][1000], int count, int max_word_len, int unique_count)
{
    int i;
    int max_len = 0;
    char tokens[100][100];
    int token_count;
    int qlen, alen;

    for (i = 0; i < count; i++)
    {
        tokenize(questions[i], tokens, &token_count);
        qlen = token_count * max_word_len * unique_count;
        if (qlen > max_len)
        {
            max_len = qlen;
        }

        tokenize(answers[i], tokens, &token_count);
        alen = token_count * max_word_len * unique_count;
        if (alen > max_len)
        {
            max_len = alen;
        }
    }

    return max_len;
}

void write_embeddings_file(char questions[200][1000], char answers[200][1000], int count, char unique_chars[], int unique_count, int max_word_len)
{
    int i, j;
    int max_embedding_length = find_max_embedding_length(questions, answers, count, max_word_len, unique_count);

    int embeddings[2][200][1000] = {{{0}}};         
    int embedding_lengths[2][200] = {{0}};           

    FILE *outfile = fopen("embeddings.txt", "w");

    if (outfile == NULL)
    {
        printf("Error: Cannot create output file\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        one_hot_encode_sentence(questions[i], unique_chars, unique_count, max_word_len, embeddings[0][i], &embedding_lengths[0][i], max_embedding_length);

        one_hot_encode_sentence(answers[i], unique_chars, unique_count, max_word_len, embeddings[1][i], &embedding_lengths[1][i], max_embedding_length);
    }

    fprintf(outfile, "Metadata part\n");
    fprintf(outfile, "Maximum embedding length: %d\n", max_embedding_length);
    fprintf(outfile, "Word embedding dimension: %d\n", unique_count * max_word_len);
    fprintf(outfile, "Number of question-answer pairs: %d\n", count);
    fprintf(outfile, "Unique count: %d\n", unique_count);
    fprintf(outfile, "Unique chars: ");
    for (i = 0; i < unique_count; i++)
    {
        fprintf(outfile, "%c", unique_chars[i]);
    }
    fprintf(outfile, "\n");
    fprintf(outfile, "#####\n");

    for (i = 0; i < count; i++)
    {
        fprintf(outfile, "Question: ");
        for (j = 0; j < embedding_lengths[0][i]; j++)
        {
            fprintf(outfile, "%d", embeddings[0][i][j]);
        }
        fprintf(outfile, "\n");
        
        fprintf(outfile, "Answer: ");
        for (j = 0; j < embedding_lengths[1][i]; j++)
        {
            fprintf(outfile, "%d", embeddings[1][i][j]);
        }

        if (i < count - 1)
        {
            fprintf(outfile, "\n---\n");
        }
    }

    fclose(outfile);
    printf("Embedding file created successfully!\n");
}

int main()
{
    char questions[200][1000], answers[200][1000];
    char unique_chars[256];
    int count, unique_count;
    int max_word_len;

    read_data(questions, answers, &count);
    build_unique_chars(unique_chars, &unique_count);
    max_word_len = find_max_word_length(questions, answers, count);

    write_embeddings_file(questions, answers, count, unique_chars, unique_count, max_word_len);

    return 0;
}