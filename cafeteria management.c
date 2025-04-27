#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ADMIN_PIN "1234" // Simple admin PIN - should be handled more securely in a real system
#define CARD_BALANCE 1000 // Initial balance for all users
#define MENU_FILE "menu.txt"
#define USERS_FILE "users.txt"

// Structure for a menu item
typedef struct MenuItem {
    int id;
    char name[50];
    float price;
    int availability;
    struct MenuItem* next;
} MenuItem;

// Structure for a user
typedef struct User {
    int id;
    char username[50];
    char card_number[20];
    float balance;
    struct User* next;
} User;

// Global variables
MenuItem* menuHead = NULL;
User* userHead = NULL;
int nextMenuItemId = 1;
int nextUserId = 1;

// Function to load menu from file
void loadMenu() {
    FILE *file = fopen("menu.txt", "r");

if (file == NULL) {
    printf("Error opening menu file, creating a new one...\n");
    file = fopen("menu.txt", "w");  // create new file
    if (file == NULL) {
        printf("Failed to create menu file!\n");
        exit(1);  // exit if even creation fails
    }
}

    MenuItem* current = NULL;
    while (!feof(file)) {
        MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
        if (fscanf(file, "%d,%[^,],%f,%d\n", &newItem->id, newItem->name, &newItem->price, &newItem->availability) == 4) {
            newItem->next = NULL;
            if (menuHead == NULL) {
                menuHead = newItem;
                current = newItem;
            } else {
                current->next = newItem;
                current = newItem;
            }
            if (newItem->id >= nextMenuItemId) {
                nextMenuItemId = newItem->id + 1;
            }
        }
    }
    fclose(file);
    printf("Menu loaded successfully.\n");
}

// Function to save menu to file
void saveMenu() {
    FILE* file = fopen(MENU_FILE, "w");
    if (file == NULL) {
        printf("Error opening menu file for writing.\n");
        return;
    }

    MenuItem* current = menuHead;
    while (current != NULL) {
        fprintf(file, "%d,%s,%.2f,%d\n", current->id, current->name, current->price, current->availability);
        current = current->next;
    }
    fclose(file);
    printf("Menu saved successfully.\n");
}

// Function to add a new menu item
void addMenuItem() {
    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
    newItem->id = nextMenuItemId++;
    printf("Enter item name: ");
    scanf("%s", newItem->name);
    printf("Enter item price: ");
    scanf("%f", &newItem->price);
    printf("Enter initial availability: \n");
    scanf("%d", &newItem->availability);
    newItem->next = NULL;

    if (menuHead == NULL) {
        menuHead = newItem;
    } else {
        MenuItem* current = menuHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newItem;
    }
    saveMenu();
    printf("Menu item added successfully (ID: %d).\n", newItem->id);
}

// Function to view the menu
void viewMenu() {
    if (menuHead == NULL) {
        printf("Menu is empty.\n");
        return;
    }
    printf("\n--- Menu ---\n");
    printf("ID\tName\t\tPrice\tAvailability\n");
    printf("----------------------------------------\n");
    MenuItem* current = menuHead;
    while (current != NULL) {
        printf("%d\t%-15s\t%.2f\t%d\n", current->id, current->name, current->price, current->availability);
        current = current->next;
    }
    printf("-------------\n");
}

// Function to find a menu item by ID
MenuItem* findMenuItem(int id) {
    MenuItem* current = menuHead;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to delete a menu item
void deleteMenuItem() {
    int id;
    printf("Enter the ID of the item to delete: ");
    scanf("%d", &id);

    MenuItem* current = menuHead;
    MenuItem* prev = NULL;

    if (current != NULL && current->id == id) {
        menuHead = current->next;
        free(current);
        saveMenu();
        printf("Menu item with ID %d deleted successfully.\n", id);
        return;
    }

    while (current != NULL && current->id != id) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Menu item with ID %d not found.\n", id);
        return;
    }

    prev->next = current->next;
    free(current);
    saveMenu();
    printf("Menu item with ID %d deleted successfully.\n", id);
}

