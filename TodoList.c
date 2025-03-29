#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
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

void InitTask(Task *task, bool isCompleted, char *description)
{
    task->description = description;
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
    if (allTasks->index 1 < indexOfTask)
    {
        printf("Invalid index, returning...");
        return;
    }
    Task *ptr = allTasks->tasks;
    ptr[indexOfTask - 1].isCompleted = true;
}
int main()
{
    int menuState = 0;
    AllTasks allTasks;
    InitAllTasks(&allTasks);
    printf("Program started successfully.\n");
    while (true)
    {
        printf("Welcome to Todo List App\n");
        printf("Enter your operation of choice\n");
        printf("1-Add a task\n");
        printf("2-Complete a task\n");
        printf("3-Delete a task\n");
        printf("4-View all tasks\n");
        printf("5-Exit\n");

        if (scanf("%d", &menuState) != 1)
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
        if (menuState == 4)
        {
            printf("Entered View\n");
            Task *ptr = allTasks.tasks;
            for (int i = 0; i < allTasks.index; i++)
            {
                printf("Task %d", i);
                printf(" - %s", ptr[i].description);
                if (ptr[i].isCompleted)
                    printf(", status - Completed\n");
                else
                    printf(", status - Not completed yet\n");
            }
        }
        else if (menuState == 5)
        {
            printf("Exiting the Todo List App. Goodbye!\n");
            break;
        }
        else
        {
            printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}
