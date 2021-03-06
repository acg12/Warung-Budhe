#include <time.h>
#define TABLE_SIZE 101

struct Dish
{
    char name[255];
    int price;
    int quantity;
    Dish *next;
} * headDish, *tailDish;

static struct Dish *currDish, *currOrder;

struct Customer
{
    char name[255];
    Customer *next, *prev;
    Dish *ordHead, *ordTail;
} * headCust[TABLE_SIZE], *tailCust[TABLE_SIZE], *currCust;

int availCust[TABLE_SIZE] = {};

time_t t;