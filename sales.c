// sales and billing system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "shared.h"

struct BillItem billItems[5000];
int billItemCount = 0;

struct Queue
{
    int billNos[100];
    int front;
    int rear;
    int size;
};

struct Stack
{
    int billNos[100];
    int top;
};

extern struct Product products[1000];
extern int productCount;

struct Bill bills[1000];
int billCount = 0;
int nextBillNo = 1001;

struct Queue billingQueue = {-1, -1, 0};
struct Stack returnStack = {-1};

void generateBill();
void viewAllBills();
void processReturn();
void dailySalesReport();
void addToQueue(int billNo);
int removeFromQueue();
void pushToStack(int billNo);
int popFromStack();

// queue
void addToQueue(int billNo)
{
    if (billingQueue.size >= 100)
    {
        printf("\n\t Queue is full!");
        return;
    }

    if (billingQueue.front == -1)
    {
        billingQueue.front = 0;
    }

    billingQueue.rear++;
    billingQueue.billNos[billingQueue.rear] = billNo;
    billingQueue.size++;
}

int removeFromQueue()
{
    if (billingQueue.size == 0)
    {
        return -1;
    }

    int billNo = billingQueue.billNos[billingQueue.front];
    billingQueue.front++;
    billingQueue.size--;

    if (billingQueue.size == 0)
    {
        billingQueue.front = -1;
        billingQueue.rear = -1;
    }

    return billNo;
}

// stack
void pushToStack(int billNo)
{
    if (returnStack.top >= 99)
    {
        printf("\n\t Return stack is full!");
        return;
    }

    returnStack.top++;
    returnStack.billNos[returnStack.top] = billNo;
}

int popFromStack()
{
    if (returnStack.top == -1)
    {
        return -1;
    }

    int billNo = returnStack.billNos[returnStack.top];
    returnStack.top--;
    return billNo;
}

// new bill
void generateBill()
{
    clearScreen();

    printf("\n\t           GENERATE NEW BILL");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        printf("\n\t Please add products first.");
        return;
    }

    printf("\n\t Enter Customer Name: ");
    scanf("%s", bills[billCount].customerName);

    bills[billCount].billNo = nextBillNo++;
    bills[billCount].items = 0;
    bills[billCount].amount = 0;

    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    strftime(bills[billCount].date, 20, "%d-%m-%Y", tm_info);

    printf("\n\t -----------ADD ITEMS ----------");

    int continueAdding = 1;
    while (continueAdding)
    {
        int productId, quantity, found = 0;

        printf("\n\t Enter Product ID: ");
        scanf("%d", &productId);

        // Search product
        for (int i = 0; i < productCount; i++)
        {
            if (products[i].id == productId)
            {
                found = 1;

                printf("\t Product: %s", products[i].name);
                printf("\n\t Price: %.2f", products[i].price);
                printf("\n\t Available Stock: %d", products[i].stock);
                printf("\n\t Enter Quantity: ");
                scanf("%d", &quantity);

                if (quantity > products[i].stock)
                {
                    printf("\n\t Insufficient stock! Only %d available.", products[i].stock);
                }
                else
                {
                    float itemTotal = products[i].price * quantity;
                    bills[billCount].amount += itemTotal;
                    bills[billCount].items++;
                    products[i].stock -= quantity;

                    printf("\n\t Added: %d x %s = ৳%.2f", quantity, products[i].name, itemTotal);

                    // Low stock alert
                    if (products[i].stock < 10)
                    {
                        printf("\n\t Warning: Low stock! Only %d left.", products[i].stock);
                    }
                }
                break;
            }
        }

        if (!found)
        {
            printf("\n\t Product not found!");
        }

        printf("\n\n\t Add more items? (1=Yes, 0=No): ");
        scanf("%d", &continueAdding);
    }

    // Payment method
    printf("\n\t Payment Method (Cash/Card/Mobile): ");
    scanf("%s", bills[billCount].paymentMethod);

    // Show bill summary
    printf("\n\t -----------------------------------");
    printf("\n\t           BILL SUMMARY");
    printf("\n\t -----------------------------------");
    printf("\n\t Bill No: %d", bills[billCount].billNo);
    printf("\n\t Customer: %s", bills[billCount].customerName);
    printf("\n\t Date: %s", bills[billCount].date);
    printf("\n\t Total Items: %d", bills[billCount].items);
    printf("\n\t Total Amount: %.2f", bills[billCount].amount);
    printf("\n\t Payment: %s", bills[billCount].paymentMethod);
    printf("\n\t -----------------------------------");

    // Add to billing queue
    addToQueue(bills[billCount].billNo);

    billCount++;

    printf("\n\n\t Bill generated successfully!");
    printf("\n\t Customers in queue: %d", billingQueue.size);
    auto_Record("New bill generated");
}

