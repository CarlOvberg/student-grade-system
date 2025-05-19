#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100

typedef struct {
    char fullName[100];
    int id;
    float midterm;
    float finalExam;
    float project;
    float gpa;
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

// Function declarations
void menu();
void addStudent();
void listStudents();
void deleteStudent();
void searchStudent();
void updateGrades();
int loadStudentsFromFile();
void sortByGPA();
float calculateGPA(float midterm, float finalExam, float project);
int getValidInt(const char* prompt);
int getValidIntInRange(const char* prompt, int min, int max);
void getValidName(char* dest, int size, const char* prompt);
int main() {
    int choice;

    while (1) {
        menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addStudent();
            break;
        case 2:
            listStudents();
            break;
        case 3:
            deleteStudent();
            break;
        case 4:
            searchStudent();
            break;
        case 5:
            updateGrades();
            break;
        case 0:
            printf("Exiting program...\n");
            return 0;
        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}

void menu() {
    printf("\n=== STUDENT GRADE SYSTEM ===\n");
    printf("1. Add Student\n");
    printf("2. List Students\n");
    printf("3. Delete Student\n");
    printf("4. Search Student\n");
    printf("5. Update Grades\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}

float calculateGPA(float midterm, float finalExam, float project) {
    return 0.4 * midterm + 0.5 * finalExam + 0.1 * project;
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Maximum student limit reached!\n");
        return;
    }

    Student newStudent;
    getchar(); // clean leftover newline
    getValidName(newStudent.fullName, sizeof(newStudent.fullName), "Enter Full Name: ");

    newStudent.id = getValidIntInRange("Enter Student ID: ", 1, 99999999);

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == newStudent.id) {
            printf("This ID is already in use!\n");
            return;
        }
    }

    newStudent.midterm = getValidIntInRange("Enter Midterm Grade (0-100): ", 0, 100);
    newStudent.finalExam = getValidIntInRange("Enter Final Grade (0-100): ", 0, 100);
    newStudent.project = getValidIntInRange("Enter Project Grade (0-100): ", 0, 100);

    newStudent.gpa = calculateGPA(newStudent.midterm, newStudent.finalExam, newStudent.project);

    students[studentCount++] = newStudent;

    FILE* file = fopen("students.txt", "a");
    if (file == NULL) {
        printf("Could not open file!\n");
        return;
    }

    fprintf(file, "%d,%s,%.2f,%.2f,%.2f,%.2f\n",
        newStudent.id,
        newStudent.fullName,
        newStudent.midterm,
        newStudent.finalExam,
        newStudent.project,
        newStudent.gpa);

    fclose(file);

    printf("Student added successfully.\n");
}



int loadStudentsFromFile() {
    FILE* file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("No student file found.\n");
        return 0;
    }

    studentCount = 0;
    while (fscanf(file, "%d,%[^,],%f,%f,%f,%f\n",
        &students[studentCount].id,
        students[studentCount].fullName,
        &students[studentCount].midterm,
        &students[studentCount].finalExam,
        &students[studentCount].project,
        &students[studentCount].gpa) == 6) {
        studentCount++;
        if (studentCount >= MAX_STUDENTS) break;
    }

    fclose(file);
    return studentCount;
}

void sortByGPA() {
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (students[j].gpa < students[j + 1].gpa) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

void listStudents() {
    if (loadStudentsFromFile() == 0) {
        printf("No students to list.\n");
        return;
    }

    sortByGPA();

    printf("\n%-10s %-25s %-6s %-6s %-6s %-6s\n", "ID", "Full Name", "Mid", "Final", "Proj", "GPA");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < studentCount; i++) {
        printf("%-10d %-25s %-6.1f %-6.1f %-6.1f %-6.2f\n",
            students[i].id,
            students[i].fullName,
            students[i].midterm,
            students[i].finalExam,
            students[i].project,
            students[i].gpa);
    }
}

