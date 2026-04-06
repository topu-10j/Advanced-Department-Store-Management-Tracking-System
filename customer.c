// customer management system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

struct Customer
{
    int id;
    char name[100];
    char phone[15];
    char email[50];
    char address[200];
    int points;
    char membership[20];
    float totalPurchase;
    char joinDate[20];
    int totalVisits;
};
struct PurchaseHistory
{
    int customerId;
    int billNo;
    float amount;
    char date[20];
};

struct CustomerBill
{
    int customerId;
    int billNo;
};

struct Customer customers[500];
int customerCount = 0;

struct PurchaseHistory purchases[2000];
int purchaseCount = 0;

struct CustomerBill customerBills[2000];
int customerBillCount = 0;

void registerCustomer();
void viewAllCustomers();
void updateLoyaltyPoints();
void viewPurchaseHistory();
void viewVIPCustomers();
void addPurchaseHistory(int customerId, int billNo, float amount);
void updateMembership(int customerId);

// add purchase history
void addPurchaseHistory(int customerId, int billNo, float amount)
{
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    strftime(purchases[purchaseCount].date, 20, "%d-%m-%Y", tm_info);

    purchases[purchaseCount].customerId = customerId;
    purchases[purchaseCount].billNo = billNo;
    purchases[purchaseCount].amount = amount;
    purchaseCount++;
}

// update membership
void updateMembership(int customerId)
{
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].id == customerId)
        {
            if (customers[i].points >= 10000)
            {
                strcpy(customers[i].membership, "Platinum");
            }
            else if (customers[i].points >= 5000)
            {
                strcpy(customers[i].membership, "Gold");
            }
            else if (customers[i].points >= 1000)
            {
                strcpy(customers[i].membership, "Silver");
            }
            else
            {
                strcpy(customers[i].membership, "Bronze");
            }
            break;
        }
    }
}

// register customer
void registerCustomer()
{
    clearScreen();

    printf("\n\t           CUSTOMER REGISTRATION");
    printf("\n\t--------------------------------------\n");

    if (customerCount >= 500)
    {
        printf("\n\t Cannot add more customers! Limit reached.");
        return;
    }

    customers[customerCount].id = customerCount + 1001;

    printf("\n\t Customer ID will be: %d", customers[customerCount].id);

    printf("\n\t Enter Name: ");
    scanf("%s", customers[customerCount].name);

    printf("\t Enter Phone: ");
    scanf("%s", customers[customerCount].phone);

    printf("\t Enter Email: ");
    scanf("%s", customers[customerCount].email);

    printf("\t Enter Address: ");
    scanf("%s", customers[customerCount].address);

    customers[customerCount].points = 0;
    customers[customerCount].totalPurchase = 0;
    customers[customerCount].totalVisits = 0;
    strcpy(customers[customerCount].membership, "Bronze");

    // Get current date
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    strftime(customers[customerCount].joinDate, 20, "%d-%m-%Y", tm_info);

    customerCount++;

    printf("\n\t Customer registered successfully!");
    printf("\n\t Customer ID: %d", customers[customerCount - 1].id);
    printf("\n\t Membership: Bronze (0 points)");
    printf("\n\t Earn 1 point for every ৳100 spent!");

    auto_Record("New customer registered");
}

// view all customers
void viewAllCustomers()
{
    clearScreen();

    printf("\n\t           ALL CUSTOMERS");
    printf("\n\t--------------------------------------\n");

    if (customerCount == 0)
    {
        printf("\n\t No customers found!");
        return;
    }

    printf("\n\t %-8s %-15s %-12s %-10s %-15s %-10s",
           "ID", "Name", "Phone", "Points", "Membership", "Total");
    printf("\n\t------------------------------------------------------------");

    for (int i = 0; i < customerCount; i++)
    {
        printf("\n\t %-8d %-15s %-12s %-10d %-15s %-10.2f",
               customers[i].id,
               customers[i].name,
               customers[i].phone,
               customers[i].points,
               customers[i].membership,
               customers[i].totalPurchase);
    }

    printf("\n\t------------------------------------------------------------");
    printf("\n\t Total Customers: %d", customerCount);

    auto_Record("Viewed all customers");
}

// update loyalty points
void updateLoyaltyPoints()
{
    int custId, billNo, found = 0;
    float billAmount = 0;

    clearScreen();

    printf("\n\t           LOYALTY POINTS SYSTEM");
    printf("\n\t--------------------------------------\n");

    if (customerCount == 0)
    {
        printf("\n\t No customers found!");
        printf("\n\t Please register customers first.");
        return;
    }

    printf("\n\t Enter Customer ID: ");
    scanf("%d", &custId);

    // Find customer
    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].id == custId)
        {
            found = 1;

            printf("\n\t Customer: %s", customers[i].name);
            printf("\n\t Current Points: %d", customers[i].points);
            printf("\n\t Current Membership: %s", customers[i].membership);

            printf("\n\n\t Enter Bill Amount: ");
            scanf("%f", &billAmount);

            int pointsEarned = (int)(billAmount / 100);
            customers[i].points += pointsEarned;
            customers[i].totalPurchase += billAmount;
            customers[i].totalVisits++;

            // Update membership
            updateMembership(custId);

            printf("\n\t Points Updated!");
            printf("\n\t Points Earned: +%d", pointsEarned);
            printf("\n\t Total Points: %d", customers[i].points);
            printf("\n\t Membership: %s", customers[i].membership);

            printf("\n\n\t Membership Benefits:");
            if (strcmp(customers[i].membership, "Bronze") == 0)
            {
                printf("\n\t • 0%% discount");
                printf("\n\t • Need 1000 points for Silver");
            }
            else if (strcmp(customers[i].membership, "Silver") == 0)
            {
                printf("\n\t • 5%% discount on all purchases");
                printf("\n\t • Need 5000 points for Gold");
            }
            else if (strcmp(customers[i].membership, "Gold") == 0)
            {
                printf("\n\t • 10%% discount on all purchases");
                printf("\n\t • Need 10000 points for Platinum");
            }
            else if (strcmp(customers[i].membership, "Platinum") == 0)
            {
                printf("\n\t • 15%% discount on all purchases");
                printf("\n\t • Free delivery");
                printf("\n\t • Birthday gift");
            }

            auto_Record("Loyalty points updated");
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Customer not found!");
    }
}

