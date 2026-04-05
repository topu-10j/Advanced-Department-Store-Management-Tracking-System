struct product
{
    int id;
    char name[100];
    char category[50];
    float price;
    int stock;
    char expiryDate[20];
};

void clearScreen();
void auto_Record(char *action);

void addproduct();
void searchproduct();
void editproduct();
void deleteproduct();
void viewallproduct();

#define MAX_PRODUCTS 1000
struct product products[MAX_PRODUCTS];
int productCount = 0;

void product_Management()
{
    int subchoice;
    do
    {
        clearScreen();

        printf("\n\t   Product Management");
        printf("\n\t---------------------------");
        printf("\n\t1. Add Product");
        printf("\n\t2. Search Product");
        printf("\n\t3. Edit Product");
        printf("\n\t4. Delete Product");
        printf("\n\t5. View All Product");
        printf("\n\t0. Back To Main Menu");
        printf("\n\t---------------------------");
        printf("\n\t Enter your choice to continue: ");

        scanf("%d", &subchoice);

        switch (subchoice)
        {
        case 1:
            addproduct();
            break;
        case 2:
            searchproduct();
            break;
        case 3:
            editproduct();
            break;
        case 4:
            deleteproduct();
            break;
        case 5:
            viewallproduct();
            break;
        case 0:
            printf("\n\t Returning to Main Menu\n");
            break;
        default:
            printf("\n\t Invalid choice..!\n");
        }

        if (subchoice != 0)
        {
            printf("\n\n\t Press Enter to continue...");
            getchar();
            getchar();
        }
    }

    while (subchoice != 0);
}

// add products