// Function to update a menu item
void updateMenuItem() {
    int id;
    printf("Enter the ID of the item to update: ");
    scanf("%d", &id);

    MenuItem* itemToUpdate = findMenuItem(id);
    if (itemToUpdate == NULL) {
        printf("Menu item with ID %d not found.\n", id);
        return;
    }

    printf("Enter new name (leave blank to keep current: %s): ", itemToUpdate->name);
    char newName[50];
    scanf(" %[^\n]s", newName); // Read line with spaces
    if (strlen(newName) > 0) {
        strcpy(itemToUpdate->name, newName);
    }

    printf("Enter new price (leave -1 to keep current: %.2f): ", itemToUpdate->price);
    float newPrice;
    scanf("%f", &newPrice);
    if (newPrice != -1) {
        itemToUpdate->price = newPrice;
    }

    printf("Enter new availability (leave -1 to keep current: %d): ", itemToUpdate->availability);
    int newAvailability;
    scanf("%d", &newAvailability);
    if (newAvailability != -1) {
        itemToUpdate->availability = newAvailability;
    }

    saveMenu();
    printf("Menu item with ID %d updated successfully.\n", id);
}

// Function to load users from file
void loadUsers() {
    FILE *userFile = fopen("user.txt", "r");

if (userFile == NULL) {
    printf("Error opening user file, creating a new one...\n");
    userFile = fopen("user.txt", "w");
    if (userFile == NULL) {
        printf("Failed to create user file!\n");
        exit(1);
    }
}

    User* current = NULL;
    while (!feof(userFile)) {
        User* newUser = (User*)malloc(sizeof(User));
        if (fscanf(userFile, "%d,%[^,],%[^,],%f\n", &newUser->id, newUser->username, newUser->card_number, &newUser->balance) == 4) {
            newUser->next = NULL;
            if (userHead == NULL) {
                userHead = newUser;
                current = newUser;
            } else {
                current->next = newUser;
                current = newUser;
            }
            if (newUser->id >= nextUserId) {
                nextUserId = newUser->id + 1;
            }
        }
    }
    fclose(userFile);
    printf("Users loaded successfully.\n");
}

// Function to save users to file
void saveUsers() {
    FILE* file = fopen(USERS_FILE, "w");
    if (file == NULL) {
        printf("Error opening users file for writing.\n");
        return;
    }

    User* current = userHead;
    while (current != NULL) {
        fprintf(file, "%d,%s,%s,%.2f\n", current->id, current->username, current->card_number, current->balance);
        current = current->next;
    }
    fclose(file);
    printf("Users saved successfully.\n");
}

// Function to add a new user
void addUser() {
    User* newUser = (User*)malloc(sizeof(User));
    newUser->id = nextUserId++;
    printf("Enter username: ");
    scanf("%s", newUser->username);
    printf("Enter card number: ");
    scanf("%s", newUser->card_number);
    newUser->balance = CARD_BALANCE;
    newUser->next = NULL;

    if (userHead == NULL) {
        userHead = newUser;
    } else {
        User* current = userHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newUser;
    }
    saveUsers();
    printf("User added successfully (ID: %d, Balance: %.2f).\n", newUser->id, newUser->balance);
}

