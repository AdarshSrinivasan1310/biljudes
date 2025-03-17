#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHIFT_KEY 3

struct Customer {
    char name[50];
    int customerID;
    float totalAmount;
};

struct Expense {
    char date[20];
    char description[100];
    float amount;
};

struct User {
    char username[50];
    char password[50];
};

struct Product {
    char name[50];
    int productID;
    int quantity;
    float price;
};

void encryptPassword(char *password) {
    int i;
    for (i = 0; password[i]; i++) {
        if (isalnum(password[i])) {
            password[i] = (password[i] + SHIFT_KEY) % 256;
        }
    }
}

void registerUser() {
    FILE *userFile = fopen("users.dat", "a+");
    struct User newUser;

    printf("Enter username: ");
    scanf("%s", newUser.username);
    printf("Enter password: ");
    scanf("%s", newUser.password);

    encryptPassword(newUser.password);
    fprintf(userFile, "%s %s\n", newUser.username, newUser.password);
    fclose(userFile);
    printf("Registration successful!\n");
}

int loginUser() {
    FILE *userFile = fopen("users.dat", "r");
    char username[50], password[50];
    char storedUsername[50], storedPassword[50];
    int usernameFound = 0;

    if (userFile == NULL) {
        printf("Error: Unable to open user file.\n");
        return 0;
    }

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    encryptPassword(password);

    while (fscanf(userFile, "%s %s", storedUsername, storedPassword) != EOF) {
        if (strcmp(username, storedUsername) == 0) {
            usernameFound = 1;
            if (strcmp(password, storedPassword) == 0) {
                fclose(userFile);
                printf("Login successful.\n");
                return 1;
            } else {
                fclose(userFile);
                printf("Incorrect password.\n");
                return 0;
            }
        }
    }

    fclose(userFile);

    if (!usernameFound) {
        printf("Username not registered.\n");
    }

    return 0;
}

void addCustomerToFile(FILE *file) {
    struct Customer newCustomer;

    printf("Enter customer name: ");
    scanf("%49s", newCustomer.name);
    printf("Enter customer ID: ");
    scanf("%d", &newCustomer.customerID);
    printf("Enter total amount: ");
    scanf("%f", &newCustomer.totalAmount);

    fprintf(file, "%s %d %.2f\n", newCustomer.name, newCustomer.customerID, newCustomer.totalAmount);
    printf("Customer added successfully.\n");
}

void displayCustomersFromFile(FILE *file) {
    struct Customer customer;
    rewind(file);

    printf("\nCustomer List:\n");
    printf("Name\tID\tTotal Amount\n");
    int i = 0;
    while (fscanf(file, "%49s %d %f", customer.name, &customer.customerID, &customer.totalAmount) == 3) {
        printf("%s\t%d\t%.2f\n", customer.name, customer.customerID, customer.totalAmount);
        i++;
    }
}

void searchCustomer(FILE *file) {
    struct Customer customer;
    char query[50];
    int found = 0;

    printf("Enter name or ID to search: ");
    scanf("%s", query);
    rewind(file);

    int i = 0;
    while (fscanf(file, "%49s %d %f", customer.name, &customer.customerID, &customer.totalAmount) == 3) {
        if (strcmp(customer.name, query) == 0 || atoi(query) == customer.customerID) {
            printf("\nFound Customer:\n");
            printf("Name: %s\nID: %d\nTotal: %.2f\n",
                  customer.name, customer.customerID, customer.totalAmount);
            found = 1;
        }
        i++;
    }
    if (!found) printf("Customer not found!\n");
}

void updateCustomer(FILE *file) {
    FILE *temp = fopen("temp.txt", "w");
    struct Customer customer;
    char query[50];
    int found = 0;

    printf("Enter customer ID to update: ");
    scanf("%s", query);
    rewind(file);

    int i = 0;
    while (fscanf(file, "%49s %d %f", customer.name, &customer.customerID, &customer.totalAmount) == 3) {
        if (atoi(query) == customer.customerID) {
            printf("Enter new amount: ");
            scanf("%f", &customer.totalAmount);
            found = 1;
        }
        fprintf(temp, "%s %d %.2f\n", customer.name, customer.customerID, customer.totalAmount);
        i++;
    }

    fclose(file);
    fclose(temp);
    remove("customer_data.txt");
    rename("temp.txt", "customer_data.txt");
    file = fopen("customer_data.txt", "a+");

    printf(found ? "Update successful!\n" : "Customer not found!\n");
}

