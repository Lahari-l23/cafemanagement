#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h> // Include for Windows getch()

// Structs for menu items and order history
struct Item {
    char name[50];
    float rating;
    float price;
    int food_Id_No;
    struct Item *next;
    struct Item *prev;
};

struct order_hist {
    int Customer_id;
    int items[10][2];
    float amount;
    char date[11];
    struct order_hist *next;
    struct order_hist *prev;
};

// Global variables
int cust_id = 1;
struct Item* head = NULL;
struct Item* last = NULL;
int Today_custmer = 0;
float total_income = 0;
struct order_hist* head1 = NULL;
struct order_hist* last1 = NULL;

// Function declarations
void insert(char n[], float p, int fin);
struct order_hist* getnewNode_hist();
void saveMenuToFile();
void saveOrderHistoryToFile();
void loadMenuFromFile();
void loadOrderHistoryFromFile();

// Function to save menu to file
void saveMenuToFile() {
    FILE *file = fopen("menu.txt", "w");
    if (!file) {
        printf("Error opening file to save menu.\n");
        return;
    }
    struct Item* temp = head;
    while (temp != NULL) {
        fprintf(file, "%s %.2f %d\n", temp->name, temp->price, temp->food_Id_No);
        temp = temp->next;
    }
    fclose(file);
}

// Function to save order history to file
void saveOrderHistoryToFile() {
    FILE *file = fopen("order_history.txt", "a");
    if (!file) {
        printf("Error opening file to save order history.\n");
        return;
    }
    struct order_hist* temp = head1;
    while (temp != NULL) {
        fprintf(file, "%d %s %.2f\n", temp->Customer_id, temp->date, temp->amount);
        temp = temp->next;
    }
    fclose(file);
}

// Function to load menu from file
void loadMenuFromFile() {
    FILE *file = fopen("menu.txt", "r");
    if (!file) {
        printf("No menu found. Starting fresh.\n");
        return;
    }
    char name[50];
    float price;
    int food_Id_No;
    while (fscanf(file, "%s %f %d", name, &price, &food_Id_No) == 3) {
        insert(name, price, food_Id_No);
    }
    fclose(file);
}

// Function to load order history from file
void loadOrderHistoryFromFile() {
    FILE *file = fopen("order_history.txt", "r");
    if (!file) {
        printf("No order history found.\n");
        return;
    }
    struct order_hist* temp;
    while (!feof(file)) {
        temp = getnewNode_hist();
        fscanf(file, "%d %s %f", &temp->Customer_id, temp->date, &temp->amount);
        if (head1 == NULL) {
            head1 = last1 = temp;
        } else {
            last1->next = temp;
            temp->prev = last1;
            last1 = temp;
        }
    }
    fclose(file);
}

// Function to create a new menu item node
struct Item* getnewNode(char a[], float p, int fin) {
    struct Item* temp = (struct Item*)malloc(sizeof(struct Item));
    temp->food_Id_No = fin;
    strcpy(temp->name, a);
    temp->rating = 4.0;
    temp->price = p;
    temp->next = NULL;
    temp->prev = NULL;
    return temp;
}

// Function to insert a new item into the menu
void insert(char n[], float p, int fin) {
    struct Item* temp1 = getnewNode(n, p, fin);
    if (head == NULL) {
        head = temp1;
        last = temp1;
    } else {
        temp1->prev = last;
        last->next = temp1;
        last = temp1;
    }
    saveMenuToFile();
}

// Function to create a new order history node
struct order_hist* getnewNode_hist() {
    struct order_hist* temp = (struct order_hist*)malloc(sizeof(struct order_hist));
    temp->next = temp->prev = NULL;
    return temp;
}

// Function to display the menu
void Display() {
    printf("                         --------                                       \n");
    printf("---------------------------MENU-----------------------------------------\n");
    printf("                         --------                                       \n");
    printf("INDEX        ITEM NAME        PRICE       RATING\n");
    struct Item* temp = head;
    if (head == NULL) {
        printf("\nEmpty");
    }
    while (temp != NULL) {
        printf("%d\t%s\t%.2f\t%.2f\n", temp->food_Id_No, temp->name, temp->price, temp->rating);
        temp = temp->next;
    }
    printf("-------------------------------------------------------------------------\n");
}

// Function for user login
int login() {
    char username[20];
    char userpwd[11];
    int i;

    printf("Enter your User Name: ");
    scanf("%s", username);

    printf("Enter your password: ");
    for (i = 0; i < 4; i++) {
        userpwd[i] = getch();
        printf("*");
    }
    userpwd[i] = '\0';

    if (!strcmp(username, "ML") && !strcmp(userpwd, "1234")) {
        printf("\n\nLogged In Successfully\n");
        return 1;
    } else {
        printf("\n\nIncorrect username or password\n");
        return 0;
    }
}