// Function to find a user by card number
User* findUserByCard(const char* cardNumber) {
    User* current = userHead;
    while (current != NULL) {
        if (strcmp(current->card_number, cardNumber) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function for admin login
bool adminLogin() {
    char enteredPin[20];
    printf("Enter admin PIN: ");
    scanf("%s", enteredPin);
    return (strcmp(enteredPin, ADMIN_PIN) == 0);
}

// Function for user login
User* userLogin() {
    char cardNumber[20];
    printf("Enter your card number: ");
    scanf("%s", cardNumber);
    return findUserByCard(cardNumber);
}

// Function to take order
void takeOrder(User* loggedInUser) {
    if (loggedInUser == NULL) {
        printf("Please log in first.\n");
        return;
    }

    int orderId;
    int quantity;
    float totalBill = 0;

    printf("\n--- Take Order ---\n");
    viewMenu();

    while (1) {
        printf("Enter item ID to order (0 to finish): ");
        scanf("%d", &orderId);
        if (orderId == 0) {
            break;
        }

        MenuItem* orderedItem = findMenuItem(orderId);
        if (orderedItem == NULL) {
            printf("Invalid item ID.\n");
            continue;
        }

        printf("Enter quantity for %s: ", orderedItem->name);
        scanf("%d", &quantity);

        if (quantity <= 0) {
            printf("Quantity must be greater than 0.\n");
            continue;
        }

        if (orderedItem->availability < quantity) {
            printf("%s is currently not available in that quantity (Available: %d).\n", orderedItem->name, orderedItem->availability);
            continue;
        }

        totalBill += orderedItem->price * quantity;
        orderedItem->availability -= quantity;
        saveMenu(); // Auto-update availability

        printf("Added %d x %s to your order. Current bill: %.2f\n", quantity, orderedItem->name, totalBill);
    }

    printf("\n--- Your Bill ---\n");
    printf("Total amount: %.2f\n", totalBill);

    if (loggedInUser->balance >= totalBill) {
        loggedInUser->balance -= totalBill;
        saveUsers();
        printf("Payment successful. Remaining balance: %.2f\n", loggedInUser->balance);
    } else {
        printf("Insufficient balance. Available balance: %.2f\n", loggedInUser->balance);
    }
}

// Function to display user balance
void displayBalance(User* loggedInUser) {
    if (loggedInUser == NULL) {
        printf("Please log in first.\n");
        return;
    }
    printf("Your current balance is: %.2f\n", loggedInUser->balance);
}

// Function to free the menu linked list
void freeMenu() {
    MenuItem* current = menuHead;
    MenuItem* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    menuHead = NULL;
}

// Function to free the user linked list
void freeUsers() {
    User* current = userHead;
    User* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    userHead = NULL;
}

int main() {
    loadMenu();
    loadUsers();

    int choice;
    User* loggedInUser = NULL;

    do {
        printf("\n--- Cafeteria System ---\n");
        if (loggedInUser != NULL) {
            printf("Logged in as User: %s\n", loggedInUser->username);
        }
        printf("1. Login (User)\n");
        printf("2. Login (Admin)\n");
        printf("3. View Menu\n");
        
            printf("4. Take Order\n");
            printf("5. View Balance\n");
            printf("6. Logout\n");
        
        printf("7. Add Menu Item (Admin)\n");
        printf("8. Delete Menu Item (Admin)\n");
        printf("9. Update Menu Item (Admin)\n");
        printf("10. Add User (Admin)\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                loggedInUser = userLogin();
                if (loggedInUser != NULL) {
                    printf("Login successful. Welcome, %s!\n", loggedInUser->username);
                } else {
                    printf("Login failed. Invalid card number.\n");
                }
                break;
            case 2:
                if (adminLogin()) {
                    printf("Admin login successful.\n");
                    int adminChoice;
                    do {
                        printf("\n--- Admin Menu ---\n");
                        printf("1. View Menu\n");
                        printf("2. Add Menu Item\n");
                        printf("3. Delete Menu Item\n");
                        printf("4. Update Menu Item\n");
                        printf("5. Add User\n");
                        printf("0. Back to Main Menu\n");
                        printf("Enter your choice: ");
                        scanf("%d", &adminChoice);
                        switch (adminChoice) {
                            case 1:
                                viewMenu();
                                break;
                            case 2:
                                addMenuItem();
                                break;
                            case 3:
                                deleteMenuItem();
                                break;
                            case 4:
                                updateMenuItem();
                                break;
                            case 5:
                                addUser();
                                break;
                            case 0:
                                printf("Returning to main menu.\n");
                                break;
                            default:
                                printf("Invalid admin choice.\n");
                        }
                    } while (adminChoice != 0);
                } else {
                    printf("Admin login failed. Incorrect PIN.\n");
                }
                break;
            case 3:
                viewMenu();
                break;
            case 4:
                takeOrder(loggedInUser);
                break;
            case 5:
                displayBalance(loggedInUser);
                break;
            case 6:
                if (loggedInUser != NULL) {
                    printf("Logged out, %s.\n", loggedInUser->username);
                    loggedInUser = NULL;
                } else {
                    printf("No user is currently logged in.\n");
                }
                break;
            case 7:
                printf("Please log in as admin to access this feature.\n");
                break;
            case 8:
                printf("Please log in as admin to access this feature.\n");
                break;
            case 9:
                printf("Please log in as admin to access this feature.\n");
                break;
            case 10:
                printf("Please log in as admin to access this feature.\n");
                break;
            case 0:
                printf("Exiting cafeteria system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    // Free allocated memory before exiting
    freeMenu();
    freeUsers();

    return 0;
}