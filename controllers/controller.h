#include <string.h>
#include <stdlib.h>
#include "..//models//model.h"

Dish *createDish(char *name, int price, int qty) {
    Dish *newDish = (Dish *)malloc(sizeof(Dish));
    strcpy(newDish->name, name);
    newDish->price = price;
    newDish->quantity = qty;
    newDish->next = NULL;
    return newDish;
}

Customer *createCust(char *name) {
    Customer *newCust = (Customer *)malloc(sizeof(Customer));
    strcpy(newCust->name, name);
    newCust->next = NULL;
    newCust->prev = NULL;
    return newCust;
}

unsigned long hash(char *name)
{
    unsigned long hash = 5381;
    int c;

    while (c = *name++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % TABLE_SIZE;
}

void insertCust(char *name) {
    int key = hash(name);

    Customer *newCust = createCust(name);
    if (!headCust[key]) {
        headCust[key] = tailCust[key] = newCust; 
    } else {
        tailCust[key]->next = newCust;
        newCust->prev = tailCust[key];
        tailCust[key] = newCust;
    }
}

void popHeadCust(char *name) {
    int key = hash(name);

    headCust[key] = headCust[key]->next;
    headCust[key]->prev->next = NULL;
    free(headCust[key]->prev);
    headCust[key]->prev = NULL;
}

void popTailCust(char *name) {
    int key = hash(name);

    tailCust[key] = tailCust[key]->prev;
    tailCust[key]->next->prev = NULL;
    free(tailCust[key]->next);
    tailCust[key]->next = NULL;
}

void deleteCust(char *name) {
    int key = hash(name);

    if (!headCust[key]) {
        return;
    } else if (strcmp(headCust[key]->name, name) == 0 && headCust[key] == tailCust[key]) {
        headCust[key] = tailCust[key] = NULL;
        free(headCust[key]);
    } else {
        if (strcmp(headCust[key]->name, name) == 0) {
            popHeadCust(name);
        } else if (strcmp(tailCust[key]->name, name)) {
            popTailCust(name);
        } else {
            Customer *temp = headCust[key];
            while (temp != NULL && strcmp(temp->name, name) != 0)
                temp = temp->next;

            if (temp != NULL) {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                temp->next = NULL;
                temp->prev = NULL;
                free(temp);
            }
        }
    }
}

Customer *searchCustName(char *name) {
    int key = hash(name);

    if (!headCust[key]) {
        return NULL;
    } else if (strcmp(headCust[key]->name, name) == 0) {
        return headCust[key];
    } else {
        currCust = headCust[key];
        while (currCust != NULL && strcmp(currCust->name, name))
            currCust = currCust->next;
        if (currCust != NULL) {
            // currCust = temp;
            return currCust;
        }
        else
            return NULL;
    }
}

void printCusts() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (headCust[i] != NULL) {
            Customer *temp = headCust[i];
            while (temp != NULL)
            {
                printf("%d. %s\n", i + 1, temp->name);
                temp = temp->next;
            }
        }
    }
}

void pushDish(char *name, int price, int qty) {
    Dish *newDish = createDish(name, price, qty);

    if (!headDish) {
        headDish = tailDish = newDish;
    } else {
        tailDish->next = newDish;
        tailDish = newDish;
    }
}

void popHeadDish(char *name) {
    if (headDish == tailDish)
        headDish = tailDish = NULL;
    else {
        Dish *newHead = headDish->next;
        headDish->next = NULL;
        free(headDish->next);
        headDish = newHead;
    }
}

void popTailDish(char *name) {
    if (headDish == tailDish)
        headDish = tailDish = NULL;
    else {
        Dish *temp = headDish;
        while (temp->next != tailDish)
            temp = temp->next;
        temp->next = NULL;
        free(temp->next);
        tailDish = temp;
    }
}

