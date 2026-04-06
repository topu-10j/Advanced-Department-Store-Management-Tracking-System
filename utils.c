// utils.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void auto_Record(char *action)
{
    FILE *log = fopen("data/system_auto.log", "a");
    if (log == NULL)
    {
#ifdef _WIN32
        system("mkdir data");
#else
        system("mkdir -p data");
#endif
        log = fopen("data/system_auto.log", "a");
        if (log == NULL)
            return;
    }

    time_t t;
    time(&t);
    fprintf(log, "[%s] %s\n", ctime(&t), action);
    fclose(log);
}

void sms_Alert(char *type, char *message)
{
    printf("\n\t [SMS to %s]: %s", type, message);
    auto_Record("SMS Alert sent");
}

void pressEnterToContinue()
{
    printf("\n\t Press Enter to continue...");
    getchar();
    getchar();
}

void printHeader(char *title)
{
    printf("\n\t--------------------------------------");
    printf("\n\t           %s", title);
    printf("\n\t--------------------------------------\n");
}

void printSuccess(char *message)
{
    printf("\n\t [OK] %s", message);
}

void printError(char *message)
{
    printf("\n\t [ERROR] %s", message);
}

void printWarning(char *message)
{
    printf("\n\t [WARNING] %s", message);
}