void clearScreen();
void auto_Record(char *action);

struct Employee
{
    int id;
    char name[100];
    char position[50];
    float salary;
    int attendance;
    char shift[20];
    char joinDate[20];
    int totalDays;
};
struct Employee employees[500];
int employeeCount = 0;

void addEmployee();
void removeEmployee();
void viewAllEmployees();
void attendanceTracking();
void salaryManagement();
void shiftScheduling();

// add employee
void addEmployee()
{
    clearScreen();

    printf("\n\t           ADD EMPLOYEE");
    printf("\n\t--------------------------------------\n");

    if (employeeCount >= 500)
    {
        printf("\n\t Cannot add more employees! Limit reached.");
        return;
    }
    printf("\n\t Enter Employee ID: ");
    scanf("%d", &employees[employeeCount].id);

    // id check
    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == employees[employeeCount].id)
        {
            printf("\n\t Employee ID already exists!");
            return;
        }
    }

    printf("\t Enter Name: ");
    scanf("%s", employees[employeeCount].name);

    printf("\t Enter Position (Manager/Cashier/Staff): ");
    scanf("%s", employees[employeeCount].position);

    printf("\t Enter Salary: ");
    scanf("%f", &employees[employeeCount].salary);

    printf("\t Enter Shift (Morning/Evening/Night): ");
    scanf("%s", employees[employeeCount].shift);

    printf("\t Enter Join Date (DD-MM-YYYY): ");
    scanf("%s", employees[employeeCount].joinDate);

    employees[employeeCount].attendance = 0;
    employees[employeeCount].totalDays = 0;

    employeeCount++;

    printf("\n\t Employee added successfully!");
    printf("\n\t Total Employees: %d", employeeCount);
    auto_Record("New employee added");
}

// view all
void viewAllEmployees()
{
    clearScreen();

    printf("\n\t           ALL EMPLOYEES");
    printf("\n\t--------------------------------------\n");

    if (employeeCount == 0)
    {
        printf("\n\t No employees found!");
        return;
    }

    printf("\n\t %-5s %-20s %-15s %-10s %-8s %-10s",
           "ID", "Name", "Position", "Salary", "Shift", "Attendance");
    printf("\n\t------------------------------------------------------------");

    for (int i = 0; i < employeeCount; i++)
    {
        float attendancePercent = 0;
        if (employees[i].totalDays > 0)
        {
            attendancePercent = (float)employees[i].attendance / employees[i].totalDays * 100;
        }

        printf("\n\t %-5d %-20s %-15s %-10.2f %-8s %-10d(%.0f%%)",
               employees[i].id,
               employees[i].name,
               employees[i].position,
               employees[i].salary,
               employees[i].shift,
               employees[i].attendance,
               attendancePercent);
    }

    printf("\n\t-----------------------------------------------------------");
    printf("\n\t Total Employees: %d", employeeCount);
}

// remove
void removeEmployee()
{
    int removeId, found = 0;
    int confirm;

    clearScreen();

    printf("\n\t           REMOVE EMPLOYEE");
    printf("\n\t--------------------------------------\n");

    if (employeeCount == 0)
    {
        printf("\n\t No employees in database!");
        return;
    }

    printf("\n\t Enter Employee ID to remove: ");
    scanf("%d", &removeId);

    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == removeId)
        {
            found = 1;

            printf("\n\t Employee Found:");
            printf("\n\t Name: %s", employees[i].name);
            printf("\n\t Position: %s", employees[i].position);
            printf("\n\t Salary: %.2f", employees[i].salary);

            printf("\n\n\t Are you sure? (1=Yes, 0=No): ");
            scanf("%d", &confirm);

            if (confirm == 1)
            {
                for (int j = i; j < employeeCount - 1; j++)
                {
                    employees[j] = employees[j + 1];
                }
                employeeCount--;
                printf("\n\t Employee removed successfully!");
                auto_Record("Employee removed");
            }
            else
            {
                printf("\n\t Removal cancelled!");
            }
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Employee not found!");
    }
}

// attendence tracking
void attendanceTracking()
{
    int empId, found = 0;
    int present;

    clearScreen();

    printf("\n\t           ATTENDANCE TRACKING");
    printf("\n\t--------------------------------------\n");

    if (employeeCount == 0)
    {
        printf("\n\t No employees found!");
        return;
    }

    printf("\n\t Enter Employee ID: ");
    scanf("%d", &empId);

    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == empId)
        {
            found = 1;

            printf("\n\t Employee: %s", employees[i].name);
            printf("\n\t Current Attendance: %d days", employees[i].attendance);
            printf("\n\t Total Working Days: %d", employees[i].totalDays);

            printf("\n\n\t Mark attendance for today?");
            printf("\n\t 1. Present");
            printf("\n\t 2. Absent");
            printf("\n\t 3. View Attendance History");
            printf("\n\t Enter choice: ");
            scanf("%d", &present);

            if (present == 1)
            {
                employees[i].attendance++;
                employees[i].totalDays++;
                printf("\n\t Attendance marked for %s!", employees[i].name);

                // percentage
                float percent = (float)employees[i].attendance / employees[i].totalDays * 100;
                printf("\n\t Attendance: %.1f%%", percent);
                auto_Record("Attendance marked");
            }
            else if (present == 2)
            {
                employees[i].totalDays++;
                printf("\n\t Marked as absent!");
            }
            else if (present == 3)
            {
                float percent = (float)employees[i].attendance / employees[i].totalDays * 100;
                printf("\n\t Attendance History:");
                printf("\n\t Total Present: %d days", employees[i].attendance);
                printf("\n\t Total Days: %d days", employees[i].totalDays);
                printf("\n\t Percentage: %.1f%%", percent);

                if (percent < 75)
                {
                    printf("\n\t Warning: Attendance below 75%%!");
                }
                else
                {
                    printf("\n\t Good attendance record!");
                }
            }
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Employee not found!");
    }
}

