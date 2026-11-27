#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFFER_SIZE 1024

// Function to get filename
void getFilename(char *filename)
{
    printf("Enter filename: ");
    fgets(filename, 256, stdin);
    filename[strcspn(filename, "\n")] = '\0';
}

// Function to write all data
int writeAll(int fd, const char *data, size_t length)
{
    size_t total = 0;

    while (total < length)
    {
        ssize_t n = write(fd, data + total, length - total);

        if (n == -1)
            return -1;

        total += n;
    }

    return 0;
}

// Create File
void createFile()
{
    char filename[256];
    getFilename(filename);

    int fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0644);

    if (fd == -1)
        perror("Error creating file");
    else
    {
        printf("File created successfully.\n");
        close(fd);
    }
}

// Write to File
void writeFile()
{
    char filename[256], data[BUFFER_SIZE];

    getFilename(filename);

    printf("Enter text: ");
    fgets(data, sizeof(data), stdin);

    int fd = open(filename, O_WRONLY | O_TRUNC);

    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }

    if (writeAll(fd, data, strlen(data)) == -1)
        perror("Error writing to file");
    else
        printf("Data written successfully.\n");

    close(fd);
}

// Read File
void readFile()
{
    char filename[256], buffer[BUFFER_SIZE];

    getFilename(filename);

    int fd = open(filename, O_RDONLY);

    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }

    printf("\nFile Contents:\n");

    ssize_t bytes;

    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
        write(STDOUT_FILENO, buffer, bytes);

    if (bytes == -1)
        perror("Error reading file");

    printf("\n");
    close(fd);
}

// Append to File
void appendFile()
{
    char filename[256], data[BUFFER_SIZE];

    getFilename(filename);

    printf("Enter text to append: ");
    fgets(data, sizeof(data), stdin);

    int fd = open(filename, O_WRONLY | O_APPEND);

    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }

    if (writeAll(fd, data, strlen(data)) == -1)
        perror("Error appending data");
    else
        printf("Data appended successfully.\n");

    close(fd);
}

// Copy File
void copyFile()
{
    char source[256], destination[256];
    char buffer[BUFFER_SIZE];

    printf("Enter source filename: ");
    fgets(source, sizeof(source), stdin);
    source[strcspn(source, "\n")] = '\0';

    printf("Enter destination filename: ");
    fgets(destination, sizeof(destination), stdin);
    destination[strcspn(destination, "\n")] = '\0';

    if (strcmp(source, destination) == 0)
    {
        printf("Source and destination must be different.\n");
        return;
    }

    int src = open(source, O_RDONLY);

    if (src == -1)
    {
        perror("Error opening source file");
        return;
    }

    int dest = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (dest == -1)
    {
        perror("Error creating destination file");
        close(src);
        return;
    }

    ssize_t bytes;
    int error = 0;

    while ((bytes = read(src, buffer, sizeof(buffer))) > 0)
    {
        if (writeAll(dest, buffer, bytes) == -1)
        {
            perror("Error copying data");
            error = 1;
            break;
        }
    }

    if (bytes == -1)
    {
        perror("Error reading source");
        error = 1;
    }

    if (close(src) == -1)
        perror("Error closing source");

    if (close(dest) == -1)
    {
        perror("Error closing destination");
        error = 1;
    }

    if (!error)
        printf("File copied successfully.\n");
}

// Rename File
void renameFile()
{
    char oldname[256], newname[256];

    printf("Enter old filename: ");
    fgets(oldname, sizeof(oldname), stdin);
    oldname[strcspn(oldname, "\n")] = '\0';

    printf("Enter new filename: ");
    fgets(newname, sizeof(newname), stdin);
    newname[strcspn(newname, "\n")] = '\0';

    if (rename(oldname, newname) == 0)
        printf("File renamed successfully.\n");
    else
        perror("Error renaming file");
}

// Delete File
void deleteFile()
{
    char filename[256];
    getFilename(filename);

    if (unlink(filename) == 0)
        printf("File deleted successfully.\n");
    else
        perror("Error deleting file");
}

// Display File Size
void fileSize()
{
    char filename[256];
    struct stat fileInfo;

    getFilename(filename);

    if (stat(filename, &fileInfo) == 0)
        printf("File Size: %lld bytes\n",
               (long long)fileInfo.st_size);
    else
        perror("Error getting file size");
}

// Change File Permissions
void changePermissions()
{
    char filename[256];
    int choice;
    mode_t permissions;

    getFilename(filename);

    printf("\n1. Read and Write (644)");
    printf("\n2. Owner Only (600)");
    printf("\n3. Full Access for Owner (755)");
    printf("\nEnter choice: ");

    if (scanf("%d", &choice) != 1)
    {
        printf("Invalid input.\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return;
    }

    while (getchar() != '\n');

    switch (choice)
    {
        case 1: permissions = 0644; break;
        case 2: permissions = 0600; break;
        case 3: permissions = 0755; break;

        default:
            printf("Invalid permission choice.\n");
            return;
    }

    if (chmod(filename, permissions) == 0)
        printf("Permissions changed successfully.\n");
    else
        perror("Error changing permissions");
}

// Main Function
int main()
{
    int choice;

    while (1)
    {
        printf("\n===== FILE MANAGEMENT SYSTEM =====\n");
        printf("1. Create File\n");
        printf("2. Write to File\n");
        printf("3. Read File\n");
        printf("4. Append to File\n");
        printf("5. Copy File\n");
        printf("6. Rename File\n");
        printf("7. Delete File\n");
        printf("8. Display File Size\n");
        printf("9. Change File Permissions\n");
        printf("10. Exit\n");

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Enter a number.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        while (getchar() != '\n');

        switch (choice)
        {
            case 1: createFile(); break;
            case 2: writeFile(); break;
            case 3: readFile(); break;
            case 4: appendFile(); break;
            case 5: copyFile(); break;
            case 6: renameFile(); break;
            case 7: deleteFile(); break;
            case 8: fileSize(); break;
            case 9: changePermissions(); break;

            case 10:
                printf("Exiting File Management System.\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}