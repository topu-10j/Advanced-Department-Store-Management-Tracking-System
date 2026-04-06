// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void clearScreen();
void auto_Record(char *action);
void sms_Alert(char *type, char *message);
void pressEnterToContinue();
void printHeader(char *title);
void printSuccess(char *message);
void printError(char *message);
void printWarning(char *message);

#endif