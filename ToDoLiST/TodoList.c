#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sodium.h>
#define SALT_ROUNDS 12
typedef struct
{
    char *description;
    bool isCompleted;
} Task;

typedef struct
{
    Task *tasks;
    int index;
} AllTasks;

typedef struct
{
    char *username;
    char *password;
} User;

void InitUser(User *user, char *password, char *username)
{
    user->username = malloc(sizeof(char) * strlen(username));
    user->password = malloc(sizeof(char) * strlen(password));
    strcpy(user->username, username);
    strcpy(user->password, password);
}
void InitTask(Task *task, bool isCompleted, char *description)
{
    task->description = malloc(sizeof(char) * strlen(description) + 1);
    strcpy(task->description, description);
    task->isCompleted = isCompleted;
}

void InitAllTasks(AllTasks *allTasks)
{
    allTasks->index = 0;
    allTasks->tasks = (Task *)(malloc(sizeof(NULL)));
}

void AddTask(AllTasks *alltasks, Task *task)
{
    alltasks->index = alltasks->index + 1;
    alltasks->tasks = realloc(alltasks->tasks, alltasks->index * sizeof(Task));
    alltasks->tasks[alltasks->index - 1] = *task;
}

void CompleteTask(AllTasks *allTasks, int indexOfTask)
{
    if (allTasks->index < indexOfTask)
    {
        printf("Invalid index, returning...");
        return;
    }
    Task *ptr = allTasks->tasks;
    ptr[indexOfTask - 1].isCompleted = true;
}

void UpdateTask(char *description, Task *task)
{
    task->description = realloc(task->description, sizeof(char) * strlen(description) + 1);
    strcpy(task->description, description);
}
void DeleteTask(int index, AllTasks *allTasks)
{
    free(allTasks->tasks[index - 1].description);
    for (int i = index - 1; i < allTasks->index - 1; i++)
        allTasks->tasks[i] = allTasks->tasks[i + 1];
    allTasks->tasks = realloc(allTasks->tasks, sizeof(Task) * (allTasks->index - 1));
    allTasks->index--;
}

void HashPassword(char *originalPassword, char *hash)
{

    if (sodium_init() < 0)
    {
        printf("Libsodium initialization failed!\n");
        return;
    }
    if (crypto_pwhash_str(hash, "mypassword", 15, crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE) != 0)
    {
        printf("Hashing failed\n");
        return;
    }
    printf("Hashed password: %s\n", hash);
}

void RegisterUser(char *username, char *password)
{
    FILE *ftpr;
    char filename[200]; // Increased size to accommodate full path
    const char *directory = "D:\\C\\C Practice questions\\MiniProjects\\ToDoList\\UserInformation\\";
    struct stat st = {0};
    snprintf(filename, sizeof(filename), "%s%s.txt", directory, username);
    ftpr = fopen64(filename, "w");
    if (ftpr == NULL)
    {
        perror("Error opening file for writing");
        return;
    }
    char hash[crypto_pwhash_STRBYTES];
    HashPassword(password, hash);
    fprintf(ftpr, "Username: %s\nPassword: %s\n", username, hash);
    fclose(ftpr);
    printf("User registered successfully, file created: %s\n", filename);
}
int main()
{
    int menuState = 0;
    AllTasks allTasks;
    InitAllTasks(&allTasks);
    printf("Welcome to Todo List App\n");
    printf("Enter your operation of choice\n");
    printf("10-Register");
    printf("1-Add a task\n");
    printf("2-Complete a task\n");
    printf("3-Edit a task\n");
    printf("4-Delete a task\n");
    printf("5-View all tasks\n");
    printf("6-Exit\n");
    while (true)
    {
        if (scanf("%d", &menuState) > 10)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (menuState == 1)
        {
            printf("Enter the task name:\n");
            char description[100];
            scanf(" %[^\n]", description);

            if (strlen(description) > 100)
            {
                printf("Task description is too long.\n");
                continue;
            }

            bool isCompleted = false;
            Task *task = (Task *)malloc(sizeof(Task));
            if (task == NULL)
            {
                printf("Memory allocation failed.\n");
                continue;
            }

            InitTask(task, isCompleted, strdup(description));
            AddTask(&allTasks, task);
            free(task);
        }
        if (menuState == 2)
        {
            printf("What task number do you want to complete?");
            int index = 01;
            scanf("%d", &index);
            CompleteTask(&allTasks, index);
        }
        if (menuState == 3)
        {
            printf("What task do you want to edit?\n");
            int index = 0;
            scanf("%d", &index);
            if (index > allTasks.index)
            {
                printf("Error, invalid index");
                continue;
            }
            char description[100];
            scanf(" %[^\n]", description);
            UpdateTask(strdup(description), &allTasks.tasks[index - 1]);
        }
        if (menuState == 4)
        {
            printf("What task do you want to delete\n");
            int index = 0;
            scanf("%d", &index);
            if (index > allTasks.index)
            {
                printf("Error, invalid index");
                continue;
            }
            DeleteTask(index, &allTasks);
        }
        if (menuState == 5)
        {
            printf("Entered View\n");
            Task *ptr = allTasks.tasks;
            for (int i = 0; i < allTasks.index; i++)
            {
                printf("Task %d", i + 1);
                printf(" - %s", ptr[i].description);
                if (ptr[i].isCompleted)
                    printf(", status - Completed\n");
                else
                    printf(", status - Not completed yet\n");
            }
        }
        else if (menuState == 6)
        {
            printf("Exiting the Todo List App. Goodbye!\n");
            break;
        }
        else if (menuState == 10)
        {
            char username[100];
            char password[100];
            printf("Enter a username");
            scanf("%s", &username);
            printf("Enter a password");
            scanf("%s", password);
            RegisterUser(username, password);
        }
        else if (menuState > 10)
        {
            printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}
