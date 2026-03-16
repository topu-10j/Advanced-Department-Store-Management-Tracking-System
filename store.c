#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// main menu
void showMain_Menu();
void product_Management();
void inventory_Management();
void employee_Management();
void sales_Billing();
void customer_Management();
void reports_Analytics();
void store_Navigation();
void settings();
void sms_Alert(char *type, char *message);
void auto_Record(char *action);

int main()
{
    int choice;

    auto_Record("system Started");

    do
    {
        showMain_Menu();
        printf("\nEnter your choice (0-8): ");
        scanf("%d", &choice);
        // jodi keo char type kore
        if (scanf("%d", &choice) != 1)
        {
            printf("\n\t  [!] Invalid Input! Please enter a number (0-8).");
            while (getchar() != '\n')
                ; // input clear kore surute niye jabe
            printf("\n\t  Press Enter to try again...");
            getchar();
            continue;
        }
        switch (choice)
        {
        case 1:
            product_Management();
            break;
        case 2:
            inventory_Management();
            break;
        case 3:
            employee_Management();
            break;
        case 4:
            sales_Billing();
            break;
        case 5:
            customer_Management();
            break;
        case 6:
            reports_Analytics();
            break;
        case 7:
            store_Navigation();
            break;
        case 8:
            settings();
            break;
        case 0:
            printf("\nThank you!\n");
            auto_Record("system closed");
            break;
        default:
            printf("\nInvalid choice!\n");
        }
        // 0 chaple exit korbe & enter chapte bolbe
        if (choice != 0)
        {
            printf("\nPress Enter to continue...");
            getchar();
            getchar(); // enter kye er jonno wait
        }

    } while (choice != 0);
    return 0;
}
void showMain_Menu()
{ // main menu show
    system("cls");
    // for time and date
    time_t t;
    struct tm *tm_info;
    char date[20], time_str[20];
    time(&t);
    tm_info = localtime(&t);
    strftime(date, 20, "%d-%b-%Y", tm_info);
    strftime(time_str, 20, "%I:%M %p", tm_info);

    printf("\n\t  ╔══════════════════════════════════════════════════════════╗");
    printf("\n\t  ║          ADVANCE DEPARTMENT STORE MANAGEMENT             ║");
    printf("\n\t  ║                 & TRACKING SYSTEM                        ║");
    printf("\n\t  ╠══════════════════════════════════════════════════════════╣");
    printf("\n\t  ║  Version: 1.0           Status: [Administrator Connected]║");
    printf("\n\t  ╚══════════════════════════════════════════════════════════╝");

    printf("\n\t   [DASHBOARD]                              [TIME: %s]", time_str);
    printf("\n\t   [DATE: %s]", date);

    printf("\n\n\t   ----------- SYSTEM STATISTICS (REAL-TIME) -----------");
    printf("\n\t    Total Products : 2,547    |    Low Stock    : 12");
    printf("\n\t    Active Staff   : 48       |    Today's Sales: ৳35,000");
    printf("\n\t   -----------------------------------------------------");

    printf("\n\n\t   [1] Product Management   |   [5] Customer Management");
    printf("\n\t   [2] Inventory Management   |  [6] Reports & Analytics");
    printf("\n\t   [3] Employee Management    |  [7] Store Navigation");
    printf("\n\t   [4] Sales & Billing        |  [8] System Settings");
    printf("\n\t   [0] Exit System");

    printf("\n\n\t  ══════════════════════════════════════════════════════════");
    printf("\n\t  Select an option [0-8]: ");
}
