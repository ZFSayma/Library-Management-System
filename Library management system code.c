#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Book structure
struct books {
    int id;
    char bookName[50];
    char authorName[50];
    char date[12];
} b;

// Student structure
struct student {
    int id;
    char sName[50];
    char sClass[50];
    int sRoll;
    char bookName[50];
    char date[12];
} s;

FILE *fp;

// Function prototypes
void addBook();
void booksList();
void del();
void issueBook();
void issueList();

int main() {
    int ch;

    while (1) {
        system("cls"); // Clear screen (use "clear" on Linux/Mac)
        printf("<== Library Management System ==>\n");
        printf("1. Add Book\n");
        printf("2. Books List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Book List\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        getchar(); // Clear newline after scanf

        switch (ch) {
            case 0:
                exit(0);

            case 1:
                addBook();
                break;

            case 2:
                booksList();
                break;

            case 3:
                del();
                break;

            case 4:
                issueBook();
                break;

            case 5:
                issueList();
                break;

            default:
                printf("Invalid Choice...\n\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }

    return 0;
}

void addBook() {
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(b.date, myDate);

    fp = fopen("books.txt", "ab");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter book ID: ");
    scanf("%d", &b.id);
    getchar(); // clear newline

    printf("Enter book name: ");
    fgets(b.bookName, sizeof(b.bookName), stdin);
    b.bookName[strcspn(b.bookName, "\n")] = 0;

    printf("Enter author name: ");
    fgets(b.authorName, sizeof(b.authorName), stdin);
    b.authorName[strcspn(b.authorName, "\n")] = 0;

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book Added Successfully!\n");
}

void booksList() {
    system("cls");
    printf("<== Available Books ==>\n\n");
    printf("%-10s %-30s %-20s %s\n\n", "Book ID", "Book Name", "Author", "Date");

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("No books available.\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %s\n", b.id, b.bookName, b.authorName, b.date);
    }

    fclose(fp);
}

void del() {
    int id, found = 0;
    FILE *ft;

    system("cls");
    printf("<== Remove Book ==>\n\n");
    printf("Enter book ID to remove: ");
    scanf("%d", &id);

    fp = fopen("books.txt", "rb");
    ft = fopen("temp.txt", "wb");

    if (fp == NULL || ft == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            found = 1;
        } else {
            fwrite(&b, sizeof(b), 1, ft);
        }
    }

    fclose(fp);
    fclose(ft);

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found) {
        printf("Book deleted successfully.\n");
    } else {
        printf("Book with ID %d not found.\n", id);
    }
}

void issueBook() {
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(s.date, myDate);

    int found = 0;

    system("cls");
    printf("<== Issue Book ==>\n\n");

    printf("Enter Book ID to issue: ");
    scanf("%d", &s.id);
    getchar(); // clear newline

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("Books file not found.\n");
        return;
    }

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == s.id) {
            strcpy(s.bookName, b.bookName);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("No book found with ID %d.\n", s.id);
        return;
    }

    fp = fopen("issue.txt", "ab");
    if (fp == NULL) {
        printf("Error opening issue file.\n");
        return;
    }

    printf("Enter student name: ");
    fgets(s.sName, sizeof(s.sName), stdin);
    s.sName[strcspn(s.sName, "\n")] = 0;

    printf("Enter student class: ");
    fgets(s.sClass, sizeof(s.sClass), stdin);
    s.sClass[strcspn(s.sClass, "\n")] = 0;

    printf("Enter student roll number: ");
    scanf("%d", &s.sRoll);
    getchar(); // clear newline

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Book issued successfully!\n");
}

void issueList() {
    system("cls");
    printf("<== Issued Book List ==>\n\n");
    printf("%-10s %-30s %-20s %-10s %-30s %s\n\n", "S.ID", "Name", "Class", "Roll", "Book Name", "Date");

    fp = fopen("issue.txt", "rb");
    if (fp == NULL) {
        printf("No issued books found.\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %-10d %-30s %s\n", s.id, s.sName, s.sClass, s.sRoll, s.bookName, s.date);
    }

    fclose(fp);
}

