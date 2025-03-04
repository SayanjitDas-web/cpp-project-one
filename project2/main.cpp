#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// Get current date in YYYY-MM-DD format
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string year = to_string(1900 + ltm->tm_year);
    string month = (ltm->tm_mon + 1 < 10) ? "0" + to_string(ltm->tm_mon + 1) : to_string(ltm->tm_mon + 1);
    string day = (ltm->tm_mday < 10) ? "0" + to_string(ltm->tm_mday) : to_string(ltm->tm_mday);
    return year + "-" + month + "-" + day;
}

// Struct to store food details
struct FoodItem {
    string name, category, expiryDate;
    int quantity;
};

// Comparator for sorting by expiry date
bool compareExpiry(FoodItem a, FoodItem b) {
    return a.expiryDate < b.expiryDate;
}

// Function to add food item
void addFood() {
    FoodItem item;
    cout << "Enter Food Name: ";
    cin >> item.name;
    cout << "Enter Category: ";
    cin >> item.category;
    cout << "Enter Quantity: ";
    cin >> item.quantity;
    cout << "Enter Expiry Date (YYYY-MM-DD): ";
    cin >> item.expiryDate;

    ofstream file("inventory.txt", ios::app);
    if (file) {
        file << item.name << " " << item.category << " " << item.quantity << " " << item.expiryDate << endl;
        cout << "✅ Food Item Added Successfully!\n";
    }
    file.close();
}

// Function to load inventory into a vector
vector<FoodItem> loadInventory() {
    vector<FoodItem> inventory;
    ifstream file("inventory.txt");
    if (!file) return inventory;

    FoodItem item;
    while (file >> item.name >> item.category >> item.quantity >> item.expiryDate) {
        inventory.push_back(item);
    }
    file.close();
    return inventory;
}

// Function to display inventory sorted by expiry date
void displayInventory() {
    vector<FoodItem> inventory = loadInventory();
    if (inventory.empty()) {
        cout << "⚠️ Inventory is empty!\n";
        return;
    }

    sort(inventory.begin(), inventory.end(), compareExpiry); // Sorting by expiry date

    cout << "\n📦 Food Inventory (Sorted by Expiry Date):\n";
    for (auto &item : inventory) {
        cout << "🍽️ " << item.name << " | " << item.category
             << " | Qty: " << item.quantity
             << " | Expiry: " << item.expiryDate << endl;
    }
}

// Function to search for a food item
void searchFood() {
    vector<FoodItem> inventory = loadInventory();
    if (inventory.empty()) {
        cout << "⚠️ Inventory is empty!\n";
        return;
    }
    
    string searchName;
    cout << "Enter Food Name to Search: ";
    cin >> searchName;
    
    bool found = false;
    for (auto &item : inventory) {
        if (item.name == searchName) {
            cout << "✅ Found: " << item.name << " | Category: " << item.category 
                 << " | Qty: " << item.quantity << " | Expiry: " << item.expiryDate << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << "❌ Item not found in inventory.\n";
}

// Function to remove expired food items
void removeExpiredItems() {
    vector<FoodItem> inventory = loadInventory();
    if (inventory.empty()) {
        cout << "⚠️ Inventory is empty!\n";
        return;
    }

    string currentDate = getCurrentDate();
    ofstream file("inventory.txt");
    bool found = false;

    for (auto &item : inventory) {
        if (item.expiryDate < currentDate) {
            cout << "❌ Removing expired item: " << item.name << " (Expired on " << item.expiryDate << ")\n";
            found = true;
        } else {
            file << item.name << " " << item.category << " " << item.quantity << " " << item.expiryDate << endl;
        }
    }
    file.close();

    if (!found) cout << "✅ No expired items found.\n";
}

// Function to delete a specific food item
void deleteFood() {
    vector<FoodItem> inventory = loadInventory();
    if (inventory.empty()) {
        cout << "⚠️ Inventory is empty!\n";
        return;
    }

    string delName;
    cout << "Enter Food Name to Delete: ";
    cin >> delName;

    ofstream file("inventory.txt");
    bool found = false;
    
    for (auto &item : inventory) {
        if (item.name == delName) {
            cout << "❌ Deleting " << item.name << " from inventory.\n";
            found = true;
        } else {
            file << item.name << " " << item.category << " " << item.quantity << " " << item.expiryDate << endl;
        }
    }
    file.close();

    if (!found) cout << "❌ Item not found.\n";
}

// Main menu
int main() {
    int choice;
    while (true) {
        cout << "\n📦 Smart Food Inventory System\n";
        cout << "1️⃣ Add Food Item\n";
        cout << "2️⃣ Display Inventory (Sorted by Expiry)\n";
        cout << "3️⃣ Search Food Item\n";
        cout << "4️⃣ Remove Expired Items\n";
        cout << "5️⃣ Delete a Food Item\n";
        cout << "6️⃣ Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addFood(); break;
            case 2: displayInventory(); break;
            case 3: searchFood(); break;
            case 4: removeExpiredItems(); break;
            case 5: deleteFood(); break;
            case 6: cout << "🚀 Exiting..."; return 0;
            default: cout << "❌ Invalid Choice! Try again.\n";
        }
    }
}
