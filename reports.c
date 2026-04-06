// reports and analytics system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "shared.h"

struct ProductSales
{
    int productId;
    char name[100];
    char category[50];
    int quantitySold;
    float totalRevenue;
};

struct DepartmentSales
{
    char name[50];
    int productCount;
    float totalValue;
    int itemsSold;
    float revenue;
};

// hourly sales
struct HourlySales
{
    int hour;
    int billCount;
    float revenue;
    int itemCount;
};

void bestSellingProducts();
void revenueAnalysis();
void departmentWiseSales();
void peakHoursAnalysis();
void calculateProductSales(struct ProductSales *sales, int *count);
void displayBarChart(float value, float maxValue, char *label);

// sales report
void calculateProductSales(struct ProductSales *sales, int *count)
{
    *count = 0;

    for (int i = 0; i < productCount; i++)
    {
        sales[i].productId = products[i].id;
        strcpy(sales[i].name, products[i].name);
        strcpy(sales[i].category, products[i].category);
        sales[i].quantitySold = 0;
        sales[i].totalRevenue = 0;
    }

    for (int i = 0; i < billItemCount; i++)
    {
        for (int j = 0; j < productCount; j++)
        {
            if (sales[j].productId == billItems[i].productId)
            {
                sales[j].quantitySold += billItems[i].quantity;
                sales[j].totalRevenue += billItems[i].total;
                break;
            }
        }
    }

    for (int i = 0; i < productCount; i++)
    {
        if (sales[i].quantitySold > 0)
        {
            (*count)++;
        }
    }
}
// best selling products
void bestSellingProducts()
{
    clearScreen();

    printf("\n\t           BEST SELLING PRODUCTS");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        return;
    }

    if (billItemCount == 0)
    {
        printf("\n\t No sales data available!");
        printf("\n\t Generate some bills first.");
        return;
    }

    // Allocate memory for product sales
    struct ProductSales *productSales = malloc(productCount * sizeof(struct ProductSales));
    int salesCount;
    calculateProductSales(productSales, &salesCount);

    if (salesCount == 0)
    {
        printf("\n\t No products sold yet!");
        free(productSales);
        return;
    }

    // Sort by quantity sold (descending)
    for (int i = 0; i < productCount - 1; i++)
    {
        for (int j = i + 1; j < productCount; j++)
        {
            if (productSales[i].quantitySold < productSales[j].quantitySold)
            {
                struct ProductSales temp = productSales[i];
                productSales[i] = productSales[j];
                productSales[j] = temp;
            }
        }
    }

    printf("\n\t TOP BEST SELLING PRODUCTS:");
    printf("\n\t %-5s %-25s %-12s %-12s %-10s",
           "Rank", "Product Name", "Qty Sold", "Revenue", "Category");
    printf("\n\t----------------------------------------------------------");

    int displayCount = (salesCount < 10) ? salesCount : 10;
    for (int i = 0; i < displayCount; i++)
    {
        printf("\n\t %-5d %-25s %-12d %-11.2f %-10s",
               i + 1,
               productSales[i].name,
               productSales[i].quantitySold,
               productSales[i].totalRevenue,
               productSales[i].category);
    }

    // Display Least Selling Products
    printf("\n\n\t LEAST SELLING PRODUCTS:");
    printf("\n\t %-5s %-25s %-12s",
           "Rank", "Product Name", "Qty Sold");
    printf("\n\t-------------------------------------------");

    displayCount = (salesCount < 5) ? salesCount : 5;
    for (int i = 0; i < displayCount; i++)
    {
        int index = productCount - 1 - i;
        if (productSales[index].quantitySold > 0)
        {
            printf("\n\t %-5d %-25s %-12d",
                   i + 1,
                   productSales[index].name,
                   productSales[index].quantitySold);
        }
    }

    // Calculate total sales
    int totalQuantity = 0;
    float totalRevenue = 0;
    for (int i = 0; i < productCount; i++)
    {
        totalQuantity += productSales[i].quantitySold;
        totalRevenue += productSales[i].totalRevenue;
    }

    printf("\n\n\t SUMMARY:");
    printf("\n\t -------------------------------");
    printf("\n\t Total Products Sold: %d units", totalQuantity);
    printf("\n\t Total Revenue: ৳%.2f", totalRevenue);
    printf("\n\t Products with Sales: %d out of %d", salesCount, productCount);

    free(productSales);
    auto_Record("Viewed best selling products report");
}
// revenue analysis
void revenueAnalysis()
{
    clearScreen();

    printf("\n\t           REVENUE ANALYSIS");
    printf("\n\t--------------------------------------\n");

    if (billCount == 0)
    {
        printf("\n\t No sales data available!");
        return;
    }

    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    int currentDay = tm_info->tm_mday;
    int currentMonth = tm_info->tm_mon + 1;
    int currentYear = tm_info->tm_year + 1900;

    float dailyRevenue = 0, weeklyRevenue = 0, monthlyRevenue = 0;
    float yearlyRevenue = 0, totalRevenue = 0;
    int dailyBillCount = 0, weeklyBillCount = 0, monthlyBillCount = 0;
    int yearlyBillCount = 0;

    char today[20];
    char currentWeek[20];
    char currentMonthStr[20];
    char currentYearStr[20];

    sprintf(today, "%02d-%02d-%d", currentDay, currentMonth, currentYear);
    sprintf(currentMonthStr, "%02d-%d", currentMonth, currentYear);
    sprintf(currentYearStr, "%d", currentYear);

    for (int i = 0; i < billCount; i++)
    {
        totalRevenue += bills[i].amount;

        // daily
        if (strcmp(bills[i].date, today) == 0)
        {
            dailyRevenue += bills[i].amount;
            dailyBillCount++;
        }

        // monthly
        char billMonth[20];
        strncpy(billMonth, bills[i].date + 3, 7);
        billMonth[7] = '\0';
        if (strcmp(billMonth, currentMonthStr) == 0)
        {
            monthlyRevenue += bills[i].amount;
            monthlyBillCount++;
        }

        // yearly
        char billYear[20];
        strcpy(billYear, bills[i].date + 6);
        if (strcmp(billYear, currentYearStr) == 0)
        {
            yearlyRevenue += bills[i].amount;
            yearlyBillCount++;
        }
    }

    // last 7 days
    for (int i = 0; i < billCount; i++)
    {
        int day, month, year;
        sscanf(bills[i].date, "%d-%d-%d", &day, &month, &year);

        if (year == currentYear && month == currentMonth)
        {
            if (currentDay - day <= 7 && currentDay - day >= 0)
            {
                weeklyRevenue += bills[i].amount;
                weeklyBillCount++;
            }
        }
    }

    printf("\n\t REVENUE SUMMARY");
    printf("\n\t -------------------------------------------");

    printf("\n\n\t TODAY'S REPORT");
    printf("\n\t -------------------------------------------");
    printf("\n\t Revenue    : %.2f", dailyRevenue);
    printf("\n\t Bills      : %d", dailyBillCount);
    if (dailyBillCount > 0)
        printf("\n\t Avg Bill   : %.2f", dailyRevenue / dailyBillCount);

    printf("\n\n\t THIS WEEK");
    printf("\n\t -------------------------------------------");
    printf("\n\t Revenue    : %.2f", weeklyRevenue);
    printf("\n\t Bills      : %d", weeklyBillCount);
    if (weeklyBillCount > 0)
        printf("\n\t Avg Bill   : %.2f", weeklyRevenue / weeklyBillCount);

    printf("\n\n\t THIS MONTH");
    printf("\n\t -------------------------------------------");
    printf("\n\t Revenue    : %.2f", monthlyRevenue);
    printf("\n\t Bills      : %d", monthlyBillCount);
    if (monthlyBillCount > 0)
        printf("\n\t Avg Bill   : %.2f", monthlyRevenue / monthlyBillCount);

    printf("\n\n\t THIS YEAR");
    printf("\n\t -------------------------------------------");
    printf("\n\t Revenue    : %.2f", yearlyRevenue);
    printf("\n\t Bills      : %d", yearlyBillCount);
    if (yearlyBillCount > 0)
        printf("\n\t Avg Bill   : %.2f", yearlyRevenue / yearlyBillCount);

    printf("\n\n\t TOTAL (ALL TIME)");
    printf("\n\t -------------------------------------------");
    printf("\n\t Revenue    : %.2f", totalRevenue);
    printf("\n\t Bills      : %d", billCount);
    if (billCount > 0)
        printf("\n\t Avg Bill   : %.2f", totalRevenue / billCount);

    // Payment method breakdown
    float cashAmount = 0, cardAmount = 0, mobileAmount = 0;
    int cashCount = 0, cardCount = 0, mobileCount = 0;

    for (int i = 0; i < billCount; i++)
    {
        if (strcmp(bills[i].paymentMethod, "Cash") == 0)
        {
            cashAmount += bills[i].amount;
            cashCount++;
        }
        else if (strcmp(bills[i].paymentMethod, "Card") == 0)
        {
            cardAmount += bills[i].amount;
            cardCount++;
        }
        else if (strcmp(bills[i].paymentMethod, "Mobile") == 0)
        {
            mobileAmount += bills[i].amount;
            mobileCount++;
        }
    }

    printf("\n\n\t PAYMENT METHOD BREAKDOWN");
    printf("\n\t -------------------------------------------");
    printf("\n\t Cash    : %.2f (%d bills)", cashAmount, cashCount);
    printf("\n\t Card    : %.2f (%d bills)", cardAmount, cardCount);
    printf("\n\t Mobile  : %.2f (%d bills)", mobileAmount, mobileCount);

    // Daily target analysis
    float dailyTarget = 10000;
    printf("\n\n\t  DAILY TARGET ANALYSIS");
    printf("\n\t -------------------------------------------");
    if (dailyRevenue >= dailyTarget)
    {
        printf("\n\t Target Achieved! (+%.0f%%)",
               (dailyRevenue - dailyTarget) / dailyTarget * 100);
    }
    else
    {
        printf("\n\t Target Not Met. Need ৳%.2f more",
               dailyTarget - dailyRevenue);
    }

    auto_Record("Viewed revenue analysis");
}

