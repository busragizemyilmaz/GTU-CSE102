#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    BACKEND_DEVELOPER,
    FRONTEND_DEVELOPER,
    FULLSTACK_DEVELOPER,
    MOBILE_DEVELOPER,
    EMBEDDED_SOFTWARE_ENGINEER,
    GAME_DEVELOPER,
    DEVOPS_ENGINEER,
    TEST_ENGINEER
} JobType;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char *name;
    char *mail;
    JobType job_type;
    int email_verified;
    Date date;
} CUSTOMER;


CUSTOMER* readData(int *size, int *capacity)
{
    CUSTOMER *person;
    char line[100];
    char *token;
    int i = 0;
    int count = 1;
    int tmp_job;

    FILE *f = fopen("input.txt", "r");

    if (f == NULL) 
    {
        printf("The file does not exist.\n");
        *size = 0;
        return NULL;
    }

    *capacity = 10;

    person = (CUSTOMER *)malloc((*capacity) * sizeof(CUSTOMER));

    while (fgets(line, sizeof(line), f) != NULL)
    {
        if (count == *capacity)
        {
            *capacity = *capacity * 2;

            person = realloc(person, (*capacity) * sizeof(CUSTOMER));
        }
        
        person[i].id = count++;

        token = strtok(line, ",");
        person[i].name = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(person[i].name, token);

        token = strtok(NULL, ",");
        person[i].mail = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(person[i].mail, token);

        token = strtok(NULL, ",");
        sscanf(token, "%d", &tmp_job);
        person[i].job_type = (JobType)tmp_job;

        token = strtok(NULL, ",");
        sscanf(token, "%d", &person[i].email_verified);

        token = strtok(NULL, ",");
        sscanf(token, "%d.%d.%d", &person[i].date.day, &person[i].date.month, &person[i].date.year);

        i++;
    }

    fclose(f);

    *size = count - 1;

    return person;
}

CUSTOMER* insert(CUSTOMER *person, int *size, char *command, int *capacity)
{
    int count = *size + 1;
    char name[100], mail[100], date[20];
    int job_type, email_verified;
    char *token;

    if (count == *capacity)
    {
        *capacity = *capacity * 2;

        person = realloc(person, (*capacity) * sizeof(CUSTOMER));
    }

    person[*size].id = count;

    if ((sscanf(command, "INSERT INTO CUSTOMER (“%[^”]”,“%[^”]”,%d,%d,“%[^”]”);", name, mail, &job_type, &email_verified, date)) == 5)
    {

        person[*size].name = (char *)malloc((strlen(name) + 1) * sizeof(char));
        strcpy(person[*size].name, name);

        person[*size].mail = (char *)malloc((strlen(mail) + 1) * sizeof(char));
        strcpy(person[*size].mail, mail);
       
        person[*size].job_type = job_type;
        
        person[*size].email_verified = email_verified;

        sscanf(date, "%d.%d.%d", &person[*size].date.day, &person[*size].date.month, &person[*size].date.year);
    }

    else
    {
        token = strtok(&command[21], ",");
        if (strlen(token) > 1)
            strcpy(name, token);
        else
            strcpy(name, "null");

        token = strtok(NULL, ",");
        strncpy(mail, token, strlen(token) - 3);
        mail[strlen(token) - 3] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL || strlen(token) == 0)
            job_type = 0;
        else
            job_type = atoi(token);

        token = strtok(NULL, ","); 
        if (token == NULL || strlen(token) == 0)
            email_verified = 0;
        else
            email_verified = atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) 
            strcpy(date, token);
        else
            strcpy(date, "null");


        person[*size].name = (char *)malloc((strlen(name) + 1) * sizeof(char));
        strcpy(person[*size].name, name);

        person[*size].mail = (char *)malloc((strlen(mail) + 1) * sizeof(char));
        strcpy(person[*size].mail, mail);

        person[*size].job_type = job_type;

        person[*size].email_verified = email_verified;

        sscanf(date, "%d.%d.%d", &person[*size].date.day, &person[*size].date.month, &person[*size].date.year);
    }
   
    (*size)++;

    return person;
}