// view purchase history
void viewPurchaseHistory()
{
    int custId, found = 0;
    int hasPurchases = 0;

    clearScreen();

    printf("\n\t           PURCHASE HISTORY");
    printf("\n\t--------------------------------------\n");

    if (customerCount == 0)
    {
        printf("\n\t No customers found!");
        return;
    }

    printf("\n\t Enter Customer ID: ");
    scanf("%d", &custId);

    for (int i = 0; i < customerCount; i++)
    {
        if (customers[i].id == custId)
        {
            found = 1;

            printf("\n\t Customer: %s", customers[i].name);
            printf("\n\t Phone: %s", customers[i].phone);
            printf("\n\t Membership: %s", customers[i].membership);
            printf("\n\t Total Points: %d", customers[i].points);
            printf("\n\t Total Visits: %d", customers[i].totalVisits);
            printf("\n\t Total Spent: %.2f", customers[i].totalPurchase);

            printf("\n\n\t Purchase History:");
            printf("\n\t %-8s %-12s %-10s",
                   "Bill No", "Date", "Amount");
            printf("\n\t-----------------------------");

            for (int j = 0; j < purchaseCount; j++)
            {
                if (purchases[j].customerId == custId)
                {
                    hasPurchases = 1;
                    printf("\n\t %-8d %-12s %-10.2f",
                           purchases[j].billNo,
                           purchases[j].date,
                           purchases[j].amount);
                }
            }

            if (!hasPurchases)
            {
                printf("\n\t No purchase history found!");
            }
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Customer not found!");
    }

    auto_Record("Viewed purchase history");
}

// view VIP customers
void viewVIPCustomers()
{
    clearScreen();

    printf("\n\t           VIP CUSTOMER LIST");
    printf("\n\t--------------------------------------\n");

    if (customerCount == 0)
    {
        printf("\n\t No customers found!");
        return;
    }

    int vipCount = 0;

    printf("\n\t VIP Customers (Gold & Platinum):");
    printf("\n\t %-8s %-20s %-12s %-15s %-10s",
           "ID", "Name", "Phone", "Membership", "Points");
    printf("\n\t--------------------------------------------------------------------");

    for (int i = 0; i < customerCount; i++)
    {
        if (strcmp(customers[i].membership, "Gold") == 0 ||
            strcmp(customers[i].membership, "Platinum") == 0)
        {
            vipCount++;
            printf("\n\t %-8d %-20s %-12s %-15s %-10d",
                   customers[i].id,
                   customers[i].name,
                   customers[i].phone,
                   customers[i].membership,
                   customers[i].points);
        }
    }

    if (vipCount == 0)
    {
        printf("\n\t No VIP customers yet!");
        printf("\n\t Customers need 5000+ points to become VIP.");
    }
    else
    {
        printf("\n\t----------------------------------------------------------------");
        printf("\n\t Total VIP Customers: %d", vipCount);
    }

    auto_Record("Viewed VIP customers");
}

void customer_Management()
{
    int subchoice;

    do
    {
        clearScreen();
        printf("\n\t      CUSTOMER MANAGEMENT");
        printf("\n\t----------------------------------------");
        printf("\n\t 1. Register Customer");
        printf("\n\t 2. View All Customers");
        printf("\n\t 3. Update Loyalty Points");
        printf("\n\t 4. View Purchase History");
        printf("\n\t 5. View VIP Customers");
        printf("\n\t 0. Back to Main Menu");
        printf("\n\t----------------------------------------");
        printf("\n\t Total Customers: %d", customerCount);
        printf("\n\t VIP Customers: ");

        // Count VIP customers
        int vipCount = 0;
        for (int i = 0; i < customerCount; i++)
        {
            if (strcmp(customers[i].membership, "Gold") == 0 ||
                strcmp(customers[i].membership, "Platinum") == 0)
            {
                vipCount++;
            }
        }
        printf("%d", vipCount);

        printf("\n\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        switch (subchoice)
        {
        case 1:
            registerCustomer();
            break;
        case 2:
            viewAllCustomers();
            break;
        case 3:
            updateLoyaltyPoints();
            break;
        case 4:
            viewPurchaseHistory();
            break;
        case 5:
            viewVIPCustomers();
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