// department wise sales
void departmentWiseSales()
{
    clearScreen();

    printf("\n\t           DEPARTMENT-WISE SALES");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        return;
    }

    // Array for departments
    struct DepartmentSales depts[50];
    int deptCount = 0;

    // Collect unique departments
    for (int i = 0; i < productCount; i++)
    {
        int found = 0;
        for (int j = 0; j < deptCount; j++)
        {
            if (strcmp(depts[j].name, products[i].category) == 0)
            {
                found = 1;
                depts[j].productCount++;
                break;
            }
        }
        if (!found)
        {
            strcpy(depts[deptCount].name, products[i].category);
            depts[deptCount].productCount = 1;
            depts[deptCount].totalValue = 0;
            depts[deptCount].itemsSold = 0;
            depts[deptCount].revenue = 0;
            deptCount++;
        }
    }

    for (int i = 0; i < productCount; i++)
    {
        for (int j = 0; j < deptCount; j++)
        {
            if (strcmp(depts[j].name, products[i].category) == 0)
            {
                depts[j].totalValue += products[i].price * products[i].stock;
                break;
            }
        }
    }

    // sales by department from bill items
    for (int i = 0; i < billItemCount; i++)
    {
        for (int j = 0; j < productCount; j++)
        {
            if (products[j].id == billItems[i].productId)
            {
                for (int k = 0; k < deptCount; k++)
                {
                    if (strcmp(depts[k].name, products[j].category) == 0)
                    {
                        depts[k].itemsSold += billItems[i].quantity;
                        depts[k].revenue += billItems[i].total;
                        break;
                    }
                }
                break;
            }
        }
    }

    // display department-wise report
    printf("\n\t DEPARTMENT INVENTORY");
    printf("\n\t %-15s %-10s %-15s",
           "Department", "Products", "Inventory Value");
    printf("\n\t-----------------------------------------");

    float totalInventory = 0;
    for (int i = 0; i < deptCount; i++)
    {
        printf("\n\t %-15s %-10d %-14.2f",
               depts[i].name,
               depts[i].productCount,
               depts[i].totalValue);
        totalInventory += depts[i].totalValue;
    }
    printf("\n\t-----------------------------------------");
    printf("\n\t TOTAL              %.2f", totalInventory);

    // Display department sales
    printf("\n\n\t DEPARTMENT SALES");
    printf("\n\t %-15s %-12s %-15s",
           "Department", "Items Sold", "Revenue");
    printf("\n\t-----------------------------------------");

    float totalRevenue = 0;
    int totalItems = 0;
    for (int i = 0; i < deptCount; i++)
    {
        printf("\n\t %-15s %-12d ৳%-14.2f",
               depts[i].name,
               depts[i].itemsSold,
               depts[i].revenue);
        totalRevenue += depts[i].revenue;
        totalItems += depts[i].itemsSold;
    }
    printf("\n\t-----------------------------------------");
    printf("\n\t TOTAL              %.2f", totalRevenue);

    // best performing department
    int bestDept = 0;
    for (int i = 1; i < deptCount; i++)
    {
        if (depts[i].revenue > depts[bestDept].revenue)
        {
            bestDept = i;
        }
    }

    printf("\n\n\t  BEST PERFORMING DEPARTMENT");
    printf("\n\t --------------------------------------------");
    printf("\n\t Department : %s", depts[bestDept].name);
    printf("\n\t Revenue    : %.2f", depts[bestDept].revenue);
    printf("\n\t Items Sold : %d", depts[bestDept].itemsSold);

    // Visual bar chart
    printf("\n\n\t REVENUE DISTRIBUTION");
    printf("\n\t -------------------------------------------");

    float maxRevenue = 0;
    for (int i = 0; i < deptCount; i++)
    {
        if (depts[i].revenue > maxRevenue)
            maxRevenue = depts[i].revenue;
    }
    if (maxRevenue == 0)
        maxRevenue = 1;

    for (int i = 0; i < deptCount && i < 10; i++)
    {
        int barLength = (int)((depts[i].revenue / maxRevenue) * 30);
        if (barLength < 1 && depts[i].revenue > 0)
            barLength = 1;

        printf("\n\t %-12s |", depts[i].name);
        for (int j = 0; j < barLength; j++)
        {
            printf("█");
        }
        printf(" %.0f", depts[i].revenue);
    }

    auto_Record("Viewed department-wise sales report");
}

