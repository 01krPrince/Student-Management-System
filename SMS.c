#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "students.txt"

typedef struct {
    char name[50];
    int roll;
    char phone[15];
    char class[10];
    float fee_payment;
} Student;

void add_student();
void display_students();
void update_student(int roll);
void delete_student(int roll);
void find_student(int roll);
void list_menu();

void add_student() {
    FILE *file = fopen(FILENAME, "a"); // Open in append mode
    if (!file) {
        perror("Unable to open file");
        return;
    }

    Student s;
    printf("Enter student name: ");
    getchar(); // Clear the buffer
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0; // Remove newline

    printf("Enter roll number: ");
    scanf("%d", &s.roll);

    printf("Enter phone number: ");
    getchar(); // Clear the buffer
    fgets(s.phone, sizeof(s.phone), stdin);
    s.phone[strcspn(s.phone, "\n")] = 0; // Remove newline

    printf("Enter class: ");
    fgets(s.class, sizeof(s.class), stdin);
    s.class[strcspn(s.class, "\n")] = 0; // Remove newline

    printf("Enter fee payment: ");
    scanf("%f", &s.fee_payment);

    // Debug print
    printf("Saving: %s,%d,%s,%s,%.2f\n", s.name, s.roll, s.phone, s.class, s.fee_payment);
    
    fprintf(file, "%s,%d,%s,%s,%.2f\n", s.name, s.roll, s.phone, s.class, s.fee_payment);
    fclose(file);
    printf("Student added successfully!\n");
}

void display_students() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    Student s;
    int count = 0; // Counter for the number of students
    printf("\n--- Student List ---\n");
    while (fscanf(file, "%49[^,],%d,%14[^,],%9[^,],%f\n", s.name, &s.roll, s.phone, s.class, &s.fee_payment) == 5) {
        printf("%-20s %-10d %-15s %-10s %.2f\n", s.name, s.roll, s.phone, s.class, s.fee_payment);
        count++;
    }

    if (count == 0) {
        printf("No students found.\n");
    }

    fclose(file);
}

void update_student(int roll) {
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w"); // Temporary file for updates
    if (!file || !temp) {
        perror("Unable to open file");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    Student s;
    int found = 0;
    while (fscanf(file, "%49[^,],%d,%14[^,],%9[^,],%f\n", s.name, &s.roll, s.phone, s.class, &s.fee_payment) == 5) {
        if (s.roll == roll) {
            printf("Updating details for Roll: %d\n", roll);
            printf("Enter new student name: ");
            getchar(); // Clear the buffer
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0; // Remove newline

            printf("Enter new phone number: ");
            fgets(s.phone, sizeof(s.phone), stdin);
            s.phone[strcspn(s.phone, "\n")] = 0; // Remove newline

            printf("Enter new class: ");
            fgets(s.class, sizeof(s.class), stdin);
            s.class[strcspn(s.class, "\n")] = 0; // Remove newline

            printf("Enter new fee payment: ");
            scanf("%f", &s.fee_payment);
            found = 1;
        }
        fprintf(temp, "%s,%d,%s,%s,%.2f\n", s.name, s.roll, s.phone, s.class, s.fee_payment);
    }

    fclose(file);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found) {
        printf("Student updated successfully!\n");
    } else {
        printf("Student with roll %d not found.\n", roll);
    }
}

void delete_student(int roll) {
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w"); // Temporary file for deletions
    if (!file || !temp) {
        perror("Unable to open file");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    Student s;
    int found = 0;
    while (fscanf(file, "%49[^,],%d,%14[^,],%9[^,],%f\n", s.name, &s.roll, s.phone, s.class, &s.fee_payment) == 5) {
        if (s.roll == roll) {
            found = 1;
            continue; // Skip this student
        }
        fprintf(temp, "%s,%d,%s,%s,%.2f\n", s.name, s.roll, s.phone, s.class, s.fee_payment);
    }

    fclose(file);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found) {
        printf("Student deleted successfully!\n");
    } else {
        printf("Student with roll %d not found.\n", roll);
    }
}

void find_student(int roll) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    Student s;
    int found = 0;
    while (fscanf(file, "%49[^,],%d,%14[^,],%9[^,],%f\n", s.name, &s.roll, s.phone, s.class, &s.fee_payment) == 5) {
        if (s.roll == roll) {
            printf("Name: %s, Roll: %d, Phone: %s, Class: %s, Fee Payment: %.2f\n",
                   s.name, s.roll, s.phone, s.class, s.fee_payment);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Student with roll %d not found.\n", roll);
    }
}

void list_menu() {
    int choice, roll;

    while (1) {
        printf("\n--- Student Management System ---\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Update Student\n");
        printf("4. Delete Student\n");
        printf("5. Find Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_student();
                break;
            case 2:
                display_students();
                break;
            case 3:
                printf("Enter roll number to update: ");
                scanf("%d", &roll);
                update_student(roll);
                break;
            case 4:
                printf("Enter roll number to delete: ");
                scanf("%d", &roll);
                delete_student(roll);
                break;
            case 5:
                printf("Enter roll number to find: ");
                scanf("%d", &roll);
                find_student(roll);
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    list_menu();
    return 0;
}
