// shared.h
#ifndef SHARED_H
#define SHARED_H

#define MAX_PRODUCTS 1000

struct Product
{
    int id;
    char name[100];
    char category[50];
    float price;
    int stock;
    char expiryDate[20];
};

extern struct Product products[MAX_PRODUCTS];
extern int productCount;

#endif