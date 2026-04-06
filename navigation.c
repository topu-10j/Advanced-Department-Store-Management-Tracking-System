// store navigation system
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "utils.h"

struct AdjListNode
{
    int dest;
    int weight; // distance in meters
    struct AdjListNode *next;
};
struct AdjList
{
    struct AdjListNode *head;
};

// Graph structure
struct Graph
{
    int vertexCount;
    struct AdjList *array;
};

// Department structure
struct Department
{
    int id;
    char name[50];
    int x;
    int y;
    char description[200];
};

// Product location structure
struct ProductLocation
{
    int productId;
    char productName[100];
    int departmentId;
    int shelfNo;
    char aisle[10];
};

struct Product
{
    int id;
    char name[100];
    char category[50];
    float price;
    int stock;
    char expiryDate[20];
};

// Path result structure
struct PathResult
{
    int *path;
    int pathLength;
    int totalDistance;
    char *directions;
};

// global variables
#define MAX_DEPARTMENTS 20
#define MAX_PRODUCTS_NAV 2000

struct Graph *storeGraph = NULL;
struct Department departments[MAX_DEPARTMENTS];
int departmentCount = 0;

struct ProductLocation productLocations[MAX_PRODUCTS_NAV];
int productLocationCount = 0;

// Store dimensions
int storeWidth = 100; // meters
int storeHeight = 60; // meters

// Create a new graph
struct Graph *createGraph(int vertexCount)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->vertexCount = vertexCount;
    graph->array = (struct AdjList *)malloc(vertexCount * sizeof(struct AdjList));

    for (int i = 0; i < vertexCount; i++)
    {
        graph->array[i].head = NULL;
    }

    return graph;
}