void deleteStudent() {
    int deleteId;
    printf("Enter the ID of the student to delete: ");
    scanf("%d", &deleteId);

    int found = 0;

    loadStudentsFromFile();

    FILE* file = fopen("students.txt", "w");
    if (file == NULL) {
        printf("Could not open file!\n");
        return;
    }

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == deleteId) {
            found = 1;
            continue;
        }

        fprintf(file, "%d,%s,%.2f,%.2f,%.2f,%.2f\n",
            students[i].id,
            students[i].fullName,
            students[i].midterm,
            students[i].finalExam,
            students[i].project,
            students[i].gpa);
    }

    fclose(file);

    if (found)
        printf("Student deleted.\n");
    else
        printf("Student with that ID not found.\n");
}

void searchStudent() {
    loadStudentsFromFile();

    int choice;
    printf("\nSearch by:\n");
    printf("1. ID\n");
    printf("2. Full Name\n");
    printf("Choice: ");
    scanf("%d", &choice);
    getchar();

    int found = 0;

    if (choice == 1) {
        int id;
        printf("Enter ID: ");
        scanf("%d", &id);

        for (int i = 0; i < studentCount; i++) {
            if (students[i].id == id) {
                found = 1;
                printf("\nFound:\n");
                printf("Full Name: %s\n", students[i].fullName);
                printf("ID: %d\n", students[i].id);
                printf("Midterm: %.2f | Final: %.2f | Project: %.2f | GPA: %.2f\n",
                    students[i].midterm,
                    students[i].finalExam,
                    students[i].project,
                    students[i].gpa);
                break;
            }
        }
    }
    else if (choice == 2) {
        char name[100];
        printf("Enter Full Name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].fullName, name) == 0) {
                found = 1;
                printf("\nFound:\n");
                printf("Full Name: %s\n", students[i].fullName);
                printf("ID: %d\n", students[i].id);
                printf("Midterm: %.2f | Final: %.2f | Project: %.2f | GPA: %.2f\n",
                    students[i].midterm,
                    students[i].finalExam,
                    students[i].project,
                    students[i].gpa);
            }
        }
    }
    else {
        printf("Invalid choice.\n");
        return;
    }

    if (!found) {
        printf("Student not found.\n");
    }
}

void updateGrades() {
    int updateId;
    printf("Enter the ID of the student to update: ");
    scanf("%d", &updateId);

    loadStudentsFromFile();

    int found = 0;

    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == updateId) {
            found = 1;

            printf("Enter new Midterm Grade: ");
            scanf("%f", &students[i].midterm);
            printf("Enter new Final Grade: ");
            scanf("%f", &students[i].finalExam);
            printf("Enter new Project Grade: ");
            scanf("%f", &students[i].project);

            students[i].gpa = calculateGPA(students[i].midterm, students[i].finalExam, students[i].project);
            break;
        }
    }

    if (!found) {
        printf("Student not found.\n");
        return;
    }

    FILE* file = fopen("students.txt", "w");
    if (file == NULL) {
        printf("Could not open file!\n");
        return;
    }

    for (int i = 0; i < studentCount; i++) {
        fprintf(file, "%d,%s,%.2f,%.2f,%.2f,%.2f\n",
            students[i].id,
            students[i].fullName,
            students[i].midterm,
            students[i].finalExam,
            students[i].project,
            students[i].gpa);
    }

    fclose(file);
    printf("Grades updated.\n");
}
int getValidInt(const char* prompt) {
    char input[100];
    int value;
    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &value) == 1)
            return value;

        printf("Invalid input! Please enter a valid number.\n");
    }
}
void getValidName(char* dest, int size, const char* prompt) {
    char input[100];

    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        int valid = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (!(isalpha(input[i]) || input[i] == ' ')) {
                valid = 0;
                break;
            }
        }

        if (valid && strlen(input) > 0) {
            strncpy(dest, input, size);
            return;
        }

        printf("Invalid name! Please use only letters and spaces.\n");
    }
}
int getValidIntInRange(const char* prompt, int min, int max) {
    char input[100];
    int value;

    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &value) == 1 && value >= min && value <= max)
            return value;

        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
    }
}