void addproduct()
{
    clearScreen();

    printf("\n\t     ADD NEW PRODUCT        ");
    printf("\n\t----------------------------");

    printf("\n\t Enter Product ID: ");
    scanf("%d", &products[productCount].id);

    printf("\t Enter Product Name: ");
    scanf("%s", products[productCount].name);

    printf("\t Enter Category: ");
    scanf("%s", products[productCount].category);

    printf("\t Enter Price: ");
    scanf("%f", &products[productCount].price);

    printf("\t Enter Stock Quantity: ");
    scanf("%d", &products[productCount].stock);

    printf("\t  Enter Expiry Date (DD-MM-YYYY): ");
    scanf("%s", products[productCount].expiryDate);

    productCount++;

    printf("\n\t Product added successfully!");
    printf("\n\t Total Products: %d", productCount);

    auto_Record("New product added");
}
// search product
void searchproduct()
{
    int searchId, searchChoice, found = 0;
    char searchName[100];

    clearScreen();

    printf("\n\t           SEARCH PRODUCT");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        printf("\n\t Please add products first.");
        return;
    }

    printf("\n\t Search by:");
    printf("\n\t 1. Product ID");
    printf("\n\t 2. Product Name");
    printf("\n\t Enter your choice: ");
    scanf("%d", &searchChoice);

    // id
    if (searchChoice == 1)
    {
        printf("\n\t Enter Product ID to search: ");
        scanf("%d", &searchId);

        for (int i = 0; i < productCount; i++)
        {
            if (products[i].id == searchId)
            {
                printf("\n\t Product Found!\n");
                printf("\n\t Product Details:");
                printf("\n\t---------------------------");
                printf("\n\t ID       : %d", products[i].id);
                printf("\n\t Name     : %s", products[i].name);
                printf("\n\t Category : %s", products[i].category);
                printf("\n\t Price    : %.2f", products[i].price);
                printf("\n\t Stock    : %d", products[i].stock);
                printf("\n\t Expiry   : %s", products[i].expiryDate);
                printf("\n\t---------------------------------------");
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("\n\t Product not found!");
            printf("\n\t Product ID %d does not exist.", searchId);
        }
    }
    // name
    else if (searchChoice == 2)
    {
        printf("\n\t Enter Product Name to search: ");
        scanf(" %[^\n]s", searchName);

        for (int i = 0; i < productCount; i++)
        {
#ifdef _WIN32
            if (stricmp(products[i].name, searchName) == 0)
#else
            if (strcasecmp(products[i].name, searchName) == 0)
#endif
            {
                printf("\n\t Product Found!\n");
                printf("\n\t Product Details:");
                printf("\n\t---------------------------");
                printf("\n\t ID       : %d", products[i].id);
                printf("\n\t Name     : %s", products[i].name);
                printf("\n\t Category : %s", products[i].category);
                printf("\n\t Price    : %.2f", products[i].price);
                printf("\n\t Stock    : %d", products[i].stock);
                printf("\n\t Expiry   : %s", products[i].expiryDate);
                printf("\n\t---------------------------------------");
                found = 1;
            }
        }
        if (!found)
        {
            printf("\n\t Product not found!");
            printf("\n\t Product Name '%s' does not exist.", searchName);
        }
    }
    else
    {
        printf("\n\t Invalid choice!");
    }

    auto_Record("Product searched");
}
// edit product
void editproduct()
{
    int editId, found = 0;
    int choice;

    clearScreen();

    printf("\n\t           EDIT PRODUCT");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        printf("\n\t Please add products first.");
        return;
    }

    printf("\n\t Enter Product ID to edit: ");
    scanf("%d", &editId);

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == editId)
        {
            found = 1;

            printf("\n\t Product Found!\n");
            printf("\n\t Current Product Details:");
            printf("\n\t---------------------------");
            printf("\n\t ID       : %d", products[i].id);
            printf("\n\t Name     : %s", products[i].name);
            printf("\n\t Category : %s", products[i].category);
            printf("\n\t Price    : ৳%.2f", products[i].price);
            printf("\n\t Stock    : %d", products[i].stock);
            printf("\n\t Expiry   : %s", products[i].expiryDate);
            printf("\n\t---------------------------");

            printf("\n\n\t What do you want to edit?");
            printf("\n\t 1. Name");
            printf("\n\t 2. Category");
            printf("\n\t 3. Price");
            printf("\n\t 4. Stock");
            printf("\n\t 5. Expiry Date");
            printf("\n\t 6. Edit All");
            printf("\n\t Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("\n\t Enter new Name: ");
                scanf("%s", products[i].name);
                printf("\n\t Name updated!");
                break;
            case 2:
                printf("\n\t Enter new Category: ");
                scanf("%s", products[i].category);
                printf("\n\t Category updated!");
                break;
            case 3:
                printf("\n\t Enter new Price: ");
                scanf("%f", &products[i].price);
                printf("\n\t Price updated!");
                break;
            case 4:
                printf("\n\t Enter new Stock: ");
                scanf("%d", &products[i].stock);
                printf("\n\t Stock updated!");
                break;
            case 5:
                printf("\n\t Enter new Expiry Date (DD-MM-YYYY): ");
                scanf("%s", products[i].expiryDate);
                printf("\n\t Expiry Date updated!");
                break;
            case 6:
                printf("\n\t Enter new Name: ");
                scanf("%s", products[i].name);
                printf("\t Enter new Category: ");
                scanf("%s", products[i].category);
                printf("\t Enter new Price: ");
                scanf("%f", &products[i].price);
                printf("\t Enter new Stock: ");
                scanf("%d", &products[i].stock);
                printf("\t Enter new Expiry Date: ");
                scanf("%s", products[i].expiryDate);
                printf("\n\t All details updated!");
                break;
            default:
                printf("\n\t Invalid choice!");
                return;
            }

            auto_Record("Product edited");
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Product not found!");
        printf("\n\t Product ID %d does not exist.", editId);
    }
}
// delete product
void deleteproduct()
{
    int deleteId, found = 0;
    int confirm;

    clearScreen();

    printf("\n\t           DELETE PRODUCT");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        printf("\n\t Please add products first.");
        return;
    }

    printf("\n\t Enter Product ID to delete: ");
    scanf("%d", &deleteId);

    // Search
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == deleteId)
        {
            found = 1;

            printf("\n\t Product Found!\n");
            printf("\n\t Product Details:");
            printf("\n\t---------------------------");
            printf("\n\t ID       : %d", products[i].id);
            printf("\n\t Name     : %s", products[i].name);
            printf("\n\t Category : %s", products[i].category);
            printf("\n\t Price    : ৳%.2f", products[i].price);
            printf("\n\t Stock    : %d", products[i].stock);
            printf("\n\t Expiry   : %s", products[i].expiryDate);
            printf("\n\t---------------------------");

            // Confirmation
            printf("\n\n\t Are you sure you want to delete this product?");
            printf("\n\t 1. Yes");
            printf("\n\t 0. No");
            printf("\n\t Enter your choice: ");
            scanf("%d", &confirm);

            if (confirm == 1)
            {

                for (int j = i; j < productCount - 1; j++)
                {
                    products[j] = products[j + 1];
                }
                productCount--;
                printf("\n\t Product deleted successfully!");
                auto_Record("Product deleted");
            }
            else
            {
                printf("\n\t Deletion cancelled!");
            }
            break;
        }
    }

    if (!found)
    {
        printf("\n\t Product not found!");
        printf("\n\t Product ID %d does not exist.", deleteId);
    }
}
// view all products
void viewallproduct()
{
    clearScreen();

    printf("\n\t           ALL PRODUCTS");
    printf("\n\t--------------------------------------\n");

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        printf("\n\t Please add products first.");
        return;
    }

    printf("\n\t %-5s %-20s %-15s %-10s %-10s %-12s",
           "ID", "Name", "Category", "Price", "Stock", "Expiry");
    printf("\n\t----------------------------------------------------------");

    for (int i = 0; i < productCount; i++)
    {
        printf("\n\t %-5d %-20s %-15s %-10.2f %-10d %-12s",
               products[i].id,
               products[i].name,
               products[i].category,
               products[i].price,
               products[i].stock,
               products[i].expiryDate);
    }

    printf("\n\t-----------------------------------------------------------");
    printf("\n\t Total Products: %d", productCount);

    // Low stock warning
    int lowStockCount = 0;
    for (int i = 0; i < productCount; i++)
    {
        if (products[i].stock < 10)
        {
            lowStockCount++;
        }
    }

    if (lowStockCount > 0)
    {
        printf("\n\t Warning: %d product(s) have low stock (<10)", lowStockCount);
    }

    auto_Record("Viewed all products");
}