struct AdjListNode *newAdjListNode(int dest, int weight)
{
    struct AdjListNode *newNode = (struct AdjListNode *)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Add edge to graph
void addEdge(struct Graph *graph, int src, int dest, int weight)
{
    // Add edge src -> dest
    struct AdjListNode *newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Add edge dest -> src (undirected)
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Dijkstra's algorithm
int minDistance(int dist[], int visited[], int vertexCount)
{
    int min = INT_MAX, minIndex = -1;

    for (int v = 0; v < vertexCount; v++)
    {
        if (!visited[v] && dist[v] <= min)
        {
            min = dist[v];
            minIndex = v;
        }
    }

    return minIndex;
}

// Dijkstra's shortest path algorithm
struct PathResult *dijkstra(struct Graph *graph, int start, int end)
{
    int vertexCount = graph->vertexCount;
    int *dist = (int *)malloc(vertexCount * sizeof(int));
    int *parent = (int *)malloc(vertexCount * sizeof(int));
    int *visited = (int *)malloc(vertexCount * sizeof(int));

    // Initialize
    for (int i = 0; i < vertexCount; i++)
    {
        dist[i] = INT_MAX;
        parent[i] = -1;
        visited[i] = 0;
    }

    dist[start] = 0;

    // Find shortest path
    for (int count = 0; count < vertexCount - 1; count++)
    {
        int u = minDistance(dist, visited, vertexCount);
        if (u == -1)
            break;

        visited[u] = 1;

        struct AdjListNode *temp = graph->array[u].head;
        while (temp != NULL)
        {
            int v = temp->dest;
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + temp->weight < dist[v])
            {
                dist[v] = dist[u] + temp->weight;
                parent[v] = u;
            }
            temp = temp->next;
        }
    }

    // Build path result
    struct PathResult *result = (struct PathResult *)malloc(sizeof(struct PathResult));
    result->totalDistance = dist[end];
    result->pathLength = 0;

    // Trace back path
    if (dist[end] != INT_MAX)
    {
        int *tempPath = (int *)malloc(vertexCount * sizeof(int));
        int current = end;
        int index = 0;

        while (current != -1)
        {
            tempPath[index++] = current;
            current = parent[current];
        }

        result->pathLength = index;
        result->path = (int *)malloc(index * sizeof(int));

        // Reverse to get start to end
        for (int i = 0; i < index; i++)
        {
            result->path[i] = tempPath[index - 1 - i];
        }

        free(tempPath);
    }
    else
    {
        result->path = NULL;
        result->pathLength = 0;
    }

    free(dist);
    free(parent);
    free(visited);

    return result;
}
// department setup
void initializeStoreLayout()
{
    departmentCount = 0;

    struct
    {
        int id;
        char name[50];
        int x;
        int y;
        int connections[5][2];
        int connCount;
    } deptSetup[] = {
        {0, "Entrance", 0, 30, {{1, 10}, {2, 15}}, 2},
        {1, "Electronics", 20, 40, {{0, 10}, {2, 8}, {3, 12}}, 3},
        {2, "Clothing", 25, 20, {{0, 15}, {1, 8}, {4, 10}}, 3},
        {3, "Grocery", 45, 45, {{1, 12}, {5, 8}, {6, 10}}, 3},
        {4, "Home Appliances", 50, 15, {{2, 10}, {5, 12}, {7, 8}}, 3},
        {5, "Toys", 65, 35, {{3, 8}, {4, 12}, {6, 6}, {8, 10}}, 4},
        {6, "Books", 40, 55, {{3, 10}, {5, 6}, {9, 8}}, 3},
        {7, "Furniture", 70, 10, {{4, 8}, {8, 12}}, 2},
        {8, "Sports", 80, 30, {{5, 10}, {7, 12}, {9, 8}}, 3},
        {9, "Cashier", 60, 50, {{6, 8}, {8, 8}}, 2}};

    int deptCount = sizeof(deptSetup) / sizeof(deptSetup[0]);

    // Create graph
    storeGraph = createGraph(deptCount);

    // Add departments
    for (int i = 0; i < deptCount; i++)
    {
        departments[i].id = deptSetup[i].id;
        strcpy(departments[i].name, deptSetup[i].name);
        departments[i].x = deptSetup[i].x;
        departments[i].y = deptSetup[i].y;
        sprintf(departments[i].description, "%s Department - Aisle %c",
                deptSetup[i].name, 'A' + i);
        departmentCount++;

        // Add edges
        for (int j = 0; j < deptSetup[i].connCount; j++)
        {
            addEdge(storeGraph, i, deptSetup[i].connections[j][0],
                    deptSetup[i].connections[j][1]);
        }
    }
}

// product location setup
void mapProductToDepartment(int productId, char *productName, char *category)
{
    productLocations[productLocationCount].productId = productId;
    strcpy(productLocations[productLocationCount].productName, productName);

    // Map category to department
    if (strcmp(category, "Electronics") == 0)
    {
        productLocations[productLocationCount].departmentId = 1;
        productLocations[productLocationCount].shelfNo = 100 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "E%d", productId % 20);
    }
    else if (strcmp(category, "Clothing") == 0)
    {
        productLocations[productLocationCount].departmentId = 2;
        productLocations[productLocationCount].shelfNo = 200 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "C%d", productId % 20);
    }
    else if (strcmp(category, "Grocery") == 0)
    {
        productLocations[productLocationCount].departmentId = 3;
        productLocations[productLocationCount].shelfNo = 300 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "G%d", productId % 20);
    }
    else if (strcmp(category, "Home") == 0 || strcmp(category, "Home Appliances") == 0)
    {
        productLocations[productLocationCount].departmentId = 4;
        productLocations[productLocationCount].shelfNo = 400 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "H%d", productId % 20);
    }
    else if (strcmp(category, "Toys") == 0)
    {
        productLocations[productLocationCount].departmentId = 5;
        productLocations[productLocationCount].shelfNo = 500 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "T%d", productId % 20);
    }
    else if (strcmp(category, "Books") == 0)
    {
        productLocations[productLocationCount].departmentId = 6;
        productLocations[productLocationCount].shelfNo = 600 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "B%d", productId % 20);
    }
    else if (strcmp(category, "Furniture") == 0)
    {
        productLocations[productLocationCount].departmentId = 7;
        productLocations[productLocationCount].shelfNo = 700 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "F%d", productId % 20);
    }
    else if (strcmp(category, "Sports") == 0)
    {
        productLocations[productLocationCount].departmentId = 8;
        productLocations[productLocationCount].shelfNo = 800 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "S%d", productId % 20);
    }
    else
    {
        productLocations[productLocationCount].departmentId = 1; // Default Electronics
        productLocations[productLocationCount].shelfNo = 900 + (productId % 50);
        sprintf(productLocations[productLocationCount].aisle, "D%d", productId % 20);
    }

    productLocationCount++;
}

// Sync product locations from product database
void syncProductLocations()
{
    extern struct Product
    {
        int id;
        char name[100];
        char category[50];
        float price;
        int stock;
        char expiryDate[20];
    } products[1000];
    extern int productCount;

    productLocationCount = 0;

    for (int i = 0; i < productCount; i++)
    {
        mapProductToDepartment(products[i].id, products[i].name, products[i].category);
    }
}

