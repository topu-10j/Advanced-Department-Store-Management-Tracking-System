// inventory management system
#include <time.h>

struct product
{
    int id;
    char name[100];
    char category[50];
    float price;
    int stock;
    char expiryDate[20];
};

extern struct product products[1000];
extern int productCount;

void clearScreen();
void sms_Alert(char *type, char *message);
void auto_Record(char *action);

void currentstockstatus();
void lowstockalert();
void expirydatetracking();
void reordermanagement();

// current stocks
void currentstockstatus()
{
    clearScreen();

    printf("\n\t           CURRENT STOCK STATUS");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        return;
    }
    printf("\n\t %-5s %-20s %-15s %-10s %-8s",
           "ID", "Name", "Category", "Stock", "Status");
    printf("\n\t-------------------------------------------------");

    int totalstock = 0;
    int lowstockcount = 0;

    for (int i = 0; i < productCount; i++)
    {
        totalstock += products[i].stock;
        // stock status
        char status[20];
        if (products[i].stock == 0)
        {
            strcpy(status, "OUT OF STOCK");
        }
        else if (products[i].stock < 10)
        {
            strcpy(status, "LOW STOCK");
            lowstockcount++;
        }
        else
        {
            strcpy(status, "IN STOCK");
        }

        printf("\n\t %-5d %-20s %-15s %-10d %-8s",
               products[i].id,
               products[i].name,
               products[i].category,
               products[i].stock,
               status);
    }
    printf("\n\t Total Products: %d", productCount);
    printf("\n\t Total Stock: %d units", totalstock);
    printf("\n\t Low Stock Items: %d", lowstockcount);

    auto_Record("Viewed current stock status");
}

// low stock alert
void lowstockalert()
{
    clearScreen();

    printf("\n\t           LOW STOCK ALERT");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        return;
    }

    int threshold = 10;
    int found = 0;

    printf("\n\t Products with stock below %d:", threshold);
    printf("\n\t %-5s %-20s %-10s %-12s",
           "ID", "Name", "Stock", "Status");
    printf("\n\t-------------------------------------");

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].stock < threshold)
        {
            found = 1;
            char status[20];
            if (products[i].stock == 0)
            {
                strcpy(status, "CRITICAL!");
            }
            else
            {
                strcpy(status, "NEED REORDER");
            }

            printf("\n\t %-5d %-20s %-10d %-12s",
                   products[i].id,
                   products[i].name,
                   products[i].stock,
                   status);

            // sms alert
            if (products[i].stock == 0)
            {
                char msg[200];
                sprintf(msg, "URGENT: %s is out of stock!", products[i].name);
                sms_Alert("Manager", msg);
            }
        }
    }

    if (!found)
    {
        printf("\n\t All products have sufficient stock!");
    }

    auto_Record("Checked low stock alert");
}

// expiry date tracking
void expirydatetracking()
{
    clearScreen();

    printf("\n\t           EXPIRY DATE TRACKING");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        return;
    }

    // current date
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    int currentDay = tm_info->tm_mday;
    int currentMonth = tm_info->tm_mon + 1;
    int currentYear = tm_info->tm_year + 1900;

    int found = 0;
    int expiringSoon = 0;

    printf("\n\t %-5s %-20s %-12s %-15s",
           "ID", "Name", "Expiry Date", "Status");
    printf("\n\t--------------------------------------------");

    for (int i = 0; i < productCount; i++)
    {
        int day, month, year;
        sscanf(products[i].expiryDate, "%d-%d-%d", &day, &month, &year);

        // check
        if (year < currentYear ||
            (year == currentYear && month < currentMonth) ||
            (year == currentYear && month == currentMonth && day < currentDay))
        {
            found = 1;
            printf("\n\t %-5d %-20s %-12s %-15s",
                   products[i].id,
                   products[i].name,
                   products[i].expiryDate,
                   "EXPIRED");

            char msg[200];
            sprintf(msg, "ALERT: %s has expired!", products[i].name);
            sms_Alert("Manager", msg);
        }
        // within 30
        else if (year == currentYear && month == currentMonth && (day - currentDay) <= 30)
        {
            found = 1;
            expiringSoon++;
            printf("\n\t %-5d %-20s %-12s %-15s",
                   products[i].id,
                   products[i].name,
                   products[i].expiryDate,
                   "EXPIRING SOON");
        }
        // in 7
        else if (year == currentYear && month == currentMonth && (day - currentDay) <= 7)
        {
            found = 1;
            expiringSoon++;
            printf("\n\t %-5d %-20s %-12s %-15s",
                   products[i].id,
                   products[i].name,
                   products[i].expiryDate,
                   "CRITICAL!");

            char msg[200];
            sprintf(msg, "WARNING: %s expires in %d days!", products[i].name, day - currentDay);
            sms_Alert("Manager", msg);
        }
    }

    if (!found)
    {
        printf("\n\t No expired or expiring products found!");
    }
    else
    {
        printf("\n\t---------------------------------------------");
        printf("\n\t Total expiring soon: %d", expiringSoon);
    }

    auto_Record("Checked expiry tracking");
}

// reorder management
void reordermanagement()
{
    clearScreen();

    printf("\n\t           REORDER MANAGEMENT");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        return;
    }

    int threshold = 15;
    int found = 0;

    printf("\n\t Products that need reorder (Stock < %d):", threshold);
    printf("\n\t %-5s %-20s %-10s %-12s",
           "ID", "Name", "Current Stock", "Recommended Qty");
    printf("\n\t--------------------------------------------");

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].stock < threshold)
        {
            found = 1;
            int recommended = threshold - products[i].stock + 20; // Reorder to 30
            if (recommended < 20)
                recommended = 20;

            printf("\n\t %-5d %-20s %-10d %-12d",
                   products[i].id,
                   products[i].name,
                   products[i].stock,
                   recommended);
        }
    }

    if (!found)
    {
        printf("\n\t No products need reorder at this time!");
    }
    else
    {
        printf("\n\t--------------------------------------------------------");
        printf("\n\t Tip: Reorder recommended quantities to avoid stockouts");
    }

    auto_Record("Checked reorder management");
}

void inventory_Management()
{
    int subchoice;

    do
    {
        clearScreen();
        printf("\n\t      INVENTORY MANAGEMENT");
        printf("\n\t----------------------------------------");
        printf("\n\t 1. Current Stock Status");
        printf("\n\t 2. Low Stock Alert");
        printf("\n\t 3. Expiry Date Tracking");
        printf("\n\t 4. Reorder Management");
        printf("\n\t 0. Back to Main Menu");
        printf("\n\t----------------------------------------");
        printf("\n\t Total Products: %d", productCount);
        printf("\n\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        switch (subchoice)
        {
        case 1:
            currentstockstatus();
            break;
        case 2:
            lowstockalert();
            break;
        case 3:
            expirydatetracking();
            break;
        case 4:
            reordermanagement();
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