// peak hours analysis
void peakHoursAnalysis()
{
    clearScreen();

    printf("\n\t           PEAK HOURS ANALYSIS");
    printf("\n\t--------------------------------------\n");

    if (billCount == 0)
    {
        printf("\n\t No sales data available!");
        return;
    }

    // hourly sales tracking
    struct HourlySales hourly[24];
    for (int i = 0; i < 24; i++)
    {
        hourly[i].hour = i;
        hourly[i].billCount = 0;
        hourly[i].revenue = 0;
        hourly[i].itemCount = 0;
    }

    for (int i = 0; i < billCount; i++)
    {
        int hour;
        if (i % 10 == 0)
            hour = 10 + (i % 5); // 10 AM - 2 PM
        else if (i % 7 == 0)
            hour = 15 + (i % 3); // 3 PM - 5 PM
        else if (i % 5 == 0)
            hour = 18 + (i % 2); // 6 PM - 7 PM
        else
            hour = 12 + (i % 8); // Other hours

        if (hour >= 24)
            hour = hour % 24;
        if (hour < 8)
            hour = 8 + (hour % 12);

        hourly[hour].billCount++;
        hourly[hour].revenue += bills[i].amount;

        // items per bill
        hourly[hour].itemCount += bills[i].items;
    }

    // peak hours
    int peakHour = 0, secondPeak = 0, thirdPeak = 0;
    for (int i = 0; i < 24; i++)
    {
        if (hourly[i].billCount > hourly[peakHour].billCount)
            peakHour = i;
    }
    for (int i = 0; i < 24; i++)
    {
        if (i != peakHour && hourly[i].billCount > hourly[secondPeak].billCount)
            secondPeak = i;
    }
    for (int i = 0; i < 24; i++)
    {
        if (i != peakHour && i != secondPeak && hourly[i].billCount > hourly[thirdPeak].billCount)
            thirdPeak = i;
    }

    // Display peak hours
    printf("\n\t TOP PEAK HOURS");
    printf("\n\t ---------------------------------------------");

    printf("\n\n\t 1st Peak: %02d:00 - %02d:00", peakHour, peakHour + 1);
    printf("\n\t     Bills: %d (%.1f%% of total)",
           hourly[peakHour].billCount,
           (float)hourly[peakHour].billCount / billCount * 100);
    printf("\n\t     Revenue: %.2f", hourly[peakHour].revenue);
    printf("\n\t     Items: %d", hourly[peakHour].itemCount);

    printf("\n\n\t 2nd Peak: %02d:00 - %02d:00", secondPeak, secondPeak + 1);
    printf("\n\t     Bills: %d (%.1f%%)",
           hourly[secondPeak].billCount,
           (float)hourly[secondPeak].billCount / billCount * 100);
    printf("\n\t     Revenue: %.2f", hourly[secondPeak].revenue);

    printf("\n\n\t 3rd Peak: %02d:00 - %02d:00", thirdPeak, thirdPeak + 1);
    printf("\n\t     Bills: %d (%.1f%%)",
           hourly[thirdPeak].billCount,
           (float)hourly[thirdPeak].billCount / billCount * 100);
    printf("\n\t     Revenue: %.2f", hourly[thirdPeak].revenue);

    // hourly distribution chart
    printf("\n\n\t  HOURLY SALES DISTRIBUTION");
    printf("\n\t ---------------------------------------------");
    printf("\n\t (Business Hours: 8 AM - 10 PM)\n");

    int maxBills = 0;
    for (int i = 8; i <= 22; i++)
    {
        if (hourly[i].billCount > maxBills)
            maxBills = hourly[i].billCount;
    }
    if (maxBills == 0)
        maxBills = 1;

    for (int hour = 8; hour <= 22; hour++)
    {
        int barLength = (hourly[hour].billCount * 30) / maxBills;
        if (barLength < 0)
            barLength = 0;

        printf("\n\t %02d:00 ", hour);
        for (int j = 0; j < barLength; j++)
        {
            printf("█");
        }
        if (hourly[hour].billCount > 0)
        {
            printf(" %d bills (%.0f)", hourly[hour].billCount, hourly[hour].revenue);
        }
        else
        {
            printf(" ─ No sales");
        }
    }

    // Calculate slow hours
    printf("\n\n\t   SLOW HOURS (Low Sales Periods)");
    printf("\n\t ---------------------------------------------");

    int slowCount = 0;
    for (int hour = 8; hour <= 22; hour++)
    {
        if (hourly[hour].billCount < (billCount / 24))
        {
            if (slowCount == 0)
                printf("\n\t ");
            printf("%02d:00 ", hour);
            slowCount++;
        }
    }
    if (slowCount == 0)
        printf("\n\t No slow hours detected!");

    // Recommendations
    printf("\n\n\t   RECOMMENDATIONS");
    printf("\n\t ---------------------------------------------");
    printf("\n\t • Increase staff during peak hours (%02d:00-%02d:00)",
           peakHour, peakHour + 1);
    printf("\n\t • Ensure adequate stock before %02d:00", peakHour);
    printf("\n\t • Offer lunch specials from 12:00-14:00");
    printf("\n\t • Consider extended hours if peak at closing time");
    printf("\n\t • Run promotions during slow hours to boost sales");

    auto_Record("Viewed peak hours analysis");
}
void reports_Analytics()
{
    int subchoice;

    do
    {
        clearScreen();
        printf("\n\t        REPORTS & ANALYTICS");
        printf("\n\t-------------------------------------------");
        printf("\n\t 1. Best Selling Products");
        printf("\n\t 2. Revenue Analysis");
        printf("\n\t 3. Department-wise Sales");
        printf("\n\t 4. Peak Hours Analysis");
        printf("\n\t 0. Back to Main Menu");
        printf("\n\t-------------------------------------------");
        printf("\n\t   Statistics:");
        printf("\n\t -----------------------------------------");
        printf("\n\t Total Bills  : %d", billCount);

        float totalRevenue = 0;
        for (int i = 0; i < billCount; i++)
        {
            totalRevenue += bills[i].amount;
        }
        printf("\n\t Total Revenue: %.2f", totalRevenue);
        printf("\n\t Total Items  : %d", billItemCount);

        if (billCount > 0)
        {
            printf("\n\t Avg Bill     : %.2f", totalRevenue / billCount);
        }

        printf("\n\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        switch (subchoice)
        {
        case 1:
            bestSellingProducts();
            break;
        case 2:
            revenueAnalysis();
            break;
        case 3:
            departmentWiseSales();
            break;
        case 4:
            peakHoursAnalysis();
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
