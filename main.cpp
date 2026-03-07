// 10 Classes: User, Buyer, Seller, Admin, Vehicle, Car, Bike, VehicleListing, Marketplace, Status enum

#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int MAX_LISTINGS = 50;
const double MIN_PRICE = 50000.0;

enum Status { PENDING, APPROVED, SOLD };

class Vehicle {
protected:
    const string brand;
    string model;
    int year;
    double price;
    int mileage;
    
public:
    // Constructors
    Vehicle() : brand("Unknown"), year(0), price(0), mileage(0) {}
    Vehicle(string b, string m, int y, double p, int mi) 
        : brand(b), model(m), year(y), price(p), mileage(mi) {}
    
    // Getters (const functions)
    string getBrand() const { return brand; }
    double getPrice() const { return price; }
    int getYear() const { return year; }
    
    virtual void display() const = 0;  // Pure virtual
    bool isAffordable(double budget) const { return price <= budget; }
    virtual ~Vehicle() {}
};

class Car : public Vehicle {
    string color;
    int seats;
public:
    Car() : Vehicle(), color("White"), seats(4) {}
    Car(string b, string m, int y, double p, int mi, string c, int s)
        : Vehicle(b, m, y, p, mi), color(c), seats(s) {}
    
    void display() const override {
        cout << brand << " " << model << " " << year 
             << " - PKR " << price << " - " << mileage << "km" << endl;
    }
};

class Bike : public Vehicle {
    int engineCC;
public:
    Bike() : Vehicle(), engineCC(125) {}
    Bike(string b, string m, int y, double p, int mi, int cc)
        : Vehicle(b, m, y, p, mi), engineCC(cc) {}
    
    void display() const override {
        cout << brand << " " << model << " " << engineCC 
             << "cc - PKR " << price << endl;
    }
};

class User {
public:
    string name;
    string email;
    string phone;
    
    User() {}
    User(string n, string e, string p) : name(n), email(e), phone(p) {}
    
    virtual void sendMessage(string toUser) const = 0;
    virtual ~User() {}
};

class Buyer : public User {
    vector<int> favorites;  // Aggregation
public:
    Buyer(string n, string e, string p) : User(n, e, p) {}
    
    void addFavorite(int listingId) { favorites.push_back(listingId); }
    void viewFavorites() const {
        cout << name << "'s favorites: ";
        for(int id : favorites) cout << id << " ";
        cout << endl;
    }
    
    void sendMessage(string toUser) const override {
        cout << name << " sent message to " << toUser << endl;
    }
};

class Seller : public User {
public:
    double rating;
    Seller(string n, string e, string p) : User(n, e, p), rating(0.0) {}
    
    void sendMessage(string toUser) const override {
        cout << name << " replied to " << toUser << endl;
    }
};

class VehicleListing {
private:
    static int nextId;
    const int id;  // Constant data member
    Status status;
    Vehicle* vehicle;     // Composition (♦)
    Seller* seller;       // Aggregation (◇)
    
public:
    // Constructors
    VehicleListing() : id(nextId++), status(PENDING), vehicle(nullptr), seller(nullptr) {}
    VehicleListing(Vehicle* v, Seller* s) : id(nextId++), status(PENDING), vehicle(v), seller(s) {}
    
    // Member functions
    void approve() { status = APPROVED; }
    void markSold() { status = SOLD; }
    bool isApproved() const { return status == APPROVED; }
    void display() const {
        cout << "ID: " << id << " - ";
        if(vehicle) vehicle->display();
    }
    
    int getId() const { return id; }
};

int VehicleListing::nextId = 1;  // Static member

class Admin {
    string username;
    string password;
public:
    Admin(string u, string p) : username(u), password(p) {}
    
    void approveListing(VehicleListing* listing) {
        listing->approve();
        cout << "Admin approved listing" << endl;
    }
    
    void removeListing(VehicleListing* listing) {
        cout << "Admin removed listing" << endl;
    }
};

class Marketplace {
public:
    static VehicleListing* listings[MAX_LISTINGS];  // Array of objects
    static int listingCount;
    
    static void addListing(Vehicle* v, Seller* s) {
        if(listingCount < MAX_LISTINGS) {
            listings[listingCount++] = new VehicleListing(v, s);
        }
    }
    
    static void displayAllApproved() {
        cout << "\n=== APPROVED LISTINGS ===" << endl;
        for(int i = 0; i < listingCount; i++) {
            if(listings[i] && listings[i]->isApproved()) {
                listings[i]->display();
            }
        }
    }
    
    static void searchByBrand(const string& brand) {
        cout << "\n=== SEARCH: " << brand << " ===" << endl;
        for(int i = 0; i < listingCount; i++) {
            if(listings[i] && listings[i]->isApproved() && 
               listings[i]->vehicle->getBrand() == brand) {
                listings[i]->display();
            }
        }
    }
};

VehicleListing* Marketplace::listings[MAX_LISTINGS] = {nullptr};
int Marketplace::listingCount = 0;

// Main demo
int main() {
    // Create users
    Seller* seller1 = new Seller("Ahmed Khan", "ahmed@email.com", "03001234567");
    Buyer* buyer1 = new Buyer("Ali Raza", "ali@email.com", "03009876543");
    
    // Create vehicles
    Car* corolla = new Car("Toyota", "Corolla", 2023, 3500000, 15000, "White", 5);
    Bike* honda = new Bike("Honda", "CB150", 2024, 450000, 5000, 150);
    
    // Add listings
    Marketplace::addListing(corolla, seller1);
    Marketplace::addListing(honda, seller1);
    
    // Admin approval
    Admin admin("admin", "1234");
    admin.approveListing(Marketplace::listings[0]);
    admin.approveListing(Marketplace::listings[1]);
    
    // Demo functionality
    Marketplace::displayAllApproved();
    Marketplace::searchByBrand("Toyota");
    
    buyer1->addFavorite(1);
    buyer1->viewFavorites();
    
    buyer1->sendMessage("Ahmed Khan");
    seller1->sendMessage("Ali Raza");
    
    return 0;
}