// find  shortest path
//  Get department ID for a product
int getProductDepartment(int productId)
{
    for (int i = 0; i < productLocationCount; i++)
    {
        if (productLocations[i].productId == productId)
        {
            return productLocations[i].departmentId;
        }
    }
    return -1;
}

// Generate turn-by-turn directions
char *generateDirections(int path[], int pathLength, int startDept, int endDept)
{
    static char directions[1000];
    directions[0] = '\0';

    char temp[200];

    sprintf(temp, "  Starting from: %s\n", departments[startDept].name);
    strcat(directions, temp);

    for (int i = 1; i < pathLength; i++)
    {
        int current = path[i - 1];
        int next = path[i];

        // Calculate direction based on coordinates
        int dx = departments[next].x - departments[current].x;
        int dy = departments[next].y - departments[current].y;

        char direction[50];
        if (abs(dx) > abs(dy))
        {
            if (dx > 0)
                strcpy(direction, "→ Go straight");
            else
                strcpy(direction, "← Go straight");
        }
        else
        {
            if (dy > 0)
                strcpy(direction, "↓ Go straight");
            else
                strcpy(direction, "↑ Go straight");
        }

        sprintf(temp, "  %d. %s to %s Department (%d meters)\n",
                i, direction, departments[next].name,
                getEdgeDistance(storeGraph, current, next));
        strcat(directions, temp);
    }

    sprintf(temp, "\n  Arrived at: %s Department", departments[endDept].name);
    strcat(directions, temp);

    return directions;
}

// Helper to get edge distance
int getEdgeDistance(struct Graph *graph, int src, int dest)
{
    struct AdjListNode *temp = graph->array[src].head;
    while (temp != NULL)
    {
        if (temp->dest == dest)
        {
            return temp->weight;
        }
        temp = temp->next;
    }
    return 0;
}