void deleteCustomer(FILE *file) {
    FILE *temp = fopen("temp.txt", "w");
    struct Customer customer;
    char query[50];
    int found = 0;

    printf("Enter customer ID to delete: ");
    scanf("%s", query);
    rewind(file);

    int i = 0;
    while (fscanf(file, "%49s %d %f", customer.name, &customer.customerID, &customer.totalAmount) == 3) {
        if (atoi(query) != customer.customerID) {
            fprintf(temp, "%s %d %.2f\n", customer.name, customer.customerID, customer.totalAmount);
        } else {
            found = 1;
        }
        i++;
    }

    fclose(file);
    fclose(temp);
    remove("customer_data.txt");
    rename("temp.txt", "customer_data.txt");
    file = fopen("customer_data.txt", "a+");

    printf(found ? "Deletion successful!\n" : "Customer not found!\n");
}

void sortCustomers(FILE *file) {
    struct Customer customers[100];
    int count = 0;

    rewind(file);
    int i = 0;
    while (fscanf(file, "%49s %d %f", customers[i].name, &customers[i].customerID, &customers[i].totalAmount) == 3) {
        count++;
        i++;
    }

    int j;
    for (i = 0; i < count-1; i++) {
        for (j = 0; j < count-i-1; j++) {
            if (customers[j].customerID > customers[j+1].customerID) {
                struct Customer temp = customers[j];
                customers[j] = customers[j+1];
                customers[j+1] = temp;
            }
        }
    }

    printf("\nSorted Customers:\n");
    for (i = 0; i < count; i++) {
        printf("%s\t%d\t%.2f\n", customers[i].name, customers[i].customerID, customers[i].totalAmount);
    }
}

void addExpense() {
    FILE *expFile = fopen("expenses.dat", "a+");
    struct Expense e;

    printf("Enter expense date (DD-MM-YYYY): ");
    scanf("%s", e.date);
    printf("Enter expense description: ");
    scanf(" %[^\n]", e.description);
    printf("Enter amount: ");
    scanf("%f", &e.amount);

    fprintf(expFile, "%s|%s|%.2f\n", e.date, e.description, e.amount);
    fclose(expFile);
    printf("Expense recorded!\n");
}

float calculateTotalExpenses() {
    FILE *expFile = fopen("expenses.dat", "r");
    struct Expense e;
    float total = 0;
    char line[150];

    int i = 0;
    while (fgets(line, sizeof(line), expFile)) {
        sscanf(line, "%[^|]|%[^|]|%f", e.date, e.description, &e.amount);
        total += e.amount;
        i++;
    }
    fclose(expFile);
    return total;
}

void profitLossReport(FILE *file) {
    float totalIncome = 0, totalExpenses = calculateTotalExpenses();
    struct Customer c;

    rewind(file);
    int i = 0;
    while (fscanf(file, "%49s %d %f", c.name, &c.customerID, &c.totalAmount) == 3) {
        totalIncome += c.totalAmount;
        i++;
    }

    printf("\nFinancial Report\n");
    printf("Total Income:  ₹%.2f\n", totalIncome);
    printf("Total Expenses: ₹%.2f\n", totalExpenses);
    printf("Net Profit:     ₹%.2f\n", totalIncome - totalExpenses);
}

void addProduct(FILE *inventoryFile) {
    struct Product newProduct;

    printf("Enter product name: ");
    scanf("%49s", newProduct.name);
    printf("Enter product ID: ");
    scanf("%d", &newProduct.productID);
    printf("Enter quantity: ");
    scanf("%d", &newProduct.quantity);
    printf("Enter price: ");
    scanf("%f", &newProduct.price);

    fprintf(inventoryFile, "%s %d %d %.2f\n", newProduct.name, newProduct.productID, newProduct.quantity, newProduct.price);
    printf("Product added successfully.\n");
}

