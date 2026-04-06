#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct SystemConfig
{
    char storeName[100];
    char address[200];
    char phone[20];
    char email[50];
    float taxRate;
    int lowStockThreshold;
    int expiryAlertDays;
    char currency[10];
    char timezone[30];
    int autoBackup;
    char backupPath[200];
};

struct User
{
    int id;
    char username[50];
    char password[50];
    char role[20]; // Admin, Manager, Staff
    int isActive;
    char lastLogin[20];
};
// Backup record
struct BackupRecord
{
    char backupId[20];
    char backupDate[20];
    char backupType[20];
    int fileCount;
    float fileSize;
    char status[20];
};

// Discount structure
struct Discount
{
    int id;
    char name[50];
    float percentage;
    char startDate[20];
    char endDate[20];
    int isActive;
    char applicableTo[50]; // All, Category, Product
    int targetId;          // Category ID or Product ID
};

struct SystemConfig config;
struct User users[100];
int userCount = 0;
struct Discount discounts[100];
int discountCount = 0;
struct BackupRecord backups[50];
int backupCount = 0;

int currentUserId = -1;
char currentUserRole[20] = "";

// Default admin user
void initDefaultUsers()
{
    userCount = 0;

    users[userCount].id = 1;
    strcpy(users[userCount].username, "admin");
    strcpy(users[userCount].password, "admin123");
    strcpy(users[userCount].role, "Admin");
    users[userCount].isActive = 1;
    userCount++;

    users[userCount].id = 2;
    strcpy(users[userCount].username, "manager");
    strcpy(users[userCount].password, "manager123");
    strcpy(users[userCount].role, "Manager");
    users[userCount].isActive = 1;
    userCount++;

    users[userCount].id = 3;
    strcpy(users[userCount].username, "staff");
    strcpy(users[userCount].password, "staff123");
    strcpy(users[userCount].role, "Staff");
    users[userCount].isActive = 1;
    userCount++;
}

void loadSystemConfig()
{
    FILE *file = fopen("data/system_config.dat", "r");
    if (file == NULL)
    {
        // Default configuration
        strcpy(config.storeName, "Advanced Department Store");
        strcpy(config.address, "123 Business Avenue, City Center");
        strcpy(config.phone, "+88801538382381");
        strcpy(config.email, "info@store.com");
        config.taxRate = 10.0;
        config.lowStockThreshold = 10;
        config.expiryAlertDays = 30;
        strcpy(config.currency, "BDT");
        strcpy(config.timezone, "Asia/Dhaka");
        config.autoBackup = 1;
        strcpy(config.backupPath, "backups/");
        return;
    }

    fscanf(file, "StoreName: %[^\n]\n", config.storeName);
    fscanf(file, "Address: %[^\n]\n", config.address);
    fscanf(file, "Phone: %[^\n]\n", config.phone);
    fscanf(file, "Email: %[^\n]\n", config.email);
    fscanf(file, "TaxRate: %f\n", &config.taxRate);
    fscanf(file, "LowStockThreshold: %d\n", &config.lowStockThreshold);
    fscanf(file, "ExpiryAlertDays: %d\n", &config.expiryAlertDays);
    fscanf(file, "Currency: %[^\n]\n", config.currency);
    fscanf(file, "Timezone: %[^\n]\n", config.timezone);
    fscanf(file, "AutoBackup: %d\n", &config.autoBackup);
    fscanf(file, "BackupPath: %[^\n]\n", config.backupPath);

    fclose(file);
}

void saveSystemConfig()
{
    FILE *file = fopen("data/system_config.dat", "w");
    if (file == NULL)
        return;

    fprintf(file, "StoreName: %s\n", config.storeName);
    fprintf(file, "Address: %s\n", config.address);
    fprintf(file, "Phone: %s\n", config.phone);
    fprintf(file, "Email: %s\n", config.email);
    fprintf(file, "TaxRate: %.2f\n", config.taxRate);
    fprintf(file, "LowStockThreshold: %d\n", config.lowStockThreshold);
    fprintf(file, "ExpiryAlertDays: %d\n", config.expiryAlertDays);
    fprintf(file, "Currency: %s\n", config.currency);
    fprintf(file, "Timezone: %s\n", config.timezone);
    fprintf(file, "AutoBackup: %d\n", config.autoBackup);
    fprintf(file, "BackupPath: %s\n", config.backupPath);

    fclose(file);
}