void popDish(char *name) {
    if (!headDish) {
        return;
    } else if (strcmp(headDish->name, name) == 0) {
        popHeadDish(name);
    } else if (strcmp(tailDish->name, name) == 0) {
        popTailDish(name);
    } else {
        Dish *temp = headDish;
        while (temp->next != NULL && strcmp(temp->next->name, name) != 0)
            temp = temp->next;

        if (temp->next != NULL) {
            Dish *tempNext = temp->next->next;
            temp->next = NULL;
            free(temp->next);
            temp->next = tempNext;
        }
    }
}

int searchDishes(char *name) {
    currDish = headDish;
    while (currDish != NULL && strcmp(currDish->name, name) != 0)
        currDish = currDish->next;
    if (currDish != NULL) {
        // currDish = temp;
        return 1;
    }
    else
        return 0;
}

Dish *searchOrders(Dish *order) {
    Dish *temp = currCust->ordHead;
    while (temp != NULL) {
        if (strcmp(temp->name, order->name) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void pushOrder(char *name, int qty) {
    // kurangi stok e sek
    currDish->quantity -= qty;
    if (currDish->quantity <= 0) {
        popDish(currDish->name);
    }

    // cek ada order sing sama apa ndak
    Dish *order = searchOrders(currDish);

    if (!order) {
        Dish *newOrder = createDish(name, currDish->price, qty);
        if (!currCust->ordHead) {
            currCust->ordHead = currCust->ordTail = newOrder;
        }
        else {
            currCust->ordTail->next = newOrder;
            currCust->ordTail = newOrder;
        }
    } else {
        order->quantity += qty;
    }
}

void popOrder() {
    if (!currCust->ordHead) {
        return;
    } else if (currCust->ordHead == currCust->ordTail) {
        currCust->ordHead = currCust->ordTail = NULL;
    } else {
        Dish *newHead = currCust->ordHead->next;
        currCust->ordHead->next = NULL;
        free(currCust->ordHead);
        currCust->ordHead = newHead;
    }
}

void printDishes() {
    if (!headDish) {
        return;
    } else {
        printf("Budhe's Menu\n");
        printf("==============================================\n");
        // 6 20 10 10 = 46
        printf("%-5s %-19s %-9s %-10s\n", "No.", "Name", "Quantity", "Price");
        
        Dish *temp = headDish;
        int i = 1;
        while (temp != NULL) {
            printf("%4d. %-19s %03d%-6s Rp%-8d\n", i, temp->name, temp->quantity, " ", temp->price);
            temp = temp->next;
        }
        printf("==============================================\n");
    }
}

int validDishName(char *name) {
    if (searchDishes(name) == 1) {
        return 0;
    } else {
        for (int i = 0; name[i] != '\0'; i++) {
            if (name[i] >= 'A' && name[i] <= 'Z') {
                return 0;
            }
            if (name[i] >= '0' && name[i] <= '9') {
                return 0;
            }
        }
        return 1;
    }
}

int validCustName(char *name) {
    // huruf depan kapital atau gak
    if (!(name[0] >= 'A' && name[0] <= 'Z')) {
        return 0;
    }

    // check ada spasi
    for (int i = 0; i < strlen(name); i++) {
        if (name[i] == ' ') {
            return 0;
        }
    }

    // check if sudah ada
    int key = hash(name);

    if (!headCust[key]) {
        return 1;
    }
    else if (strcmp(headCust[key]->name, name) == 0) {
        return 0;
    }
    else {
        Customer *temp = headCust[key];
        while (temp != NULL && strcmp(temp->name, name))
            temp = temp->next;
        if (temp != NULL)
            return 0;
        else
            return 1;
    }
}

int dishAvail(char *name, int qty) {
    if (searchDishes(name) == 0) {
        return 0;
    } else {
        if (currDish->quantity >= qty) {
            return 1;
        } else {
            return 0;
        }
    }
}

void pressEnter() {
    printf("Press enter to continue...");
    getchar();
}

const char *detectOS()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #elif __FreeBSD__
    return "FreeBSD";
    #elif __unix || __unix__
    return "Unix";
    #else
    return "Other";
    #endif
}

void clear() {
    for (int i = 0; i < 10000; i++)
        puts("");
}