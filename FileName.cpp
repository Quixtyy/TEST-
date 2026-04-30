#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <stdexcept>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


class OutOfStockException : public runtime_error {
public:
    explicit OutOfStockException(const string& item)
        : runtime_error("Out of stock") {
    }
};


struct Order {
    string product;
    int amount;
};


void processOrders(map<string, int>& stock, queue<Order>& orders) {
    cout << "Processing Queue\n";
    while (!orders.empty()) {
        Order current = orders.front();
        orders.pop();

        try {
          
            if (stock.find(current.product) == stock.end() || stock[current.product] < current.amount) {
                throw OutOfStockException(current.product);
            }

            stock[current.product] -= current.amount;
            cout << "Processed: " << current.product << " (Qty: " << current.amount << ")\n";
        }
        catch (const OutOfStockException& e) {
            cerr << "Skipped: " << e.what() << endl;
        }
    }
}


int main() {
  
    map<string, int> stock;
    queue<Order> orders;

  
    try {
        ifstream inputFile("inventory.json");
        if (!inputFile.is_open())
            throw runtime_error("Could not find inventory.json");

        json j;
        inputFile >> j;
        for (auto it = j.begin(); it != j.end(); ++it) {
            stock[it.key()] = it.value().get<int>();
        }
        cout << "Inventory loaded from JSON.\n";
    }
    catch (const exception& e) {
        cerr << "JSON Error: " << e.what() << endl;
        return 1;
    }

    
    orders.push({ "apple", 5 });
    orders.push({ "bread", 100 }); 
    orders.push({ "milk", 2 });

    processOrders(stock, orders);

   
    try {
        ofstream outFile("updated_stock.csv");
        if (!outFile.is_open()) throw runtime_error("Failed to create CSV");

        outFile << "Product,Quantity\n";
        for (const auto& pair : stock) {
            outFile << pair.first << "," << pair.second << "\n";
        }
        cout << "\nStock updated in 'updated_stock.csv'.\n";
    }
    catch (const exception& e) {
        cerr << "Export Error: " << e.what() << endl;
    }

    return 0;
}