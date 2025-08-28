#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LECTURES 5
#define MAX_NAME_LEN 100

/* Enum for lectures */
typedef enum {
    CSE101 = 0,
    CSE102,
    CSE103,
    CSE104,
    CSE105,
    LECTURE_COUNT
} Lecture;

const char *lecture_names[] = {"CSE101", "CSE102", "CSE103", "CSE104", "CSE105"};
const int lecture_credits[] = {3, 4, 3, 2, 5};

/* Custom strcspn replacement */
size_t my_strcspn(const char *s, const char *reject) {
    size_t i, j;
    for (i = 0; s[i] != '\0'; i++) {
        for (j = 0; reject[j] != '\0'; j++) {
            if (s[i] == reject[j]) {
                return i;
            }
        }
    }
    return i;
}

/* Student structure */
typedef struct Student {
    char name[MAX_NAME_LEN];
    int id;
    int grades[LECTURE_COUNT]; /* -1 means not taken */
    struct Student *next;
} Student;

Student *head = NULL;
int next_id = 1;

int getLectureIndex(const char *name) {
    int i;
    for (i = 0; i < LECTURE_COUNT; i++) {
        if (strcmp(name, lecture_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

double calculateGPA(Student *s) {
    int i, total_credit = 0;
    double weighted_sum = 0;
    for (i = 0; i < LECTURE_COUNT; i++) {
        if (s->grades[i] != -1) {
            total_credit += lecture_credits[i];
            weighted_sum += s->grades[i] * lecture_credits[i];
        }
    }
    return total_credit ? weighted_sum / total_credit : 0;
}

void printAllIds() {
    Student *curr = head;
    while (curr) {
        printf("%d", curr->id);
        if (curr->next) printf(",");
        curr = curr->next;
    }
    printf("\n");
}

void addStudent() {
    Student *new_s = (Student *)malloc(sizeof(Student));
    if (!new_s) return;

    printf("Enter student name: ");
    fgets(new_s->name, MAX_NAME_LEN, stdin);
    new_s->name[my_strcspn(new_s->name, "\n")] = '\0';

    int i;
    for (i = 0; i < LECTURE_COUNT; i++) new_s->grades[i] = -1;

    char lectures[200], grades[200];
    printf("Enter lectures: ");
    fgets(lectures, 200, stdin);
    lectures[my_strcspn(lectures, "\n")] = '\0';

    printf("Enter grades: ");
    fgets(grades, 200, stdin);
    grades[my_strcspn(grades, "\n")] = '\0';

    char *lec = strtok(lectures, ",");
    char *grd = strtok(grades, ",");

    while (lec && grd) {
        int idx = getLectureIndex(lec);
        if (idx != -1)
            new_s->grades[idx] = atoi(grd);
        lec = strtok(NULL, ",");
        grd = strtok(NULL, ",");
    }

    new_s->id = next_id++;
    new_s->next = head;
    head = new_s;

    printf("Student added.\n");
    printf("id : ");
    printAllIds();
}

Student* findStudentById(int id) {
    Student *curr = head;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

void removeStudent() {
    int id;
    printf("Enter student id: ");
    scanf("%d", &id);
    getchar();

    Student *curr = head, *prev = NULL;
    while (curr) {
        if (curr->id == id) {
            if (prev) prev->next = curr->next;
            else head = curr->next;
            free(curr);
            printf("Student was removed.\n");
            printf("id : ");
            printAllIds();
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Student not found.\n");
}

void searchStudent() {
    int id;
    printf("Enter student id: ");
    scanf("%d", &id);
    getchar();
    Student *s = findStudentById(id);
    if (!s) {
        printf("Student  not found.\n");
        return;
    }
    printf("-------------------------------------------------------------------------------------------\n");
    printf("Name Lectures   Grades  GPA\n");
    printf("-------------------------------------------------------------------------------------------\n");
    int i;
    printf("%s ", s->name);
    for (i = 0; i < LECTURE_COUNT; i++)
        if (s->grades[i] != -1)
            printf("%s,", lecture_names[i]);
    printf(" ");
    for (i = 0; i < LECTURE_COUNT; i++)
        if (s->grades[i] != -1)
            printf("%d,", s->grades[i]);
    printf(" %.3lf\n", calculateGPA(s));
}

void addLecture() {
    int id;
    char lectures[200], grades[200];
    printf("Enter student id: ");
    scanf("%d", &id);
    getchar();
    Student *s = findStudentById(id);
    if (!s) {
        printf("Student not found.\n");
        return;
    }
    printf("Enter lectures: ");
    fgets(lectures, 200, stdin);
    lectures[my_strcspn(lectures, "\n")] = '\0';
    printf("Enter grades: ");
    fgets(grades, 200, stdin);
    grades[my_strcspn(grades, "\n")] = '\0';

    char *lec = strtok(lectures, ",");
    char *grd = strtok(grades, ",");

    while (lec && grd) {
        int idx = getLectureIndex(lec);
        if (idx != -1) s->grades[idx] = atoi(grd);
        lec = strtok(NULL, ",");
        grd = strtok(NULL, ",");
    }
    searchStudent();
}

void removeLecture() {
    int id;
    char lectures[200];
    printf("Enter student id: ");
    scanf("%d", &id);
    getchar();
    Student *s = findStudentById(id);
    if (!s) {
        printf("Student not found.\n");
        return;
    }
    printf("Enter lectures: ");
    fgets(lectures, 200, stdin);
    lectures[my_strcspn(lectures, "\n")] = '\0';

    char *lec = strtok(lectures, ",");
    while (lec) {
        int idx = getLectureIndex(lec);
        if (idx != -1) s->grades[idx] = -1;
        lec = strtok(NULL, ",");
    }
    searchStudent();
}

void printList() {
    Student *curr = head;
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Name  Lectures   Grades  GPA       id\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    while (curr) {
        int i;
        printf("%s ", curr->name);
        for (i = 0; i < LECTURE_COUNT; i++)
            if (curr->grades[i] != -1)
                printf("%s,", lecture_names[i]);
        printf(" ");
        for (i = 0; i < LECTURE_COUNT; i++)
            if (curr->grades[i] != -1)
                printf("%d,", curr->grades[i]);
        printf(" %.3lf       %d\n", calculateGPA(curr), curr->id);
        curr = curr->next;
    }
}

void freeList() {
    Student *curr = head;
    while (curr) {
        Student *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    head = NULL;
}

int main() {
    int choice;
    do {
        printf("\nMenu\n1. Add Student\n2. Remove Student\n3. Search Student\n4. Add Lecture\n5. Remove Lecture\n6. Print ID\n7. Print List\n8. Exit\n\nChoice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: addStudent(); break;
            case 2: removeStudent(); break;
            case 3: searchStudent(); break;
            case 4: addLecture(); break;
            case 5: removeLecture(); break;
            case 6: printf("id : "); printAllIds(); break;
            case 7: printList(); break;
            case 8: freeList(); printf("Freeing memory... Have a nice day!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 8);
    return 0;
}
