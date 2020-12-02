#define TABLE_SIZE 100

struct Dish {
    char name[255];
    int price;
    int quantity;
} *headDish, *headTail;

struct Customer {
    char name[255];
    Dish *ordHead, *ordTail;
};

struct Customer custTable[TABLE_SIZE][255];
int currCust;