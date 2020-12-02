#include <stdio.h>
#include ".//controller.h"

void addDish();
void removeDish();
void addCust();
void searchCust();
void viewWarteg();
void order();
void payment();

int main()
{
    int main = 0;
    do {
        printf("System: %s\n", detectOS());
        // printf("date");
        printf("1. Add Dish\n");
        printf("2. Remove Dish\n");
        printf("3. Add Customer\n");
        printf("4. Search Customer\n");
        printf("5. View Warteg\n");
        printf("6. Order\n");
        printf("7. Payment\n");
        printf("8. Exit Warteg\n");

        do {
            printf(">> ");
            scanf("%d", &main); getchar();
        } while (main < 1 || main > 8);

        switch (main) {
            case 1:
                addDish();
                break;
            case 2:
                removeDish();
                break;
            case 3:
                addCust();
                break;
            case 4:
                searchCust();
                break;
            case 5:
                viewWarteg();
                break;
            case 6:
                order();
                break;
            case 7:
                payment();
                break;
        }
    } while (main != 8);

    printf("Please expand your terminal to full screen!\n");
    pressEnter();

    FILE *fp = fopen("splash-screen.txt", "r");
    while (!feof(fp)) {
        char string[103];
        fscanf(fp, "%[^\n]\n", string);
        printf("%s\n", string);
    }

    return 0;
}

void addDish()
{
    char name[255];
    int price, qty;
    do {
        printf("Insert the name of the dish [Lowercase letters]: ");
        scanf("%[^\n]", name); getchar();
    } while (validDishName(name) == 0);

    do {
        printf("Insert the price of the dish [1000...50000]: ");
        scanf("%d", &price); getchar();
    } while (price < 1000 || price > 50000);

    do {
        printf("Insert the quantity of the dish [1...999]: ");
        scanf("%d", &qty); getchar();
    } while (qty < 1 || qty > 999);

    pushDish(name, price, qty);
    printf("The dish has been added!\n");
    pressEnter();
}

void removeDish()
{
    char name[255];
    printDishes();

    do {
        printf("Insert dish's name to be deleted: ");
        scanf("%[^\n]", name); getchar();
    } while (searchDishes(name) == 0);

    popDish(name);
    printf("The dish has been removed!\n");
    pressEnter();
}

void addCust()
{
    char name[255];
    do {
        printf("Insert the customer's name [Without space]: ");
        scanf("%[^\n]", name); getchar();
    } while (validCustName(name) == 0);

    printf("Customer has been added!\n");
    pressEnter();
}

void searchCust()
{
    char name[255];
    printf("Insert the customer's name to be searched: ");
    scanf("%[^\n]", name); getchar();

    if (searchCustName(name) == 0) {
        printf("%s is not present\n", name);
    } else {
        printf("%s is present\n", name);
    }

    pressEnter();
}

void viewWarteg()
{
    printCusts();
    pressEnter();
}

void order()
{
    char name[255];
    do {
        printf("Insert the customer's name: ");
        scanf("%[^\n]", name); getchar();
    } while (searchCustName(name) == 0);

    int total;
    printf("Insert the amount of dish: ");
    scanf("%d", &total);

    char orders[total][255];
    for (int i = 0; i < total; i++) {
        printf("[%d] Insert the dish's name and quantity: ");
        scanf("%[^\n]", orders[i]);

        
    }
}

void payment();