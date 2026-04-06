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

struct Bill
{
    int billNo;
    int customerId;
    char customerName[100];
    char date[20];
    float amount;
    int items;
    char paymentMethod[20];
};

struct BillItem
{
    int billNo;
    int productId;
    char productName[100];
    int quantity;
    float price;
    float total;
};

extern struct Product products[MAX_PRODUCTS];
extern int productCount;

extern struct Bill bills[1000];
extern int billCount;

extern struct BillItem billItems[5000];
extern int billItemCount;

#endif