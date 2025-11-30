#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHIFT 3   // Caesar cipher shift fixed to 3

// User structure
struct User {
    char username[20];
    char password[20];
};

// Product structure
struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
};

struct Product products[100];
int productCount = 0;

struct User users[50];
int userCount = 0;

// Caesar cipher functions
void caesarEncrypt(char* str, int shift) {
    for(int i=0; str[i]!='\0'; i++) {
        char c = str[i];
        if(c >= 'A' && c <= 'Z')
            str[i] = ((c - 'A' + shift) % 26) + 'A';
        else if(c >= 'a' && c <= 'z')
            str[i] = ((c - 'a' + shift) % 26) + 'a';
    }
}

void caesarDecrypt(char* str, int shift) {
    caesarEncrypt(str, 26 - (shift % 26));
}

// Function prototypes
void registerUser();
int loginUser();
void addProduct();
void showProducts();
void searchProduct();
void updateStock();
void billing();
void adminMenu();
void customerMenu();

// Main function
int main() {
    int choice, loginSuccess = 0;

    while(1) {
        printf("\n--- Super Shop Management System ---\n");
        printf("1. Register\n2. Login\n3. Exit\nChoose: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginSuccess = loginUser();
                if(loginSuccess) {
                    int role;
                    printf("Enter role: 1 for Admin, 2 for Customer: ");
                    scanf("%d", &role);
                    if(role == 1) adminMenu();
                    else customerMenu();
                }
                break;
            case 3:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}

// Registration
void registerUser() {
    struct User u;
    printf("Enter username: ");
    scanf("%s", u.username);
    printf("Enter password: ");
    scanf("%s", u.password);

    caesarEncrypt(u.username, SHIFT);
    caesarEncrypt(u.password, SHIFT);

    users[userCount++] = u;
    printf("Registration successful! (Encrypted with shift %d)\n", SHIFT);
}

// Login
int loginUser() {
    char username[20], password[20];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    caesarEncrypt(username, SHIFT);
    caesarEncrypt(password, SHIFT);

    for(int i=0; i<userCount; i++) {
        if(strcmp(username, users[i].username)==0 && strcmp(password, users[i].password)==0) {
            printf("Login successful!\n");
            return 1;
        }
    }

    printf("Invalid credentials!\n");
    return 0;
}

// Admin menu
void adminMenu() {
    int choice;
    while(1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. Add Product\n2. Show Products\n3. Update Stock\n4. Search Product\n5. Logout\nChoose: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addProduct(); break;
            case 2: showProducts(); break;
            case 3: updateStock(); break;
            case 4: searchProduct(); break;
            case 5: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// Customer menu
void customerMenu() {
    int choice;
    while(1) {
        printf("\n--- Customer Menu ---\n");
        printf("1. Show Products\n2. Billing\n3. Logout\nChoose: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: showProducts(); break;
            case 2: billing(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// Add product
void addProduct() {
    struct Product p;
    p.id = productCount + 1;

    printf("Enter product name: ");
    scanf("%s", p.name);

    printf("Enter product price: ");
    scanf("%f", &p.price);

    printf("Enter product quantity: ");
    scanf("%d", &p.quantity);

    products[productCount++] = p;
    printf("Product added successfully!\n");
}

// Show all products
void showProducts() {
    if(productCount == 0) {
        printf("No products available.\n");
        return;
    }

    printf("\nID\tName\tPrice\tQuantity\n");
    for(int i=0; i<productCount; i++) {
        printf("%d\t%s\t%.2f\t%d\n",
               products[i].id,
               products[i].name,
               products[i].price,
               products[i].quantity);
    }
}

// Search product
void searchProduct() {
    char name[50];
    printf("Enter product name to search: ");
    scanf("%s", name);

    int found = 0;

    for(int i=0; i<productCount; i++) {
        if(strcmp(products[i].name, name) == 0) {
            printf("Found: ID:%d Name:%s Price:%.2f Quantity:%d\n",
                   products[i].id,
                   products[i].name,
                   products[i].price,
                   products[i].quantity);

            found = 1;
        }
    }

    if(!found)
        printf("Product not found!\n");
}

// Update stock
void updateStock() {
    int id, qty;
    printf("Enter Product ID to update: ");
    scanf("%d", &id);

    for(int i=0; i<productCount; i++) {
        if(products[i].id == id) {
            printf("Enter new quantity: ");
            scanf("%d", &qty);
            products[i].quantity = qty;
            printf("Stock updated!\n");
            return;
        }
    }

    printf("Product ID not found!\n");
}

// Billing
void billing() {
    int id, qty;
    float total = 0;
    char choice;

    do {
        printf("Enter Product ID to buy: ");
        scanf("%d", &id);

        printf("Enter quantity: ");
        scanf("%d", &qty);

        int found = 0;

        for(int i=0; i<productCount; i++) {
            if(products[i].id == id) {
                if(products[i].quantity >= qty) {
                    total += products[i].price * qty;
                    products[i].quantity -= qty;
                    printf("Added %d x %s to bill.\n", qty, products[i].name);
                } else {
                    printf("Not enough stock!\n");
                }
                found = 1;
                break;
            }
        }

        if(!found)
            printf("Product ID not found!\n");

        printf("Buy more? (y/n): ");
        scanf(" %c", &choice);

    } while(choice == 'y' || choice == 'Y');

    printf("Total amount: %.2f\n", total);
}