void updateStockAfterSale(FILE *inventoryFile) {
    FILE *tempFile = fopen("temp_inventory.txt", "w");
    struct Product product;
    int productID, soldQuantity;
    int found = 0;

    printf("Enter product ID sold: ");
    scanf("%d", &productID);
    printf("Enter quantity sold: ");
    scanf("%d", &soldQuantity);

    rewind(inventoryFile);
    int i = 0;
    while (fscanf(inventoryFile, "%49s %d %d %f", product.name, &product.productID, &product.quantity, &product.price) == 4) {
        if (product.productID == productID) {
            if (product.quantity >= soldQuantity) {
                product.quantity -= soldQuantity;
                found = 1;
            } else {
                printf("Error: Not enough stock available.\n");
                fclose(tempFile);
                remove("temp_inventory.txt");
                return;
            }
        }
        fprintf(tempFile, "%s %d %d %.2f\n", product.name, product.productID, product.quantity, product.price);
        i++;
    }

    fclose(inventoryFile);
    fclose(tempFile);

    remove("inventory.txt");
    rename("temp_inventory.txt", "inventory.txt");

    inventoryFile = fopen("inventory.txt", "a+");

    printf(found ? "Stock updated successfully.\n" : "Product not found.\n");
}

void displayLowStock(FILE *inventoryFile) {
    struct Product product;
    int threshold;

    printf("Enter low stock threshold: ");
    scanf("%d", &threshold);

    rewind(inventoryFile);

    printf("\nProducts Below Threshold:\n");
    printf("Name\tID\tQuantity\tPrice\n");

    int i = 0;
    while (fscanf(inventoryFile, "%49s %d %d %f", product.name, &product.productID, &product.quantity, &product.price) == 4) {
        if (product.quantity <= threshold) {
            printf("%s\t%d\t%d\t%.2f\n", product.name, product.productID, product.quantity, product.price);
        }
        i++;
    }
}

void generateInventoryReport(FILE *inventoryFile) {
    struct Product product;

    rewind(inventoryFile);

    printf("\nInventory Report:\n");
    printf("Name\tID\tQuantity\tPrice\n");

    int i = 0;
    while (fscanf(inventoryFile, "%49s %d %d %f", product.name, &product.productID, &product.quantity, &product.price) == 4) {
        printf("%s\t%d\t%d\t%.2f\n", product.name, product.productID, product.quantity, product.price);
        i++;
    }
}

int main() {
    int mainChoice;
    do {
        printf("\n=== Main Menu ===\n");
        printf("1. Register\n2. Login\n3. Exit\nChoice: ");
        scanf("%d", &mainChoice);

        if (mainChoice == 1) {
            registerUser();
        } else if (mainChoice == 2) {
            if (loginUser()) {
                FILE *custFile = fopen("customer_data.txt", "a+");
                FILE *invFile = fopen("inventory.txt", "a+");

                if (!custFile) {
                    perror("Error opening customer file");
                    return 1;
                }
                if (!invFile) {
                    perror("Error opening inventory file");
                    return 1;
                }

                int userChoice;

                do {
                    printf("\n=== Dashboard ===\n");
                    printf("1. Add Customer\n2. List Customers\n3. Search Customer\n");
                    printf("4. Update Customer\n5. Delete Customer\n6. Sort Customers\n");
                    printf("7. Add Expense\n8. Financial Report\n9. Add Product\n");
                    printf("10. Update Stock\n11. Display Low Stock\n12. Inventory Report\n13. Logout\nChoice: ");
                    scanf("%d", &userChoice);

                    switch(userChoice) {
                        case 1: addCustomerToFile(custFile); break;
                        case 2: displayCustomersFromFile(custFile); break;
                        case 3: searchCustomer(custFile); break;
                        case 4: updateCustomer(custFile); break;
                        case 5: deleteCustomer(custFile); break;
                        case 6: sortCustomers(custFile); break;
                        case 7: addExpense(); break;
                        case 8: profitLossReport(custFile); break;
                        case 9: addProduct(invFile); break;
                        case 10: updateStockAfterSale(invFile); break;
                        case 11: displayLowStock(invFile); break;
                        case 12: generateInventoryReport(invFile); break;
                        case 13: printf("Logging out...\n"); break;
                        default: printf("Invalid choice! Please try again.\n");
                    }

                    fflush(custFile);
                    fflush(invFile);
                } while(userChoice != 13);
                fclose(custFile);
                fclose(invFile);
            }
        }
    } while(mainChoice != 3);

    printf("Exiting system...\n");
    return 0;
}
