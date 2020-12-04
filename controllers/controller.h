#include <string.h>
#include <stdlib.h>
#include "..//models//model.h"

// buat create new dish
Dish *createDish(char *name, int price, int qty)
{
    Dish *newDish = (Dish *)malloc(sizeof(Dish));
    strcpy(newDish->name, name);
    newDish->price = price;
    newDish->quantity = qty;
    newDish->next = NULL;
    return newDish;
}

// create new customer
Customer *createCust(char *name)
{
    Customer *newCust = (Customer *)malloc(sizeof(Customer));
    strcpy(newCust->name, name);
    newCust->next = NULL;
    newCust->prev = NULL;
    newCust->ordHead = NULL;
    newCust->ordTail = NULL;
    return newCust;
}

// hash function djb2
unsigned long hash(char *name)
{
    unsigned long hash = 5381;
    int c;

    while (c = *name++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % TABLE_SIZE;
}

// utk insert Cust ke hash table
void insertCust(char *name)
{
    int key = hash(name);

    Customer *newCust = createCust(name);
    if (!headCust[key]) {
        // kalo blm ada yg menempati di head
        headCust[key] = tailCust[key] = newCust;
    }
    else { // kalo udah ada, maka dipasang di tail
        tailCust[key]->next = newCust;
        newCust->prev = tailCust[key];
        tailCust[key] = newCust;
    }
}

// untuk ambil customer dari head
void popHeadCust(char *name)
{
    int key = hash(name);

    if (!headCust[key])
        return;
    else if (headCust[key] == tailCust[key]) {
        headCust[key] = tailCust[key] = NULL;
        free(headCust[key]);
    }
    else {
        headCust[key] = headCust[key]->next;
        headCust[key]->prev->next = NULL;
        free(headCust[key]->prev);
        headCust[key]->prev = NULL;
    }
}

// ambil customer di bagian tail
void popTailCust(char *name)
{
    int key = hash(name);

    if (!headCust[key])
        return;
    else if (headCust[key] == tailCust[key]) {
        headCust[key] = tailCust[key] = NULL;
        free(headCust[key]);
    }
    else {
        tailCust[key] = tailCust[key]->prev;
        tailCust[key]->next->prev = NULL;
        free(tailCust[key]->next);
        tailCust[key]->next = NULL;
    }
}

void deleteCust(char *name)
{
    // search key yg mau di delete
    int key = hash(name);

    if (!headCust[key]) { // kalo invalid return
        return;
    }
    else if (strcmp(headCust[key]->name, name) == 0) {
        // if ada di kepala
        popHeadCust(name);
    }
    else if (strcmp(tailCust[key]->name, name) == 0) {
        // if di tail
        popTailCust(name);
    }
    else {
        // banyak data
        Customer *temp = headCust[key];
        // di loop sampai ketemu
        while (temp != NULL && strcmp(temp->name, name) != 0)
            temp = temp->next;

        if (temp != NULL) {
            // diatur lagi
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            temp->next = NULL;
            temp->prev = NULL;
            free(temp);
        }
    }
}

Customer *searchCustName(char *name) {
    // search the key
    int key = hash(name);

    if (!headCust[key]) {
        return NULL;
    }
    else if (strcmp(headCust[key]->name, name) == 0) {
        // if at head
        return headCust[key];
    }
    else if (strcmp(tailCust[key]->name, name) == 0) {
        // if at tail
        return tailCust[key];
    }
    else {
        // if in the middle
        currCust = headCust[key];
        while (currCust != NULL && strcmp(currCust->name, name))
            currCust = currCust->next;
        if (currCust != NULL)
            return currCust;
        else
            return NULL;
    }
}

void printCusts()
{
    for (int i = 0; i < TABLE_SIZE; i++) {
        // print per head
        if (headCust[i] != NULL) {
            Customer *temp = headCust[i];
            // loop from head to tail
            while (temp != NULL) {
                printf("%d. %s\n", i + 1, temp->name);
                temp = temp->next;
            }
        }
    }
}

// add new dish to linked list
void pushDish(char *name, int price, int qty) {
    Dish *newDish = createDish(name, price, qty);

    if (!headDish) {
        headDish = tailDish = newDish;
    }
    else {
        tailDish->next = newDish;
        tailDish = newDish;
    }
}

// if dish yg dicari ada di head
void popHeadDish(char *name)
{
    if (headDish == tailDish)
        headDish = tailDish = NULL;
    else {
        Dish *newHead = headDish->next;
        headDish->next = NULL;
        free(headDish->next);
        headDish = newHead;
    }
}

// if dish yg dicari ada di tail
void popTailDish(char *name)
{
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

// pop dish from linked list
void popDish(char *name)
{
    if (!headDish) {
        return;
    }
    else if (strcmp(headDish->name, name) == 0) {
        popHeadDish(name);
    }
    else if (strcmp(tailDish->name, name) == 0) {
        popTailDish(name);
    }
    else {
        Dish *temp = headDish;
        while (temp->next != NULL && strcmp(temp->next->name, name) != 0)
            temp = temp->next;

        if (temp->next != NULL)
        {
            Dish *tempNext = temp->next->next;
            temp->next = NULL;
            free(temp->next);
            temp->next = tempNext;
        }
    }
}

Dish *searchDishes(char *name)
{
    Dish *temp = headDish;
    while (temp != NULL && strcmp(temp->name, name) != 0)
        temp = temp->next;
    if (temp != NULL)
        return temp;
    else
        return NULL;
}

Dish *searchOrders(char *name)
{
    Dish *temp = currCust->ordHead;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void pushOrder(char *name, int qty)
{
    // kurangi stoknya dulu
    // cari dish yg dimaksud
    currDish = searchDishes(name);
    currDish->quantity -= qty; // kurangin stoknya
    if (currDish->quantity <= 0) {
        popDish(currDish->name); // kalo sudah habis di remove
    }

    // cek ada order yg sama atau tidak
    currOrder = searchOrders(name);

    if (!currOrder) {
        // if no same existing order
        Dish *newOrder = createDish(name, currDish->price, qty);
        if (!currCust->ordHead) {
            currCust->ordHead = currCust->ordTail = newOrder;
        }
        else {
            currCust->ordTail->next = newOrder;
            currCust->ordTail = newOrder;
        }
    }
    else {
        // if there is
        currOrder->quantity += qty;
    }
}

// to delete the orders
void popOrder()
{
    if (!currCust->ordHead)
        return;
    else if (currCust->ordHead == currCust->ordTail)
        currCust->ordHead = currCust->ordTail = NULL;
    else {
        Dish *newHead = currCust->ordHead->next;
        currCust->ordHead->next = NULL;
        free(currCust->ordHead);
        currCust->ordHead = newHead;
    }
}

void printDishes()
{
    // print menu
    if (!headDish)
        return;
    else {
        printf("Budhe's Menu\n");
        printf("==============================================\n");
        // 6 20 10 10 = 46
        printf("%-5s %-19s %-9s %-10s\n", "No.", "Name", "Quantity", "Price");

        Dish *temp = headDish;
        int i = 1;
        while (temp != NULL)
        {
            printf("%4d. %-19s %03d%-6s Rp%-8d\n", i, temp->name, temp->quantity, " ", temp->price);
            temp = temp->next;
            i++;
        }
        printf("==============================================\n");
    }
}

int validDishName(char *name)
{
    // search dish menu utk cari ada yg sama dgn nama itu atau gak
    Dish *temp = searchDishes(name);
    if (temp) // kalo ada
        return 0;
    else { // kalo gak ada yg sama
        for (int i = 0; name[i] != '\0'; i++) {
            // kalo ada selain huruf kecil
            if (!(name[i] >= 'a' && name[i] <= 'z'))
                return 0;
        }
        return 1;
    }
}

int validCustName(char *name)
{
    // huruf depan kapital atau gak
    if (!(name[0] >= 'A' && name[0] <= 'Z'))
        return 0;

    // check ada spasi
    for (int i = 0; i < strlen(name); i++) {
        if (name[i] == ' ')
            return 0;
    }

    // check if sudah ada
    int key = hash(name);

    if (!headCust[key]) {
        return 1;
    }
    else if (strcmp(headCust[key]->name, name) == 0) {
        // if it's the same as the head
        return 0;
    }
    else {
        // check per nama yg ada di linked list yg ada di key ini
        Customer *temp = headCust[key];
        while (temp != NULL && strcmp(temp->name, name))
            temp = temp->next;
        if (temp != NULL)
            return 0;
        else
            return 1;
    }
}

int dishAvail(char *name, int qty)
{
    currDish = searchDishes(name);
    if (!currDish) {
        return 0;
    }
    else {
        if (currDish->quantity >= qty)
            return 1;
        else
            return 0;
    }
}

void pressEnter()
{
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

void clear()
{
    for (int i = 0; i < 40; i++)
        puts("");
}