// Function to process an order
void order() {
    int a[10][2];
    int n, j = 0, i = 0;
    do {
        printf("\nEnter the FOOD ID NUMBER OF ITEM AND ITS QUANTITY: ");
        for (i = 0; i < 2; i++) {
            scanf("%d", &a[j][i]);
        }
        j++;
        printf("Do you want to add more items?\n1. Yes\n2. No\n");
        scanf("%d", &n);
    } while (n == 1);

    float total_amount = 0.0;
    char name[25];
    char Date[11];
    printf("\nEnter your Name: ");
    scanf("%s", name);

    int d, m, y;
    bool valid_date;
    do {
        valid_date = true;
        printf("Enter the date (DD MM YYYY): ");
        scanf("%d %d %d", &d, &m, &y);
        if (d < 1 || d > 31 || m < 1 || m > 12 || y < 0) {
            printf("Invalid date! Please enter a valid date.\n");
            valid_date = false;
        }
    } while (!valid_date);

    sprintf(Date, "%02d/%02d/%04d", d, m, y);
    int k = 0;
    printf("\n-----------------------------------------------------------\n");
    printf("                            BILL                              \n");
    printf("Name: %s\n", name);
    printf("Date: %s\n", Date);

    for (k = 0; k < j; k++) {
        struct Item* temp = head;
        while (temp != NULL && temp->food_Id_No != a[k][0]) {
            temp = temp->next;
        }
        if (temp) {
            printf("----------------------------------------------------------\n");
            printf("%d\t%s\t%d\t\t%.2f\n", temp->food_Id_No, temp->name, a[k][1], (a[k][1] * (temp->price)));
            total_amount += (a[k][1] * (temp->price));
        } else {
            printf("Food ID %d not found!\n", a[k][0]);
        }
    }
    printf("-----------------------------------------------------------\n");
    printf("\nTotal Payable amount is: %.2f\n", total_amount);
    printf("-----------------------------------------------------------\n");

    struct order_hist* temp2 = getnewNode_hist();
    temp2->amount = total_amount;
    temp2->Customer_id = cust_id++;

    int p, s;
    for (p = 0; p < j; p++) {
        for (s = 0; s < 2; s++) {
            temp2->items[p][s] = a[p][s];
        }
    }

    if (head1 == NULL) {
        head1 = last1 = temp2;
    } else {
        last1->next = temp2;
        temp2->prev = last1;
        last1 = temp2;
    }

    strcpy(temp2->date, Date);
    Today_custmer++;
    total_income += total_amount;

    // Save order history to file
    saveOrderHistoryToFile();
}

// Function to display order history
void display_rd_hist() {
    printf("\n------------------------------ORDER HISTORY-----------------------------------\n");
    printf("\nSR_NO             DATE           TOTAL AMOUNT\n");

    struct order_hist* temp = head1;
    if (head1 == NULL) {
        printf("Empty");
    }
    while (temp != NULL) {
        printf("%d\t\t%s\t\t%f \n", temp->Customer_id, temp->date, temp->amount);
        temp = temp->next;
    }
}

int main() {
    head = NULL;
    last = NULL;

    // Load menu and order history from files
    loadMenuFromFile();
    loadOrderHistoryFromFile();

    int choice;
    outer: { do {
        printf("\n---------------------------------------");
        printf("\n1. FOOD PART");
        printf("\n2. ADMIN PANEL");
        printf("\n3. EXIT");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("");
                int ex;
                do {
                    printf("\n1. FOOD LIST\n2. ORDER\n");
                    printf("\nEnter your choice: ");
                    int n;
                    scanf("%d", &n);
                    switch (n) {
                        case 1:
                            Display();
                            break;
                        case 2:
                            order();
                            break;
                    }
                    printf("1. FOOD PART\n2. MAIN MENU\n");
                    printf("\nEnter your choice: ");
                    scanf("%d", &ex);
                } while (ex == 1);
                break;
            case 2:
                printf("\n");
                int k = login();
                int e;
                if (k == 1) {
                    do {
                        printf("\n------------------------------------------------------------------------\n");
                        printf("1. ADD NEW DISH\n");
                        printf("2. TODAY'S TOTAL INCOME\n");
                        printf("3. TODAY TOTAL NO OF CUSTOMER\n");
                        printf("4. ORDER HISTORY\n");
                        printf("5. LIST OF ITEM\n");
                        printf("6. GO TO MAIN MENU\n");
                        printf("\nEnter your choice: ");
                        int ch;
                        scanf("%d", &ch);
                        switch (ch) {
                            case 1:
                                printf("\n");
                                float p;
                                int fin;
                                char n[50];

                                printf("Enter the name of Item: ");
                                scanf("%s", n);
                                printf("Enter the Price of Item: ");
                                scanf("%f", &p);
                                printf("Enter the food_id_no of Item: ");
                                scanf("%d", &fin);
                                insert(n, p, fin);
                                printf("\n---------------------------------------\n");
                                printf("     NEW DISH IS ADDED SUCCESSFULLY      \n");
                                printf("-----------------------------------------\n");
                                printf("\n");
                                break;

                            case 2:
                                printf("\n---------------------------------------------------------------\n");
                                printf("       TODAY'S TOTAL INCOME IS: %f\n", total_income);
                                printf("---------------------------------------------------------------\n");
                                break;

                            case 3:
                                printf("\n---------------------------------------------------------------\n");
                                printf("     TODAY TOTAL NO OF CUSTOMER VISITED ID: %d\n", Today_custmer);
                                printf("---------------------------------------------------------------\n");
                                break;

                            case 4:
                                printf("\n----------------------------------------------------------------\n");
                                printf("                       ORDER SUMMARY                            \n");
                                printf("----------------------------------------------------------------\n");
                                display_rd_hist();
                                printf("\n");
                                break;

                            case 5:
                                printf("\n");
                                Display();
                                break;
                            case 6:
                                printf("\n");
                                goto outer;
                        }
                    } while (1);
                }
                break;
        }
    } while (choice != 3);
    }
    return 0;
}