// Find shortest path to product
void findShortestPathToProduct()
{
    clearScreen();

    printf("\n\t        FIND SHORTEST PATH TO PRODUCT");
    printf("\n\t----------------------------------------------\n");

    if (storeGraph == NULL)
    {
        initializeStoreLayout();
    }

    if (productCount == 0)
    {
        printf("\n\t No products in database!");
        printf("\n\t Please add products first.");
        return;
    }

    // Sync product locations
    syncProductLocations();

    int productId;
    printf("\n\t Enter Product ID: ");
    scanf("%d", &productId);

    // Find product
    int productDept = getProductDepartment(productId);
    char productName[100] = "";

    extern struct Product products[1000];
    extern int productCount;

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].id == productId)
        {
            strcpy(productName, products[i].name);
            break;
        }
    }

    if (productDept == -1)
    {
        printf("\n\t  Product not found!");
        return;
    }

    // Show product location info
    printf("\n\t  Product Found: %s", productName);
    printf("\n\t  Location Details:");
    printf("\n\t    Department: %s", departments[productDept].name);

    for (int i = 0; i < productLocationCount; i++)
    {
        if (productLocations[i].productId == productId)
        {
            printf("\n\t    Aisle: %s", productLocations[i].aisle);
            printf("\n\t    Shelf: %d", productLocations[i].shelfNo);
            break;
        }
    }

    // Find shortest path from entrance (0) to product department
    struct PathResult *result = dijkstra(storeGraph, 0, productDept);

    if (result->pathLength > 0)
    {
        printf("\n\n\t   SHORTEST PATH FROM ENTRANCE:");
        printf("\n\t -----------------------------------------------");
        printf("\n\t Total Distance: %d meters", result->totalDistance);
        printf("\n\t Departments to visit: %d", result->pathLength);

        printf("\n\n\t  PATH:");
        for (int i = 0; i < result->pathLength; i++)
        {
            if (i == 0)
            {
                printf("\n\t     START → %s", departments[result->path[i]].name);
            }
            else if (i == result->pathLength - 1)
            {
                printf("\n\t     → %s (PRODUCT LOCATION)", departments[result->path[i]].name);
            }
            else
            {
                printf("\n\t    ↓ → %s", departments[result->path[i]].name);
            }
        }

        // Generate turn-by-turn directions
        printf("\n\n\t  TURN-BY-TURN DIRECTIONS:");
        printf("\n\t ------------------------------------------------");

        char *directions = generateDirections(result->path, result->pathLength, 0, productDept);
        printf("\n\t %s", directions);

        // Estimated time
        printf("\n\n\t  Estimated walking time: %d minutes", result->totalDistance / 50);

        free(result->path);
    }
    else
    {
        printf("\n\t  No path found!");
    }

    free(result);
    auto_Record("Found shortest path to product");
}
void displayDepartmentLayout()
{
    clearScreen();

    printf("\n\t           STORE DEPARTMENT LAYOUT");
    printf("\n\t----------------------------------------------\n");

    if (storeGraph == NULL)
    {
        initializeStoreLayout();
    }

    // Create simple map
    char map[61][101];

    // Initialize map with spaces
    for (int i = 0; i <= storeHeight; i++)
    {
        for (int j = 0; j <= storeWidth; j++)
        {
            map[i][j] = ' ';
        }
    }

    // Draw departments on map
    for (int i = 0; i < departmentCount; i++)
    {
        int x = departments[i].x;
        int y = departments[i].y;

        if (x >= 0 && x <= storeWidth && y >= 0 && y <= storeHeight)
        {
            map[y][x] = 'D';

            // Add department name around the dot
            int nameLen = strlen(departments[i].name);
            int startX = x - nameLen / 2;
            if (startX < 0)
                startX = 0;

            for (int j = 0; j < nameLen && startX + j <= storeWidth; j++)
            {
                if (map[y - 1][startX + j] == ' ')
                {
                    map[y - 1][startX + j] = departments[i].name[j];
                }
            }
        }
    }

    // Draw connections
    for (int i = 0; i < departmentCount; i++)
    {
        struct AdjListNode *temp = storeGraph->array[i].head;
        while (temp != NULL)
        {
            if (temp->dest > i)
            { // Draw each connection once
                int x1 = departments[i].x;
                int y1 = departments[i].y;
                int x2 = departments[temp->dest].x;
                int y2 = departments[temp->dest].y;

                // Draw line between points (simplified)
                int dx = abs(x2 - x1);
                int dy = abs(y2 - y1);
                int sx = (x1 < x2) ? 1 : -1;
                int sy = (y1 < y2) ? 1 : -1;
                int err = dx - dy;

                int x = x1, y = y1;
                while (x != x2 || y != y2)
                {
                    if (x >= 0 && x <= storeWidth && y >= 0 && y <= storeHeight)
                    {
                        if (map[y][x] == ' ')
                            map[y][x] = '.';
                    }
                    int e2 = 2 * err;
                    if (e2 > -dy)
                    {
                        err -= dy;
                        x += sx;
                    }
                    if (e2 < dx)
                    {
                        err += dx;
                        y += sy;
                    }
                }
            }
            temp = temp->next;
        }
    }

    // Display map
    printf("\n\t Store Layout Map (D = Department, . = Path):\n");
    printf("\n\t ");
    for (int i = 0; i <= storeWidth; i++)
        printf("─");

    for (int y = 0; y <= storeHeight; y += 3)
    {
        printf("\n\t ");
        for (int x = 0; x <= storeWidth; x++)
        {
            printf("%c", map[y][x]);
        }
    }

    printf("\n\t ");
    for (int i = 0; i <= storeWidth; i++)
        printf("─");

    // Legend
    printf("\n\n\t  LEGEND:");
    printf("\n\t --------------------------------------------");
    for (int i = 0; i < departmentCount; i++)
    {
        printf("\n\t %d. %s Department", departments[i].id, departments[i].name);
    }
    printf("\n\n\t  Entrance is at Department 0");

    auto_Record("Viewed department layout");
}