// salary management
void salaryManagement()
{
    int empId, found = 0;

    clearScreen();

    printf("\n\t           SALARY MANAGEMENT");
    printf("\n\t--------------------------------------\n");

    if (employeeCount == 0)
    {
        printf("\n\t No employees found!");
        return;
    }

    printf("\n\t 1. View Single Employee Salary");
    printf("\n\t 2. View All Employees Salary");
    printf("\n\t 3. Process Monthly Salary");
    printf("\n\t Enter choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("\n\t Enter Employee ID: ");
        scanf("%d", &empId);

        for (int i = 0; i < employeeCount; i++)
        {
            if (employees[i].id == empId)
            {
                found = 1;
                float attendancePercent = 0;
                if (employees[i].totalDays > 0)
                {
                    attendancePercent = (float)employees[i].attendance / employees[i].totalDays * 100;
                }

                float bonus = 0;
                if (attendancePercent >= 95)
                    bonus = employees[i].salary * 0.10;
                else if (attendancePercent >= 85)
                    bonus = employees[i].salary * 0.05;

                float deduction = 0;
                if (attendancePercent < 75)
                    deduction = employees[i].salary * 0.05;

                float netSalary = employees[i].salary + bonus - deduction;

                printf("\n\t Employee: %s", employees[i].name);
                printf("\n\t Position: %s", employees[i].position);
                printf("\n\t Base Salary: %.2f", employees[i].salary);
                printf("\n\t Attendance: %.1f%%", attendancePercent);
                printf("\n\t Bonus: %.2f", bonus);
                printf("\n\t Deduction: %.2f", deduction);
                printf("\n\t Net Salary: %.2f", netSalary);
                break;
            }
        }

        if (!found)
            printf("\n\t Employee not found!");
    }
    else if (choice == 2)
    {
        printf("\n\t %-5s %-20s %-12s %-12s %-10s",
               "ID", "Name", "Base Salary", "Bonus", "Net Salary");
        printf("\n\t-------------------------------------------------");

        float totalSalary = 0;
        for (int i = 0; i < employeeCount; i++)
        {
            float attendancePercent = 0;
            if (employees[i].totalDays > 0)
            {
                attendancePercent = (float)employees[i].attendance / employees[i].totalDays * 100;
            }

            float bonus = 0;
            if (attendancePercent >= 95)
                bonus = employees[i].salary * 0.10;
            else if (attendancePercent >= 85)
                bonus = employees[i].salary * 0.05;

            float netSalary = employees[i].salary + bonus;
            totalSalary += netSalary;

            printf("\n\t %-5d %-20s %-12.2f %-12.2f %-10.2f",
                   employees[i].id,
                   employees[i].name,
                   employees[i].salary,
                   bonus,
                   netSalary);
        }
        printf("\n\t------------------------------------------------");
        printf("\n\t Total Salary Cost: %.2f", totalSalary);
    }
    else if (choice == 3)
    {
        printf("\n\t Processing monthly salary...");
        printf("\n\t Salaries processed for %d employees!", employeeCount);
        auto_Record("Monthly salary processed");
    }
}

// shift schedule
void shiftScheduling()
{
    int empId, found = 0;
    char newShift[20];

    clearScreen();

    printf("\n\t           SHIFT SCHEDULING");
    printf("\n\t--------------------------------------\n");

    if (employeeCount == 0)
    {
        printf("\n\t No employees found!");
        return;
    }

    printf("\n\t Current Shifts:");
    printf("\n\t %-5s %-20s %-10s", "ID", "Name", "Shift");
    printf("\n\t-----------------------------------------");

    for (int i = 0; i < employeeCount; i++)
    {
        printf("\n\t %-5d %-20s %-10s",
               employees[i].id,
               employees[i].name,
               employees[i].shift);
    }

    printf("\n\n\t Enter Employee ID to change shift: ");
    scanf("%d", &empId);

    for (int i = 0; i < employeeCount; i++)
    {
        if (employees[i].id == empId)
        {
            found = 1;
            printf("\n\t Current Shift: %s", employees[i].shift);
            printf("\n\t Available Shifts: Morning, Evening, Night");
            printf("\n\t Enter New Shift: ");
            scanf("%s", newShift);

            strcpy(employees[i].shift, newShift);
            printf("\n\t Shift updated for %s!", employees[i].name);
            auto_Record("Shift changed");
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Employee not found!");
    }
}

void employee_Management()
{
    int subchoice;

    do
    {
        clearScreen();
        printf("\n\t      EMPLOYEE MANAGEMENT");
        printf("\n\t---------------------------------------");
        printf("\n\t 1. Add Employee");
        printf("\n\t 2. View All Employees");
        printf("\n\t 3. Remove Employee");
        printf("\n\t 4. Attendance Tracking");
        printf("\n\t 5. Salary Management");
        printf("\n\t 6. Shift Scheduling");
        printf("\n\t 0. Back to Main Menu");
        printf("\n\t----------------------------------------");
        printf("\n\t Total Employees: %d", employeeCount);
        printf("\n\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        switch (subchoice)
        {
        case 1:
            addEmployee();
            break;
        case 2:
            viewAllEmployees();
            break;
        case 3:
            removeEmployee();
            break;
        case 4:
            attendanceTracking();
            break;
        case 5:
            salaryManagement();
            break;
        case 6:
            shiftScheduling();
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