// ========== VIEW ALL BILLS ==========
void viewAllBills()
{
    clearScreen();

    printf("\n\t           ALL BILLS/INVOICES");
    printf("\n\t--------------------------------------\n");

    if (billCount == 0)
    {
        printf("\n\t No bills found!");
        return;
    }

    printf("\n\t %-8s %-15s %-12s %-10s %-10s %-8s",
           "Bill No", "Customer", "Date", "Items", "Amount", "Payment");
    printf("\n\t-------------------------------------------------------------- ");

    float totalRevenue = 0;
    for (int i = 0; i < billCount; i++)
    {
        printf("\n\t %-8d %-15s %-12s %-10d %-10.2f %-8s",
               bills[i].billNo,
               bills[i].customerName,
               bills[i].date,
               bills[i].items,
               bills[i].amount,
               bills[i].paymentMethod);
        totalRevenue += bills[i].amount;
    }

    printf("\n\t-------------------------------------------------------------- ");
    printf("\n\t Total Bills: %d", billCount);
    printf("\n\t Total Revenue: %.2f", totalRevenue);

    // Queue status
    printf("\n\n\t Billing Queue Status:");
    printf("\n\t Customers waiting: %d", billingQueue.size);

    auto_Record("Viewed all bills");
}

// ========== PROCESS RETURN ==========
void processReturn()
{
    clearScreen();

    printf("\n\t           PROCESS RETURN");
    printf("\n\t--------------------------------------\n");

    if (billCount == 0)
    {
        printf("\n\t No bills found!");
        return;
    }

    int billNo;
    printf("\n\t Enter Bill Number to return: ");
    scanf("%d", &billNo);

    int found = 0;
    for (int i = 0; i < billCount; i++)
    {
        if (bills[i].billNo == billNo)
        {
            found = 1;

            printf("\n\t Bill Found:");
            printf("\n\t Customer: %s", bills[i].customerName);
            printf("\n\t Date: %s", bills[i].date);
            printf("\n\t Amount: %.2f", bills[i].amount);

            printf("\n\n\t Refund Amount: ৳%.2f", bills[i].amount);
            printf("\n\t Confirm return? (1=Yes, 0=No): ");
            int confirm;
            scanf("%d", &confirm);

            if (confirm == 1)
            {
                // Add to return stack
                pushToStack(billNo);
                printf("\n\t Return processed successfully!");
                printf("\n\t Refund amount: %.2f", bills[i].amount);
                auto_Record("Return processed");
            }
            else
            {
                printf("\n\t Return cancelled!");
            }
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Bill not found!");
    }

    // Show return stack
    printf("\n\n\t Recent Returns:");
    if (returnStack.top == -1)
    {
        printf("\n\t No returns yet.");
    }
    else
    {
        printf("\n\t Bill numbers in return stack: ");
        for (int i = 0; i <= returnStack.top; i++)
        {
            printf("%d ", returnStack.billNos[i]);
        }
    }
}

// ========== DAILY SALES REPORT ==========
void dailySalesReport()
{
    clearScreen();

    printf("\n\t           DAILY SALES REPORT");
    printf("\n\t--------------------------------------\n");

    // Get current date
    time_t t;
    struct tm *tm_info;
    char today[20];
    time(&t);
    tm_info = localtime(&t);
    strftime(today, 20, "%d-%m-%Y", tm_info);

    printf("\n\t Date: %s", today);
    printf("\n\t--------------------------------------\n");

    int dailyBillCount = 0;
    float dailyRevenue = 0;
    int cashCount = 0, cardCount = 0, mobileCount = 0;
    float cashAmount = 0, cardAmount = 0, mobileAmount = 0;

    for (int i = 0; i < billCount; i++)
    {
        if (strcmp(bills[i].date, today) == 0)
        {
            dailyBillCount++;
            dailyRevenue += bills[i].amount;

            if (strcmp(bills[i].paymentMethod, "Cash") == 0)
            {
                cashCount++;
                cashAmount += bills[i].amount;
            }
            else if (strcmp(bills[i].paymentMethod, "Card") == 0)
            {
                cardCount++;
                cardAmount += bills[i].amount;
            }
            else if (strcmp(bills[i].paymentMethod, "Mobile") == 0)
            {
                mobileCount++;
                mobileAmount += bills[i].amount;
            }
        }
    }

    if (dailyBillCount == 0)
    {
        printf("\n\t No sales recorded today!");
    }
    else
    {
        printf("\n\t Total Bills: %d", dailyBillCount);
        printf("\n\t Total Revenue: %.2f", dailyRevenue);
        printf("\n\t Average Bill: %.2f", dailyRevenue / dailyBillCount);

        printf("\n\n\t Payment Method Breakdown:");
        printf("\n\t --------------------------------------");
        printf("\n\t Cash   : %d bills (৳%.2f)", cashCount, cashAmount);
        printf("\n\t Card   : %d bills (৳%.2f)", cardCount, cardAmount);
        printf("\n\t Mobile : %d bills (৳%.2f)", mobileCount, mobileAmount);

        // Top selling products today
        printf("\n\n\t Today's Bills:");
        printf("\n\t %-8s %-15s %-10s",
               "Bill No", "Customer", "Amount");
        printf("\n\t--------------------------------------");

        for (int i = 0; i < billCount; i++)
        {
            if (strcmp(bills[i].date, today) == 0)
            {
                printf("\n\t %-8d %-15s %-10.2f",
                       bills[i].billNo,
                       bills[i].customerName,
                       bills[i].amount);
            }
        }
    }

    // Queue status
    printf("\n\n\t Current Billing Queue: %d customers waiting", billingQueue.size);

    auto_Record("Generated daily sales report");
}

// ========== SALES & BILLING MAIN MENU ==========
void sales_Billing()
{
    int subchoice;

    do
    {
        clearScreen();
        printf("\n\t--------------------------------------");
        printf("\n\t         SALES & BILLING");
        printf("\n\t--------------------------------------");
        printf("\n\t 1. Generate New Bill");
        printf("\n\t 2. View All Bills/Invoices");
        printf("\n\t 3. Process Return/Exchange");
        printf("\n\t 4. Daily Sales Report");
        printf("\n\t 0. Back to Main Menu");
        printf("\n\t--------------------------------------");
        printf("\n\t Total Bills: %d", billCount);
        printf("\n\t Queue Size: %d", billingQueue.size);
        printf("\n\t Returns Pending: %d", returnStack.top + 1);
        printf("\n\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        switch (subchoice)
        {
        case 1:
            generateBill();
            break;
        case 2:
            viewAllBills();
            break;
        case 3:
            processReturn();
            break;
        case 4:
            dailySalesReport();
            break;
        case 0:
            printf("\n\t Returning to Main Menu...");
            break;
        default:
            printf("\n\t Invalid choice!");
        }

        if (subchoice != 0)
        {
            printf("\n\n\t Press Enter to continue...");
            getchar();
            getchar();
        }
    } while (subchoice != 0);
}