CUSTOMER* delete(CUSTOMER *person, int *size, char *command, int *errorFlag)
{
    int id;
    int index = -1;
    int i;
    FILE *f;

    sscanf(command, "DELETE FROM CUSTOMER WHERE id=%d;", &id);

    for (i = 0; i < *size; i++)
    {
        if (person[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1) 
    {
        f = fopen("output.txt", "a");
        fprintf(f, "error\n");
        fprintf(f, "----------\n");
        fclose(f);

        *errorFlag = 1;

        return person;
    }

    free(person[index].name);
    free(person[index].mail);

    for (i = index; i < *size - 1; i++)
    {
        person[i] = person[i + 1];
    }

    (*size)--;
    *errorFlag = 0;

    return person;
}

CUSTOMER* update(CUSTOMER *person, int *size, char *command, int *errorFlag)
{
    char mail[100];
    int job_type;
    int id;
    int index = -1;
    int i;
    FILE *f;

    sscanf(command, "UPDATE CUSTOMER SET email=“%[^”]”, job_type=%d WHERE id=%d;", mail, &job_type, &id);

    for (i = 0; i < *size; i++)
    {
        if (person[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1) 
    {
        f = fopen("output.txt", "a");
        fprintf(f, "error\n");
        fprintf(f, "----------\n");
        fclose(f);

        *errorFlag = 1;

        return person;
    }

    strcpy(person[index].mail,mail);
    person[index].job_type = job_type;

    *errorFlag = 0;

    return person;

}

void Free(CUSTOMER *person, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        free(person[i].name);
        free(person[i].mail);
    }

    free(person);
}

CUSTOMER* truncate(CUSTOMER *person, int *size)
{
    Free(person, *size);

    *size = 0;

    person = (CUSTOMER *)malloc(10 * sizeof(CUSTOMER));

    return person;
}

char* print_JobType(JobType type)
{
    switch (type)
    {
        case BACKEND_DEVELOPER: return "BACKEND_DEVELOPER";
        case FRONTEND_DEVELOPER: return "FRONTEND_DEVELOPER"; 
        case FULLSTACK_DEVELOPER: return "FULLSTACK_DEVELOPER"; 
        case MOBILE_DEVELOPER: return "MOBILE_DEVELOPER";
        case EMBEDDED_SOFTWARE_ENGINEER: return "EMBEDDED_SOFTWARE_ENGINEER";
        case GAME_DEVELOPER: return "GAME_DEVELOPER";
        case DEVOPS_ENGINEER: return "DEVOPS_ENGINEER";
        case TEST_ENGINEER: return "TEST_ENGINEER";
        default: return "UNKNOWN";
    }
}

char* print_boolean(int n)
{
    if (n == 1)
        return "true";

    else
        return "false";
}

void print_output(CUSTOMER *person, int size)
{
    int i; 

    FILE *f = fopen("output.txt", "r");

    if (f == NULL)
        f = fopen("output.txt", "w");

    else
        f = fopen("output.txt", "a");

    for (i = 0; i < size; i++) 
    {
        fprintf(f, "%s,%s,%s,%s,%02d.%02d.%04d\n", 
            person[i].name, 
            person[i].mail, 
            print_JobType(person[i].job_type), 
            print_boolean(person[i].email_verified),
            person[i].date.day, 
            person[i].date.month, 
            person[i].date.year);
    }

    fprintf(f, "----------\n");

    fclose(f);
}

void main()
{
    int size;
    char commands[100];
    int errorFlag;
    int capacity;

    CUSTOMER *person = readData(&size, &capacity);

    FILE *f = fopen("commands.txt", "r");

    print_output(person, size);

    while(fgets(commands, sizeof(commands), f) != NULL)
    {
        if (strncmp(commands, "INSERT INTO CUSTOMER", 20) == 0)
        {
            person = insert(person, &size, commands, &capacity);
            print_output(person, size);
        }

        else if (strncmp(commands, "DELETE", 6) == 0)
        {
            person = delete(person, &size, commands, &errorFlag);

            if (!errorFlag)
                print_output(person, size);
        }

        else if (strncmp(commands, "UPDATE", 6) == 0)
        {
            person = update(person, &size, commands, &errorFlag);
           
            if (!errorFlag)
                print_output(person, size);
        }

        else if (strncmp(commands, "TRUNCATE", 8) == 0)
        {
            person = truncate(person, &size);
            print_output(person, size);
        }
    }

    Free(person, size);

    fclose(f);
}