// Optimize staff route for multiple tasks
void optimizeStaffMovement()
{
    clearScreen();

    printf("\n\t        STAFF MOVEMENT OPTIMIZATION");
    printf("\n\t----------------------------------------------\n");

    if (storeGraph == NULL)
    {
        initializeStoreLayout();
    }

    printf("\n\t This feature helps staff find optimal routes for multiple tasks.\n");
    printf("\n\t Select departments to visit:");

    int selectedDepts[MAX_DEPARTMENTS];
    int selectedCount = 0;

    // Show department list
    printf("\n\n\t Available Departments:");
    for (int i = 0; i < departmentCount; i++)
    {
        printf("\n\t %d. %s", departments[i].id, departments[i].name);
    }

    printf("\n\n\t Enter department IDs (0 to finish):");

    while (selectedCount < MAX_DEPARTMENTS)
    {
        int deptId;
        printf("\n\t Department %d: ", selectedCount + 1);
        scanf("%d", &deptId);

        if (deptId < 0 || deptId >= departmentCount)
        {
            printf("\n\t  Invalid department ID!");
            continue;
        }

        if (deptId == 0 && selectedCount == 0)
        {
            printf("\n\t  At least one department required!");
            continue;
        }

        selectedDepts[selectedCount++] = deptId;

        char more;
        printf("\n\t Add more? (y/n): ");
        scanf(" %c", &more);
        if (more != 'y' && more != 'Y')
            break;
    }

    if (selectedCount == 0)
    {
        printf("\n\t  No departments selected!");
        return;
    }

    // Calculate optimal route (nearest neighbor algorithm)
    int *route = (int *)malloc((selectedCount + 1) * sizeof(int));
    int *visited = (int *)calloc(departmentCount, sizeof(int));
    int routeIndex = 0;

    route[routeIndex++] = 0; // Start from entrance

    for (int step = 0; step < selectedCount; step++)
    {
        int current = route[routeIndex - 1];
        int nearest = -1;
        int minDist = INT_MAX;

        for (int i = 0; i < selectedCount; i++)
        {
            if (!visited[selectedDepts[i]])
            {
                struct PathResult *path = dijkstra(storeGraph, current, selectedDepts[i]);
                if (path->totalDistance < minDist)
                {
                    minDist = path->totalDistance;
                    nearest = selectedDepts[i];
                }
                free(path->path);
                free(path);
            }
        }

        if (nearest != -1)
        {
            route[routeIndex++] = nearest;
            visited[nearest] = 1;
        }
    }

    // Calculate total distance
    int totalDistance = 0;
    for (int i = 1; i < routeIndex; i++)
    {
        struct PathResult *path = dijkstra(storeGraph, route[i - 1], route[i]);
        totalDistance += path->totalDistance;
        free(path->path);
        free(path);
    }

    // Display optimal route
    printf("\n\n\t   OPTIMAL STAFF ROUTE:");
    printf("\n\t ------------------------------------------");
    printf("\n\t Total Distance: %d meters", totalDistance);
    printf("\n\t Estimated Time: %d minutes", totalDistance / 50);

    printf("\n\n\t  ROUTE SEQUENCE:");
    for (int i = 0; i < routeIndex; i++)
    {
        if (i == 0)
        {
            printf("\n\t     START → %s", departments[route[i]].name);
        }
        else if (i == routeIndex - 1)
        {
            printf("\n\t     → %s (FINAL TASK)", departments[route[i]].name);
        }
        else
        {
            printf("\n\t    ↓ → %s", departments[route[i]].name);
        }
    }

    // Generate task order
    printf("\n\n\t  TASK ORDER:");
    for (int i = 1; i < routeIndex; i++)
    {
        printf("\n\t %d. Go to %s Department", i, departments[route[i]].name);

        // Add task suggestions based on department
        if (strcmp(departments[route[i]].name, "Electronics") == 0)
        {
            printf(" (Check stock, Restock shelves)");
        }
        else if (strcmp(departments[route[i]].name, "Grocery") == 0)
        {
            printf(" (Check expiry, Restock perishables)");
        }
        else if (strcmp(departments[route[i]].name, "Cashier") == 0)
        {
            printf(" (Assist billing, Manage queue)");
        }
        else
        {
            printf(" (General assistance)");
        }
    }

    free(route);
    free(visited);
    auto_Record("Staff movement optimized");
}

void store_Navigation()
{
    int subchoice;

    // Initialize graph once
    if (storeGraph == NULL)
    {
        initializeStoreLayout();
    }

    do
    {
        clearScreen();
        printf("\n\t         STORE NAVIGATION");
        printf("\n\t---------------------------------------");
        printf("\n\t 1. Find Shortest Path to Product");
        printf("\n\t 2. View Department Layout");
        printf("\n\t 3. Staff Movement Optimization");
        printf("\n\t 0. Back to Main Menu");
        printf("\n\t---------------------------------------");
        printf("\n\t  Store Statistics:");
        printf("\n\t-----------------------------------------");
        printf("\n\t Departments: %d", departmentCount);
        printf("\n\t Products Mapped: %d", productLocationCount);
        printf("\n\t Store Area: %d sq meters", storeWidth * storeHeight);

        printf("\n\n\t Enter your choice: ");
        scanf("%d", &subchoice);

        switch (subchoice)
        {
        case 1:
            findShortestPathToProduct();
            break;
        case 2:
            displayDepartmentLayout();
            break;
        case 3:
            optimizeStaffMovement();
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