#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
using namespace std;

class User {
protected:
    string name;
    string phoneNumber;
    string location;
public:
    User(string phone = "", string loc = "") : phoneNumber(phone), location(loc) {}
    void setName(string n) { name = n; }
    string getName() { return name; }
    string getLocation() { return location; }
    string getPhoneNumber() { return phoneNumber; }

    virtual void displayDetails() {
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "Location: " << location << endl;
    }
};

class Customer : public User {
private:
    int quantity10L = 0;
    int quantity20L = 0;
    int quantitySmallDisp = 0;
    int quantityLargeDisp = 0;
    const double price10L = 10;
    const double price20L = 20;
    const double priceSmallDisp = 25;
    const double priceLargeDisp = 50;
    double deliveryFee = 0;
    double totalCost = 0;
    double x, y;

public:
    Customer(string phone, string loc, double xCoord, double yCoord)
        : User(phone, loc), x(xCoord), y(yCoord) {}

    double getX() { return x; }
    double getY() { return y; }

    void selectWater() {
        int choice, qty;
        cout << "\nSelect item:\n";
        cout << "1. 10L Water\n2. 20L Water\n3. Small Dispenser\n4. Large Dispenser\n";
        cin >> choice;
        cout << "Enter the number of bottles you want: ";
        cin >> qty;
        switch (choice) {
            case 1: quantity10L = qty; break;
            case 2: quantity20L = qty; break;
            case 3: quantitySmallDisp = qty; break;
            case 4: quantityLargeDisp = qty; break;
            default: cout << "Invalid choice!\n";
        }
    }
    void setDeliveryFee(double fee) { deliveryFee = fee; }
    void calculateTotal() {
        totalCost = (quantity10L * price10L) +
                    (quantity20L * price20L) +
                    (quantitySmallDisp * priceSmallDisp) +
                    (quantityLargeDisp * priceLargeDisp) +
                    deliveryFee;
    }
    void displayOrder() {
        cout << "\n--- ORDER SUMMARY ---\n";
        cout << "Phone: " << phoneNumber << endl;
        cout << "Location: " << location << endl;
        cout << "10L: " << quantity10L << " bottles\n";
        cout << "20L: " << quantity20L << " bottles\n";
        cout << "Small Disp: " << quantitySmallDisp << " bottles\n";
        cout << "Large Disp: " << quantityLargeDisp << " bottles\n";
        cout << "Delivery Fee: " << fixed << setprecision(2) << deliveryFee << " Ksh\n";
        cout << "Total Cost: " << fixed << setprecision(2) << totalCost << " Ksh\n";
    }
    int getQuantity10L() { return quantity10L; }
    int getQuantity20L() { return quantity20L; }
    int getQuantitySmallDisp() { return quantitySmallDisp; }
    int getQuantityLargeDisp() { return quantityLargeDisp; }
    double getTotalCost() { return totalCost; }
};
class Rider : public User {
private:
    bool available;
    string currentOrder;
    double x, y;
public:
    int riderID;
    string vehicleType;
    double commissionEarned = 0.0;
    Rider(int id, string phone, string loc,
          string vehicle, double xCoord, double yCoord)
        : User(phone, loc),
          riderID(id),
          vehicleType(vehicle),
          available(true),
          currentOrder(""),
          x(xCoord),
          y(yCoord) {}
    double getX() { return x; }
    double getY() { return y; }
    void setAvailability(bool status) { available = status; }
    bool isAvailable() { return available; }
    void acceptOrder(string order, double commission) {
        if (available) {
            currentOrder = order;
            available = false;
            commissionEarned += commission;
            cout << "Rider accepted order.\n";
        }
    }
    void updateStatus(string status) {
        cout << "Order Status: " << status << endl;
        if (status == "Delivered") {
            currentOrder = "";
            available = true;
        }
    }
    void displayDetails() override {
        cout << "\n--- RIDER DETAILS ---\n";
        cout << "Rider ID: " << riderID << endl;
        cout << "Phone: " << phoneNumber << endl;
        cout << "Vehicle: " << vehicleType << endl;
        cout << "Total Commission Earned: "
             << commissionEarned << " Ksh\n";
    }
};
class Vendor : public User {
private:
    double x, y;
    bool available;
    double ratingSum = 0.0;
    int ratingCount = 0;
    double totalEarnings = 0.0;

public:
    int vendorID;
    string businessName;
    Vendor(int id, string phone, string loc, string name, double xCoord, double yCoord)
        : User(phone, loc), vendorID(id), businessName(name), x(xCoord), y(yCoord), available(true) {}
    void addEarnings(double earnings) { totalEarnings += earnings; }
    double getTotalEarnings() { return totalEarnings; }
    double getX() { return x; }
    double getY() { return y; }
    bool isAvailable() { return available; }
    void setOnline(bool status) { available = status; }
    void addRating(double rating) { ratingSum += rating; ratingCount++; }
    double getAverageRating() { return (ratingCount == 0) ? 0.0 : ratingSum / ratingCount; }
    void displayDetails() override {
        cout << "\n--- VENDOR DETAILS ---\n";
        cout << "Vendor ID: " << vendorID << endl;
        cout << "Vendor: " << businessName << endl;
        cout << "Phone: " << phoneNumber << endl;
        cout << "Average Rating: " << fixed << setprecision(2) << getAverageRating() << "\n";
        cout << "Total Earnings: " << totalEarnings << " Ksh\n";
    }
};
double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
double calculateDeliveryFee(double distance) {
    return distance * 10;
}
int calculateETAInMinutes(double distance, string vehicleType) {
    double speed = (vehicleType == "e-bike") ? 30.0 : 60.0;
    return static_cast<int>((distance / speed) * 60);
}
Vendor* findNearestVendor(Customer& cust, vector<Vendor>& vendors) {
    double minDist = 1e9;
    Vendor* nearest = nullptr;
    for (auto &v : vendors) {
        double dist = calculateDistance(cust.getX(), cust.getY(), v.getX(), v.getY());
        if (dist < minDist && v.isAvailable()) {
            minDist = dist;
            nearest = &v;
        }
    }
    return nearest;
}
Rider* findNearestRider(Vendor* vendor, vector<Rider>& riders) {
    double minDist = 1e9;
    Rider* nearest = nullptr;
    for(auto &r : riders) {
        if(r.isAvailable()) {
            double dist = calculateDistance(
                vendor->getX(),
                vendor->getY(),
                r.getX(),
                r.getY()
            );
            if(dist < minDist) {
                minDist = dist;
                nearest = &r;
            }
        }
    }
    return nearest;
}
double calculateRiderCommission(Customer& cust) {
    double commission = 10;
    commission += cust.getQuantity10L() * 5;
    commission += cust.getQuantity20L() * 7.5;
    commission += cust.getQuantitySmallDisp() * 7.5;
    commission += cust.getQuantityLargeDisp() * 10;
    double maxCommission = 0.45 * cust.getTotalCost();
    return min(commission, maxCommission);
}
double calculateVendorProfit(Customer& cust, double riderCommission) {
    double profit = cust.getTotalCost() - riderCommission;
    if (profit < 0.55 * cust.getTotalCost()) profit = 0.55 * cust.getTotalCost();
    return profit;
}
void processPayment(Customer& cust) {
    int paymentChoice;
    cout << "\n--- PAYMENT METHOD ---\n";
    cout << "1. Cash\n";
    cout << "2. M-Pesa\n";
    cout << "3. Credit Card\n";
    cout << "Choose payment method: ";
    cin >> paymentChoice;
    switch(paymentChoice) {
        case 1:
            cout << "\nPayment will occur after rider arrives with order.\n";
            cout << "Order processed successfully.\n";
            break;
        case 2: {
            string correctPin = "1234";
            string enteredPin;
            cout << "\nM-Pesa Number: " << cust.getPhoneNumber() << endl;
            while(true) {
                cout << "Enter M-Pesa PIN: ";
                cin >> enteredPin;

                if(enteredPin == correctPin) {

                    cout << "\nProcessing payment";

                    for(int i = 0; i < 5; i++) {
                        cout << ".";
                        cout.flush();
                        this_thread::sleep_for(chrono::seconds(1));
                    }
                    cout << "\n\nPayment Successful!\n";
                    cout << "Ksh " << fixed << setprecision(2)
                         << cust.getTotalCost()
                         << " deducted from M-Pesa account.\n";
                    cout << "Order processed successfully.\n";
                    break;
                }
                else {
                    cout << "Wrong PIN! Please enter correct PIN.\n";
                }
            }
            break;
        }
        case 3:
            cout << "\nCredit Card payment feature coming soon.\n";
            cout << "Order processed successfully.\n";
            break;
        default:
            cout << "Invalid payment option!\n";
    }
}
void loadRiders(vector<Rider>& riders) {
    ifstream file("riders.csv");
    if(!file) return;
    string line;
    while(getline(file, line)) {
        stringstream ss(line);
        string idStr, phone, loc, vehicle, commissionStr;
        getline(ss, idStr, ',');
        getline(ss, phone, ',');
        getline(ss, loc, ',');
        getline(ss, vehicle, ',');
        getline(ss, commissionStr, ',');
        try {
            int id = stoi(idStr);
            double commission = stod(commissionStr);
            for(auto &r : riders) {
                if(r.riderID == id) {
                    r.commissionEarned = commission;
                }
            }
        }
        catch(...) {
            cout << "Invalid rider data skipped.\n";
        }
    }
    file.close();
}
void saveRiders(vector<Rider>& riders) {
    ofstream file("riders.csv");
    for(auto &r : riders) {
        file << r.riderID << ","
             << r.getPhoneNumber() << ","
             << r.getLocation() << ","
             << r.vehicleType << ","
             << r.commissionEarned
             << "\n";
    }
    file.close();
}
string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}
void loadVendors(vector<Vendor>& vendors) {
    ifstream file("vendors.csv");
    if(!file) return;
    string line;
    while(getline(file, line)) {
        line = trim(line);
        if(line.empty()) continue; 
        stringstream ss(line);
        string idStr, phone, loc, name, earningsStr;
        if(!getline(ss, idStr, ',')) continue;
        if(!getline(ss, phone, ',')) continue;
        if(!getline(ss, loc, ',')) continue;
        if(!getline(ss, name, ',')) continue;
        if(!getline(ss, earningsStr, ',')) continue;
        try {
            int id = stoi(trim(idStr));
            double earnings = stod(trim(earningsStr));
            for(auto &v : vendors) {
                if(v.vendorID == id) {
                    v.addEarnings(earnings - v.getTotalEarnings());
                }
            }
        } catch(...) {
            cout << "Warning: Skipped malformed line in vendors.csv\n";
            continue;
        }
    }
}
void saveVendors(vector<Vendor>& vendors) {
    ofstream file("vendors.csv");
    for(auto &v : vendors) {
        file << v.vendorID << "," << v.getPhoneNumber() << "," << v.getLocation()
             << "," << v.businessName << "," << v.getTotalEarnings() << "\n";
    }
}
int main() {
    while(true){
    int role;
    cout << "Welcome to Maji Water Delivery System!\n";
    cout << "Select role:\n1. Customer\n2. Rider\n3. Vendor\n4. Exit\n";
    cin >> role;
   vector<Rider> riders = {
    Rider(101, "0798321453", "Central", "e-bike", 5, 5),
    Rider(102, "0712345678", "Juja Stage", "motorcycle", 3, 1),
    Rider(103, "0723456789", "Main gate", "e-bike", 1, 2),
    Rider(104, "0734567890", "Gachororo", "motorcycle", 2, 5),
    Rider(105, "0745678901", "Juja Square", "e-bike", 8, 6),
    Rider(106, "0756789012", "Gate B", "motorcycle", 4, 7),
    Rider(107, "0767890123", "Gate D", "e-bike", 7, 2),
    Rider(108, "0790231456", "Juja Hospital", "motorcycle", 9, 4)
};
    vector<Vendor> vendors = {
        Vendor(201, "0734897432", "A", "Vendor A", 2, 3),
        Vendor(202, "0771123564", "B", "Vendor B", 5, 1),
        Vendor(203, "0743223444", "C", "Vendor C", 7, 8),
        Vendor(204, "0788990011", "D", "Vendor D", 1, 9),
        Vendor(205, "0733295671", "E", "Vendor E", 4, 6),
        Vendor(206, "0706667788", "F", "Vendor F", 8, 2),
        Vendor(207, "0707778899", "G", "Vendor G", 6, 5),
        Vendor(208, "0767855213", "H", "Vendor H", 3, 4),
        Vendor(209, "0759990011", "I", "Vendor I", 9, 7),
        Vendor(210, "0766923415", "J", "Vendor J", 10, 5)
    };
    loadRiders(riders);
    loadVendors(vendors);
       if (role==4){
        cout<<"Exiting the system.Thank you for trusting Maji Water!!Goodbye!\n";
        break;
    }
    if(role == 1) { 
        string phoneNumber;
        cout << "Enter your M-pesa phone number: ";
        cin >> phoneNumber;
        vector<pair<string, pair<double, double>>> locations = {
            {"Main gate", {1, 2}}, {"Gachororo", {2, 5}}, {"Juja Stage", {3, 1}},
            {"Gate B", {4, 7}}, {"JKUAT Ndani", {5, 3}}, {"Juja Market", {6, 8}},
            {"Gate D", {7, 2}}, {"Juja Square", {8, 6}}, {"Juja Hospital", {9, 4}},
            {"Gate C", {10, 5}}
        };
        cout << "Choose your location:\n";
        for(int i=0;i<locations.size();i++)
            cout << i+1 << ". " << locations[i].first << endl;
        int choice; cin >> choice;
        Customer cust(phoneNumber, locations[choice-1].first,
                      locations[choice-1].second.first, locations[choice-1].second.second);
        bool orderConfirmed = false;
        double distance = 0;
        Vendor* nearestVendor = nullptr;
        while(!orderConfirmed) {
            cust.selectWater();
            nearestVendor = findNearestVendor(cust, vendors);
            if(nearestVendor) {
                distance = calculateDistance(cust.getX(), cust.getY(), nearestVendor->getX(), nearestVendor->getY());
                cust.setDeliveryFee(calculateDeliveryFee(distance));
                cust.calculateTotal();
                cust.displayOrder();
                processPayment(cust);
            }
            cout << "\nDo you want to proceed?\n1. Yes\n2. Edit\n3. Cancel\n";
            int decision; cin >> decision;
            if(decision == 1) orderConfirmed = true;
            else if(decision == 2) cout << "Editing your order...\n";
            else if(decision == 3) {
                cout << "Order canceled.\n";
                break;
            }
        }
       Rider* assignedRider = findNearestRider(nearestVendor, riders);
        if(nearestVendor && assignedRider) {
            double riderCommission = calculateRiderCommission(cust);
            double vendorProfit = calculateVendorProfit(cust, riderCommission);
            assignedRider->acceptOrder("Water Delivery", riderCommission);
            nearestVendor->addEarnings(vendorProfit);
            int etaMinutes = calculateETAInMinutes(distance, assignedRider->vehicleType);
            cout << "\nDistance: " << distance << " km\nDelivery Fee: "
                 << cust.getTotalCost() - ((cust.getQuantity10L()*10)+(cust.getQuantity20L()*20)) << " Ksh\nETA: "
                 << etaMinutes << " minutes\n";
            cout << "\n--- ASSIGNED RIDER ---\n";
            cout << "Phone: " << assignedRider->getPhoneNumber() << endl;
            cout << "Vehicle: " << assignedRider->vehicleType << endl;
            cout << "\n--- NEAREST VENDOR ---\n";
            cout << "Vendor: " << nearestVendor->businessName << endl;
            cout << "Phone: " << nearestVendor->getPhoneNumber() << endl;
            cout << "Average Rating: " << fixed << setprecision(2) << nearestVendor->getAverageRating() << endl;
            cust.displayOrder();
            double riderRating, vendorRating;
            cout << "Rate your rider (1-5): "; cin >> riderRating;
            cout << "Rate your vendor (1-5): "; cin >> vendorRating;
            assignedRider->updateStatus("Delivered");
            nearestVendor->addRating(vendorRating);
            cout << "\nThank you for using Maji Water!\n";
        }
    } else if(role == 2) { 
        int loginID;
        cout << "Enter your Rider ID: "; cin >> loginID;
        bool found = false;
        for(Rider &r : riders) {
            if(r.riderID == loginID) {
                r.displayDetails();
                found = true;
                break;
            }
        }
        if(!found) cout << "Rider ID not found!\n";

    } else if(role == 3) { 
        int loginID;
        cout << "Enter your Vendor ID: "; cin >> loginID;
        bool found = false;
        for(Vendor &v : vendors) {
            if(v.vendorID == loginID) {
                v.displayDetails();
                found = true;
                break;
            }
        }
        if(!found) cout << "Vendor ID not found!\n";
    }
    saveRiders(riders);
    saveVendors(vendors);
}
    return 0;
}
