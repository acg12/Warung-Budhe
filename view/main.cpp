#include <stdio.h>
#include "..//controllers//controller.h"

void addDish();
void removeDish();
void addCust();
void searchCust();
void viewWarteg();
void order();
void payment();
void splashScreen();

int main()
{
    int main = 0;
    do {
        clear();
        printf("System: %s\n", detectOS());
        time(&t);
        printf("%s", ctime(&t));
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

    splashScreen();

    return 0;
}

void addDish()
{
    clear();
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
    clear();
    if (!headDish) {
        printf("There are no dishes yet!\n");
        pressEnter();
        return;
    }

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
    clear();
    char name[255];
    do {
        printf("Insert the customer's name [Without space]: ");
        scanf("%[^\n]", name); getchar();
    } while (validCustName(name) == 0);

    insertCust(name);

    printf("Customer has been added!\n");
    pressEnter();
}

void searchCust()
{
    clear();
    char name[255];
    printf("Insert the customer's name to be searched: ");
    scanf("%[^\n]", name); getchar();

    currCust = searchCustName(name);
    if (!currCust) {
        printf("%s is not present\n", name);
    } else {
        printf("%s is present\n", name);
    }

    pressEnter();
}

void viewWarteg()
{
    clear();
    printf("Customer's List\n");
    printCusts();
    pressEnter();
}

void order()
{
    clear();
    if (!headDish) {
        printf("There are no dishes yet!\n");
        pressEnter();
        return;
    }
    
    printDishes();
    char name[255];
    do {
        printf("Insert the customer's name: ");
        scanf("%[^\n]", name); getchar();
        currCust = searchCustName(name);
    } while (!currCust);

    int total;
    printf("Insert the amount of dish: ");
    scanf("%d", &total); getchar();

    char order[255];
    int quant;
    for (int i = 1; i <= total; i++) {
        do {
            printf("[%d] Insert the dish's name and quantity: ", i);
            scanf("%[^x]x%d", order, &quant); getchar();
            order[strlen(order) - 1] = '\0';
        } while (dishAvail(order, quant) == 0);
        pushOrder(order, quant);
    }

    printf("Order success!\n");
    pressEnter();
}

void payment()
{
    clear();
    char name[255];
    do {
        printf("Insert the customer's name: ");
        scanf("%[^\n]", name); getchar();
        currCust = searchCustName(name);
    } while (!currCust);

    printf("%s\n", name);

    Dish *temp = currCust->ordHead;
    int total = 0;
    for (int i = 1; temp != NULL; i++) {
        printf("[%d] %s x%d\n", i, temp->name, temp->quantity);
        total += (temp->price * temp->quantity);
        temp = temp->next;
    }

    printf("Total: Rp%d\n", total);
    pressEnter();

    temp = currCust->ordHead;
    while (temp != NULL) {
        temp = temp->next;
        popOrder();
    }

    deleteCust(name);
}

void splashScreen()
{
    clear();
    printf("Please expand your terminal to full screen!\n");
    pressEnter();

    FILE *fp = fopen("splash-screen.txt", "r");
    while (!feof(fp))
    {
        char string[103];
        fscanf(fp, "%[^\n]\n", string);
        printf("%s\n", string);
    }
    fclose(fp);
}