void systemConfiguration()
{
    int subchoice;

    do
    {
        clearScreen();
        printf("\n\t           SYSTEM CONFIGURATION");
        printf("\n\t--------------------------------------\n");

        printf("\n\t Current Settings:");
        printf("\n\t ------------------------------------");
        printf("\n\t Store Name        : %s", config.storeName);
        printf("\n\t Address           : %s", config.address);
        printf("\n\t Phone             : %s", config.phone);
        printf("\n\t Email             : %s", config.email);
        printf("\n\t Tax Rate          : %.2f%%", config.taxRate);
        printf("\n\t Low Stock Alert   : %d units", config.lowStockThreshold);
        printf("\n\t Expiry Alert Days : %d days", config.expiryAlertDays);
        printf("\n\t Currency          : %s", config.currency);
        printf("\n\t Auto Backup       : %s", config.autoBackup ? "Enabled" : "Disabled");
        printf("\n\t ------------------------------------");

        printf("\n\n\t 1. Edit Store Information");
        printf("\n\t 2. Edit Tax & Alert Settings");
        printf("\n\t 3. Toggle Auto Backup");
        printf("\n\t 0. Back to Settings Menu");
        printf("\n\t--------------------------------------");
        printf("\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        if (subchoice == 1)
        {
            printf("\n\t Enter New Store Name: ");
            scanf(" %[^\n]", config.storeName);
            printf("\t Enter New Address: ");
            scanf(" %[^\n]", config.address);
            printf("\t Enter New Phone: ");
            scanf(" %[^\n]", config.phone);
            printf("\t Enter New Email: ");
            scanf(" %[^\n]", config.email);
            saveSystemConfig();
            printf("\n\t Configuration saved successfully!");
        }
        else if (subchoice == 2)
        {
            printf("\n\t Enter Tax Rate (%%): ");
            scanf("%f", &config.taxRate);
            printf("\t Enter Low Stock Threshold: ");
            scanf("%d", &config.lowStockThreshold);
            printf("\t Enter Expiry Alert Days: ");
            scanf("%d", &config.expiryAlertDays);
            saveSystemConfig();
            printf("\n\t Alert settings updated!");
        }
        else if (subchoice == 3)
        {
            config.autoBackup = !config.autoBackup;
            saveSystemConfig();
            printf("\n\t Auto Backup %s", config.autoBackup ? "Enabled" : "Disabled");
        }

        if (subchoice != 0)
        {
            printf("\n\n\t Press Enter to continue...");
            getchar();
            getchar();
        }

    } while (subchoice != 0);
}

// BACKUP & RESTORE 

void createBackup()
{
    clearScreen();

    printf("\n\t           BACKUP DATA");
    printf("\n\t--------------------------------------\n");

    char backupDir[200];
    time_t t;
    struct tm *tm_info;
    char timestamp[30];

    time(&t);
    tm_info = localtime(&t);
    strftime(timestamp, 30, "%Y%m%d_%H%M%S", tm_info);

    sprintf(backupDir, "%sbackup_%s", config.backupPath, timestamp);

    // Create backup directory
    char command[500];
#ifdef _WIN32
    sprintf(command, "mkdir %s", backupDir);
#else
    sprintf(command, "mkdir -p %s", backupDir);
#endif
    system(command);

    int fileCount = 0;
    float totalSize = 0;

    // Backup product data
    FILE *src, *dest;
    char srcPath[200], destPath[200];

    // Backup products.dat
    src = fopen("data/products.dat", "r");
    if (src != NULL)
    {
        sprintf(destPath, "%s/products.dat", backupDir);
        dest = fopen(destPath, "w");
        if (dest != NULL)
        {
            char ch;
            while ((ch = fgetc(src)) != EOF)
            {
                fputc(ch, dest);
            }
            fclose(dest);
            fileCount++;
        }
        fclose(src);
    }

    // Backup bills.dat
    src = fopen("data/bills.dat", "r");
    if (src != NULL)
    {
        sprintf(destPath, "%s/bills.dat", backupDir);
        dest = fopen(destPath, "w");
        if (dest != NULL)
        {
            char ch;
            while ((ch = fgetc(src)) != EOF)
            {
                fputc(ch, dest);
            }
            fclose(dest);
            fileCount++;
        }
        fclose(src);
    }

    // Backup employees.dat
    src = fopen("data/employees.dat", "r");
    if (src != NULL)
    {
        sprintf(destPath, "%s/employees.dat", backupDir);
        dest = fopen(destPath, "w");
        if (dest != NULL)
        {
            char ch;
            while ((ch = fgetc(src)) != EOF)
            {
                fputc(ch, dest);
            }
            fclose(dest);
            fileCount++;
        }
        fclose(src);
    }

    // Backup customers.dat
    src = fopen("data/customers.dat", "r");
    if (src != NULL)
    {
        sprintf(destPath, "%s/customers.dat", backupDir);
        dest = fopen(destPath, "w");
        if (dest != NULL)
        {
            char ch;
            while ((ch = fgetc(src)) != EOF)
            {
                fputc(ch, dest);
            }
            fclose(dest);
            fileCount++;
        }
        fclose(src);
    }

    // Backup config
    src = fopen("data/system_config.dat", "r");
    if (src != NULL)
    {
        sprintf(destPath, "%s/system_config.dat", backupDir);
        dest = fopen(destPath, "w");
        if (dest != NULL)
        {
            char ch;
            while ((ch = fgetc(src)) != EOF)
            {
                fputc(ch, dest);
            }
            fclose(dest);
            fileCount++;
        }
        fclose(src);
    }

    // Record backup
    struct BackupRecord newBackup;
    sprintf(newBackup.backupId, "BKP_%s", timestamp);
    strcpy(newBackup.backupDate, ctime(&t));
    newBackup.backupDate[strlen(newBackup.backupDate) - 1] = '\0';
    strcpy(newBackup.backupType, "Full");
    newBackup.fileCount = fileCount;
    newBackup.fileSize = totalSize;
    strcpy(newBackup.status, "Success");

    backups[backupCount] = newBackup;
    backupCount++;

    printf("\n\t Backup created successfully!");
    printf("\n\t Location: %s", backupDir);
    printf("\n\t Files backed up: %d", fileCount);
    printf("\n\t Backup ID: %s", newBackup.backupId);

    auto_Record("Backup created");
}

void restoreData()
{
    clearScreen();

    printf("\n\t           RESTORE DATA");
    printf("\n\t--------------------------------------\n");

    if (backupCount == 0)
    {
        printf("\n\t No backups found!");
        return;
    }

    printf("\n\t Available Backups:");
    printf("\n\t %-15s %-20s %-10s %-10s",
           "Backup ID", "Date", "Files", "Status");
    printf("\n\t------------------------------------------");

    for (int i = 0; i < backupCount; i++)
    {
        printf("\n\t %-15s %-20s %-10d %-10s",
               backups[i].backupId,
               backups[i].backupDate,
               backups[i].fileCount,
               backups[i].status);
    }

    char backupId[20];
    printf("\n\n\t Enter Backup ID to restore: ");
    scanf("%s", backupId);

    int found = 0;
    for (int i = 0; i < backupCount; i++)
    {
        if (strcmp(backups[i].backupId, backupId) == 0)
        {
            found = 1;

            char backupPath[200];
            sprintf(backupPath, "%s%s", config.backupPath, backupId);

            printf("\n\t Warning: Restoring will overwrite current data!");
            printf("\n\t Are you sure? (1=Yes, 0=No): ");
            int confirm;
            scanf("%d", &confirm);

            if (confirm == 1)
            {
                // Restore products
                char srcPath[200], destPath[200];
                sprintf(srcPath, "%s/products.dat", backupPath);
                sprintf(destPath, "data/products.dat");

                FILE *src = fopen(srcPath, "r");
                if (src != NULL)
                {
                    FILE *dest = fopen(destPath, "w");
                    if (dest != NULL)
                    {
                        char ch;
                        while ((ch = fgetc(src)) != EOF)
                        {
                            fputc(ch, dest);
                        }
                        fclose(dest);
                    }
                    fclose(src);
                }

                printf("\n\t Data restored successfully!");
                printf("\n\t Please restart the application for changes to take effect.");
                auto_Record("Data restored");
            }
            else
            {
                printf("\n\t Restore cancelled.");
            }
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Backup not found!");
    }
}

// USER PERMISSIONS 

int checkPermission(char *requiredRole)
{
    if (strcmp(currentUserRole, "Admin") == 0)
        return 1;
    if (strcmp(currentUserRole, "Manager") == 0 && strcmp(requiredRole, "Staff") != 0)
        return 1;
    if (strcmp(currentUserRole, requiredRole) == 0)
        return 1;
    return 0;
}

void userLogin()
{
    clearScreen();

    printf("\n\t           USER LOGIN");
    printf("\n\t--------------------------------------\n");

    char username[50], password[50];

    printf("\n\t Username: ");
    scanf("%s", username);
    printf("\t Password: ");
    scanf("%s", password);

    int found = 0;
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0 &&
            users[i].isActive == 1)
        {
            found = 1;
            currentUserId = users[i].id;
            strcpy(currentUserRole, users[i].role);

            time_t t;
            struct tm *tm_info;
            time(&t);
            tm_info = localtime(&t);
            strftime(users[i].lastLogin, 20, "%d-%m-%Y %H:%M", tm_info);

            printf("\n\t Login successful!");
            printf("\n\t Welcome, %s", users[i].username);
            printf("\n\t Role: %s", users[i].role);
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Invalid username or password!");
    }

    auto_Record("User login attempted");
}

void userManagement()
{
    if (!checkPermission("Admin"))
    {
        printf("\n\t Access denied! Admin permission required.");
        return;
    }

    int subchoice;

    do
    {
        clearScreen();
        printf("\n\t           USER MANAGEMENT");
        printf("\n\t--------------------------------------\n");

        printf("\n\t %-5s %-15s %-12s %-10s",
               "ID", "Username", "Role", "Status");
        printf("\n\t------------------------------------------");

        for (int i = 0; i < userCount; i++)
        {
            printf("\n\t %-5d %-15s %-12s %-10s",
                   users[i].id,
                   users[i].username,
                   users[i].role,
                   users[i].isActive ? "Active" : "Inactive");
        }

        printf("\n\n\t 1. Add New User");
        printf("\n\t 2. Edit User");
        printf("\n\t 3. Delete/Deactivate User");
        printf("\n\t 4. Change User Role");
        printf("\n\t 0. Back");
        printf("\n\t--------------------------------------");
        printf("\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        if (subchoice == 1)
        {
            if (userCount >= 100)
            {
                printf("\n\t Maximum users reached!");
            }
            else
            {
                users[userCount].id = userCount + 1;
                printf("\n\t Username: ");
                scanf("%s", users[userCount].username);
                printf("\t Password: ");
                scanf("%s", users[userCount].password);
                printf("\t Role (Admin/Manager/Staff): ");
                scanf("%s", users[userCount].role);
                users[userCount].isActive = 1;
                userCount++;
                printf("\n\t User added successfully!");
            }
        }
        else if (subchoice == 2)
        {
            int uid;
            printf("\n\t Enter User ID: ");
            scanf("%d", &uid);
            for (int i = 0; i < userCount; i++)
            {
                if (users[i].id == uid)
                {
                    printf("\t New Password: ");
                    scanf("%s", users[i].password);
                    printf("\t User updated!");
                    break;
                }
            }
        }
        else if (subchoice == 3)
        {
            int uid;
            printf("\n\t Enter User ID: ");
            scanf("%d", &uid);
            for (int i = 0; i < userCount; i++)
            {
                if (users[i].id == uid)
                {
                    users[i].isActive = 0;
                    printf("\n\t User deactivated!");
                    break;
                }
            }
        }
        else if (subchoice == 4)
        {
            int uid;
            printf("\n\t Enter User ID: ");
            scanf("%d", &uid);
            for (int i = 0; i < userCount; i++)
            {
                if (users[i].id == uid)
                {
                    printf("\t New Role (Admin/Manager/Staff): ");
                    scanf("%s", users[i].role);
                    printf("\n\t Role updated!");
                    break;
                }
            }
        }

        if (subchoice != 0)
        {
            printf("\n\n\t Press Enter to continue...");
            getchar();
            getchar();
        }

    } while (subchoice != 0);
}

// TAX & DISCOUNT SETTINGS

void discountSettings()
{
    if (!checkPermission("Manager"))
    {
        printf("\n\t Access denied! Manager permission required.");
        return;
    }

    int subchoice;

    do
    {
        clearScreen();
        printf("\n\t           DISCOUNT SETTINGS");
        printf("\n\t--------------------------------------\n");

        printf("\n\t Current Tax Rate: %.2f%%", config.taxRate);

        printf("\n\n\t Active Discounts:");
        printf("\n\t %-5s %-20s %-10s %-12s %-10s",
               "ID", "Name", "Percent", "Valid Until", "Status");
        printf("\n\t--------------------------------------------------");

        for (int i = 0; i < discountCount; i++)
        {
            printf("\n\t %-5d %-20s %-10.1f %-12s %-10s",
                   discounts[i].id,
                   discounts[i].name,
                   discounts[i].percentage,
                   discounts[i].endDate,
                   discounts[i].isActive ? "Active" : "Expired");
        }

        printf("\n\n\t 1. Add New Discount");
        printf("\n\t 2. Edit Tax Rate");
        printf("\n\t 3. Activate/Deactivate Discount");
        printf("\n\t 4. View Discount Details");
        printf("\n\t 0. Back");
        printf("\n\t--------------------------------------");
        printf("\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        if (subchoice == 1)
        {
            discounts[discountCount].id = discountCount + 1;
            printf("\n\t Discount Name: ");
            scanf(" %[^\n]", discounts[discountCount].name);
            printf("\t Discount Percentage: ");
            scanf("%f", &discounts[discountCount].percentage);
            printf("\t End Date (DD-MM-YYYY): ");
            scanf("%s", discounts[discountCount].endDate);
            discounts[discountCount].isActive = 1;
            strcpy(discounts[discountCount].applicableTo, "All");
            discountCount++;
            printf("\n\t Discount added successfully!");
        }
        else if (subchoice == 2)
        {
            printf("\n\t Enter New Tax Rate (%%): ");
            scanf("%f", &config.taxRate);
            saveSystemConfig();
            printf("\n\t Tax rate updated!");
        }
        else if (subchoice == 3)
        {
            int did;
            printf("\n\t Enter Discount ID: ");
            scanf("%d", &did);
            for (int i = 0; i < discountCount; i++)
            {
                if (discounts[i].id == did)
                {
                    discounts[i].isActive = !discounts[i].isActive;
                    printf("\n\t Discount %s!", discounts[i].isActive ? "Activated" : "Deactivated");
                    break;
                }
            }
        }
        else if (subchoice == 4)
        {
            int did;
            printf("\n\t Enter Discount ID: ");
            scanf("%d", &did);
            for (int i = 0; i < discountCount; i++)
            {
                if (discounts[i].id == did)
                {
                    printf("\n\t Name: %s", discounts[i].name);
                    printf("\n\t Percentage: %.1f%%", discounts[i].percentage);
                    printf("\n\t Start Date: %s", discounts[i].startDate);
                    printf("\n\t End Date: %s", discounts[i].endDate);
                    printf("\n\t Status: %s", discounts[i].isActive ? "Active" : "Inactive");
                    printf("\n\t Applicable To: %s", discounts[i].applicableTo);
                    break;
                }
            }
        }

        if (subchoice != 0)
        {
            printf("\n\n\t Press Enter to continue...");
            getchar();
            getchar();
        }

    } while (subchoice != 0);
}

//SETTINGS MAIN MENU

void settings()
{
    int subchoice;

    // Initialize data
    loadSystemConfig();
    initDefaultUsers();

    do
    {
        clearScreen();
        printf("\n\t           SETTINGS");
        printf("\n\t--------------------------------------\n");

        printf("\n\t Current User: ");
        if (currentUserId == -1)
        {
            printf("Not logged in");
        }
        else
        {
            printf("%s (%s)", users[currentUserId - 1].username, currentUserRole);
        }

        printf("\n\n\t 1. System Configuration");
        printf("\n\t 2. Backup & Restore Data");
        printf("\n\t 3. User Management");
        printf("\n\t 4. Tax & Discount Settings");
        printf("\n\t 5. User Login");
        printf("\n\t 6. Logout");
        printf("\n\t 0. Back to Main Menu");
        printf("\n\t--------------------------------------");
        printf("\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        switch (subchoice)
        {
        case 1:
            systemConfiguration();
            break;
        case 2:
            if (checkPermission("Manager"))
            {
                int backupChoice;
                printf("\n\t 1. Create Backup\n\t 2. Restore Data\n\t Enter choice: ");
                scanf("%d", &backupChoice);
                if (backupChoice == 1)
                    createBackup();
                else if (backupChoice == 2)
                    restoreData();
            }
            else
            {
                printf("\n\t Access denied!");
            }
            break;
        case 3:
            userManagement();
            break;
        case 4:
            discountSettings();
            break;
        case 5:
            userLogin();
            break;
        case 6:
            currentUserId = -1;
            strcpy(currentUserRole, "");
            printf("\n\t Logged out successfully!");
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