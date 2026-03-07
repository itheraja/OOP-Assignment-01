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

    Vehicle() : brand("Unknown"), model(""), year(0), price(0), mileage(0) {}

    Vehicle(string b, string m, int y, double p, int mi)
        : brand(b), model(m), year(y), price(p), mileage(mi) {}

    string getBrand() const { return brand; }
    double getPrice() const { return price; }
    int getYear() const { return year; }
    int getMileage() const { return mileage; }

    void setPrice(double p) { price = p; }

    // Each vehicle type defines its own display format
    virtual void display() const = 0;

    bool isAffordable(double budget) const {
        return price <= budget;
    }

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

        cout << brand << " " << model << " "
             << year << " | Seats: " << seats
             << " | Mileage: " << mileage
             << " km | PKR " << price << endl;
    }
};

class Bike : public Vehicle {

    int engineCC;

public:

    Bike() : Vehicle(), engineCC(125) {}

    Bike(string b, string m, int y, double p, int mi, int cc)
        : Vehicle(b, m, y, p, mi), engineCC(cc) {}

    void display() const override {

        cout << brand << " " << model
             << " " << engineCC << "cc"
             << " | PKR " << price << endl;
    }
};

class User {

private:
    string name_;
    string email_;
    string phone_;
    int userId_;
    double balance_;

public:

    User() {}

    User(string n, string e, string p, int id, double bal)
        : name_(n), email_(e), phone_(p), userId_(id), balance_(bal) {}

    void setName(string n) { name_ = n; }
    void setEmail(string e) { email_ = e; }
    void setPhone(string p) { phone_ = p; }

    string getName() const { return name_; }

    virtual void sendMessage(string toUser) const = 0;

    virtual ~User() {}
};

class Buyer : public User {

private:
    vector<int> favorites;

public:

    Buyer(string n, string e, string p, int id, double bal)
        : User(n, e, p, id, bal) {}

    void addFavorite(int listingId) {
        favorites.push_back(listingId);
    }

    void removeFavorite(int id) {

        for (auto it = favorites.begin(); it != favorites.end(); ++it) {
            if (*it == id) {
                favorites.erase(it);
                break;
            }
        }
    }

    void viewFavorites() const {

        cout << "Favorite listings: ";

        for (int id : favorites)
            cout << id << " ";

        cout << endl;
    }

    void sendMessage(string toUser) const override {

        cout << getName() << " sent a message to "
             << toUser << endl;
    }
};

class Seller : public User {

public:

    double rating;

    Seller(string n, string e, string p, int id, double bal)
        : User(n, e, p, id, bal), rating(0.0) {}

    void sendMessage(string toUser) const override {

        cout << getName() << " replied to "
             << toUser << endl;
    }
};

class VehicleListing {

private:

    static int nextId;

    const int id;

    Status status;

    Vehicle* const vehicle;
    Seller* seller;

    const string location = "Karachi";

public:

    VehicleListing(Vehicle* v, Seller* s)
        : id(nextId++), status(PENDING), vehicle(v), seller(s) {}

    VehicleListing(const VehicleListing& other)
        : id(nextId++), status(other.status),
          vehicle(new Car(*dynamic_cast<Car*>(other.vehicle))),
          seller(other.seller) {}

    void approve() { status = APPROVED; }

    void markSold() { status = SOLD; }

    bool isApproved() const { return status == APPROVED; }

    int getId() const { return id; }

    Vehicle* getVehicle() const { return vehicle; }

    void display() const {

        cout << "Listing ID: " << id
             << " | Location: " << location << endl;

        if (vehicle)
            vehicle->display();
    }

    bool matchesFilter(string brand, int minYear,
                       double maxPrice, int maxMileage) const {

        return vehicle->getBrand() == brand &&
               vehicle->getYear() >= minYear &&
               vehicle->getPrice() <= maxPrice &&
               vehicle->getMileage() <= maxMileage;
    }
};

int VehicleListing::nextId = 1;

class Admin {

    string username;
    string password;

public:

    Admin(string u, string p) : username(u), password(p) {}

    void approveListing(VehicleListing* listing) {

        listing->approve();

        cout << "Admin approved listing "
             << listing->getId() << endl;
    }
};

class Marketplace {

public:

    static VehicleListing* listings[MAX_LISTINGS];
    static int listingCount;

    static void addListing(Vehicle* v, Seller* s) {

        if (listingCount < MAX_LISTINGS) {

            listings[listingCount++] =
                new VehicleListing(v, s);
        }
    }

    static void deleteListing(int id) {

        for (int i = 0; i < listingCount; i++) {

            if (listings[i] && listings[i]->getId() == id) {

                delete listings[i];
                listings[i] = nullptr;

                cout << "Listing deleted." << endl;
                break;
            }
        }
    }

    static void updatePrice(int id, double newPrice) {

        for (int i = 0; i < listingCount; i++) {

            if (listings[i] &&
                listings[i]->getId() == id) {

                listings[i]->getVehicle()
                    ->setPrice(newPrice);

                cout << "Price updated." << endl;
            }
        }
    }

    static void displayAllApproved() {

        cout << "\nApproved Listings\n";

        for (int i = 0; i < listingCount; i++) {

            if (listings[i] &&
                listings[i]->isApproved()) {

                listings[i]->display();
            }
        }
    }
};

VehicleListing* Marketplace::listings[MAX_LISTINGS] = { nullptr };
int Marketplace::listingCount = 0;

int main() {

    Seller* seller1 =
        new Seller("Ahmed Khan",
                   "ahmed@email.com",
                   "03001234567",
                   1,
                   100000);

    Buyer* buyer1 =
        new Buyer("Ali Raza",
                  "ali@email.com",
                  "03009876543",
                  2,
                  50000);

    Car* corolla =
        new Car("Toyota","Corolla",
                2023,3500000,
                15000,"White",5);

    Bike* honda =
        new Bike("Honda","CB150",
                 2024,450000,
                 5000,150);

    Marketplace::addListing(corolla, seller1);
    Marketplace::addListing(honda, seller1);

    Admin admin("admin","1234");

    admin.approveListing(Marketplace::listings[0]);
    admin.approveListing(Marketplace::listings[1]);

    Marketplace::displayAllApproved();

    buyer1->addFavorite(1);
    buyer1->viewFavorites();

    buyer1->sendMessage("Ahmed Khan");
    seller1->sendMessage("Ali Raza");

    Marketplace::updatePrice(1,3400000);

    return 0;
}
