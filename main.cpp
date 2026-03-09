/*
 * ============================================================
 *   CAR MARKETPLACE SYSTEM
 * ============================================================
 * CLASSES (10):
 *  1. Address    2. Vehicle (abstract)  3. Car   4. Bike
 *  5. User (abstract)  6. Seller  7. Buyer  8. Admin
 *  9. Message   10. Marketplace
 *
 * COMPOSITION  : Marketplace owns Car[] & Bike[] (array of objects)
 *                User        owns Address object
 *                Buyer       owns Message[] inbox
 * AGGREGATION  : Buyer  stores Vehicle* favorites (external refs)
 *                Seller stores Vehicle* listings  (external refs)
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

const int MAX_LISTINGS   = 100;
const int MAX_USERS      = 50;
const int MAX_FAVORITES  = 20;
const int MAX_MESSAGES   = 30;
const int MAX_SELLER_ADS = 20;

// ═══════════════════════════════════
// CLASS 1: Address
// Used via COMPOSITION inside User
// Default: address unknown at registration
// Parameterized: full address provided
// Copy: cloning user profiles
// ═══════════════════════════════════
class Address {
private:
    string city, province, country, street;
    int postalCode;
public:
    Address() : city("Unknown"), province("Unknown"),
                country("Pakistan"), street("N/A"), postalCode(0) {}

    Address(string c, string pr, string co, string st, int pc)
        : city(c), province(pr), country(co), street(st), postalCode(pc) {}

    Address(const Address& o)
        : city(o.city), province(o.province), country(o.country),
          street(o.street), postalCode(o.postalCode) {}

    string getCity()     const { return city; }
    string getProvince() const { return province; }
    string getCountry()  const { return country; }
    string getStreet()   const { return street; }
    int    getPostal()   const { return postalCode; }
    void   setCity(string c)    { city = c; }
    void   setProvince(string p){ province = p; }
    void   setStreet(string s)  { street = s; }
    void   setPostal(int pc)    { postalCode = pc; }

    // CONSTANT FUNCTION: returns formatted string, no state change
    string getFullAddress() const {
        return street + ", " + city + ", " + province + ", " + country;
    }
    bool isSameCity(const Address& o) const { return city == o.city; }

    void display() const {
        cout << street << ", " << city << ", " << province
             << " - " << postalCode;
    }
    void print() const {
        cout << "  Address  : "; display(); cout << "\n";
    }
};

// ═══════════════════════════════════
// CLASS 2: Vehicle (Abstract Base)
// Parameterized: all listings need core specs
// Default: needed for arrays in derived classes
// Copy: duplicating listings
//
// STATIC: totalVehiclesCreated
//   Reason: platform-wide count, belongs to the
//   class concept, not any individual object
// ═══════════════════════════════════
class Vehicle {
protected:
    int    listingID;
    string brand, model;
    int    year;
    double price;
    int    mileage;
    string registrationNumber;  // no public setter = logically const
    bool   isApproved, isActive;
    static int totalVehiclesCreated;

public:
    Vehicle() : listingID(0), brand(""), model(""), year(2020),
                price(0), mileage(0), registrationNumber(""),
                isApproved(false), isActive(false) {}

    Vehicle(int id, string br, string mo, int yr, double pr,
            int mi, string reg)
        : listingID(id), brand(br), model(mo), year(yr),
          price(pr), mileage(mi), registrationNumber(reg),
          isApproved(false), isActive(true)
    { totalVehiclesCreated++; }

    Vehicle(const Vehicle& v)
        : listingID(v.listingID), brand(v.brand), model(v.model),
          year(v.year), price(v.price), mileage(v.mileage),
          registrationNumber(v.registrationNumber),
          isApproved(v.isApproved), isActive(v.isActive)
    { totalVehiclesCreated++; }

    virtual ~Vehicle() {}

    int    getID()       const { return listingID; }
    string getBrand()    const { return brand; }
    string getModel()    const { return model; }
    int    getYear()     const { return year; }
    double getPrice()    const { return price; }
    int    getMileage()  const { return mileage; }
    string getRegNo()    const { return registrationNumber; }
    bool   getApproved() const { return isApproved; }
    bool   getActive()   const { return isActive; }

    void setPrice(double p)  { price = p; }
    void setMileage(int m)   { mileage = m; }
    void setBrand(string b)  { brand = b; }
    void setModel(string m)  { model = m; }
    void approve()    { isApproved = true; }
    void deactivate() { isActive = false; }
    void activate()   { isActive = true; }

    // STATIC FUNCTION: class-wide query, no per-object state needed
    static int getTotalVehiclesCreated() { return totalVehiclesCreated; }

    // CONSTANT FUNCTION: read-only formatted summary
    string getSummary() const {
        return "[#" + to_string(listingID) + "] " + brand + " " + model
             + " (" + to_string(year) + ") PKR "
             + to_string((long long)price) + " | "
             + to_string(mileage) + "km";
    }

    // CONSTANT FUNCTION: search filter check
    bool matchesFilter(string fb, string fm, double lo, double hi,
                       int ylo, int yhi, int mxMi) const {
        if (!fb.empty() && brand  != fb) return false;
        if (!fm.empty() && model  != fm) return false;
        if (price < lo  || price  > hi)  return false;
        if (year  < ylo || year   > yhi) return false;
        if (mileage > mxMi)              return false;
        return true;
    }

    virtual void   displayInfo() const = 0;
    virtual string getType()     const = 0;
};
int Vehicle::totalVehiclesCreated = 0;

// ═══════════════════════════════════
// CLASS 3: Car (IS-A Vehicle)
// Default: for array storage
// Parameterized: all car specs at listing time
// Copy: duplicate listings
//
// featureTag has no setter = logically immutable
// after construction (simulates const data member)
// ═══════════════════════════════════
class Car : public Vehicle {
private:
    string fuelType, transmission, color, featureTag;
    int engineCC, numDoors;
public:
    Car() : Vehicle(), fuelType("Petrol"), transmission("Manual"),
            color("White"), featureTag("Standard"), engineCC(1000), numDoors(4) {}

    Car(int id, string br, string mo, int yr, double pr, int mi,
        string reg, string fuel, string trans, int eng,
        string col, int doors, string tag = "Standard")
        : Vehicle(id, br, mo, yr, pr, mi, reg),
          fuelType(fuel), transmission(trans), color(col),
          featureTag(tag), engineCC(eng), numDoors(doors) {}

    Car(const Car& c)
        : Vehicle(c), fuelType(c.fuelType), transmission(c.transmission),
          color(c.color), featureTag(c.featureTag),
          engineCC(c.engineCC), numDoors(c.numDoors) {}

    string getFuel()         const { return fuelType; }
    string getTransmission() const { return transmission; }
    string getColor()        const { return color; }
    string getFeatureTag()   const { return featureTag; }  // no setter
    int    getEngineCC()     const { return engineCC; }
    int    getNumDoors()     const { return numDoors; }

    void setFuel(string f)         { fuelType = f; }
    void setTransmission(string t) { transmission = t; }
    void setColor(string c)        { color = c; }

    string getType() const override { return "Car"; }

    void displayInfo() const override {
        cout << "┌──────────────────────────────────────────┐\n";
        cout << "│  CAR #" << listingID
             << (isApproved ? " [APPROVED]" : " [PENDING]") << "\n";
        cout << "├──────────────────────────────────────────┤\n";
        cout << "│  Brand       : " << brand         << "\n";
        cout << "│  Model       : " << model         << "\n";
        cout << "│  Year        : " << year          << "\n";
        cout << "│  Price (PKR) : " << (long long)price << "\n";
        cout << "│  Mileage     : " << mileage << " km\n";
        cout << "│  Fuel        : " << fuelType      << "\n";
        cout << "│  Transmission: " << transmission  << "\n";
        cout << "│  Engine      : " << engineCC << " cc\n";
        cout << "│  Color       : " << color         << "\n";
        cout << "│  Doors       : " << numDoors      << "\n";
        cout << "│  Reg. No.    : " << registrationNumber << "\n";
        cout << "│  Tag         : " << featureTag    << "\n";
        cout << "└──────────────────────────────────────────┘\n";
    }

    double estimateTax()   const { return price * 0.17; }
    bool   isAutomatic()   const { return transmission == "Automatic"; }
    bool   isElectric()    const { return fuelType == "Electric"; }
};

// ═══════════════════════════════════
// CLASS 4: Bike (IS-A Vehicle)
// Default: empty slot in array
// Parameterized: bike listing with specs
// Copy: duplicate listings
// ═══════════════════════════════════
class Bike : public Vehicle {
private:
    string bikeType, fuelType, condition;
    int    engineCC;
    bool   hasABS;
public:
    Bike() : Vehicle(), bikeType("Standard"), fuelType("Petrol"),
             condition("Used"), engineCC(125), hasABS(false) {}

    Bike(int id, string br, string mo, int yr, double pr, int mi,
         string reg, string bt, int eng, bool abs, string fuel, string cond)
        : Vehicle(id, br, mo, yr, pr, mi, reg),
          bikeType(bt), fuelType(fuel), condition(cond),
          engineCC(eng), hasABS(abs) {}

    Bike(const Bike& b)
        : Vehicle(b), bikeType(b.bikeType), fuelType(b.fuelType),
          condition(b.condition), engineCC(b.engineCC), hasABS(b.hasABS) {}

    string getBikeType()  const { return bikeType; }
    string getFuel()      const { return fuelType; }
    string getCondition() const { return condition; }
    int    getEngineCC()  const { return engineCC; }
    bool   getHasABS()    const { return hasABS; }

    void setBikeType(string t)  { bikeType = t; }
    void setCondition(string c) { condition = c; }
    void setHasABS(bool a)      { hasABS = a; }

    string getType() const override { return "Bike"; }

    void displayInfo() const override {
        cout << "┌──────────────────────────────────────────┐\n";
        cout << "│  BIKE #" << listingID
             << (isApproved ? " [APPROVED]" : " [PENDING]") << "\n";
        cout << "├──────────────────────────────────────────┤\n";
        cout << "│  Brand    : " << brand    << "\n";
        cout << "│  Model    : " << model    << "\n";
        cout << "│  Year     : " << year     << "\n";
        cout << "│  Price    : PKR " << (long long)price << "\n";
        cout << "│  Mileage  : " << mileage << " km\n";
        cout << "│  Type     : " << bikeType << "\n";
        cout << "│  Engine   : " << engineCC << " cc\n";
        cout << "│  ABS      : " << (hasABS ? "Yes" : "No") << "\n";
        cout << "│  Fuel     : " << fuelType  << "\n";
        cout << "│  Condition: " << condition << "\n";
        cout << "│  Reg. No. : " << registrationNumber << "\n";
        cout << "└──────────────────────────────────────────┘\n";
    }

    bool   isCityCommuter()    const { return engineCC <= 150; }
    double monthlyFuelCost()   const { return (1000.0 / 40.0) * 300.0; }
    double estimateResale()    const { return price * 0.75; }
};

// ═══════════════════════════════════
// CLASS 9: Message
// Default: empty inbox slot
// Parameterized: real message
// Copy: store in inbox array
// timestamp has no setter = logically immutable
// ═══════════════════════════════════
class Message {
private:
    int    messageID, listingID;
    string senderName, receiverName, content, timestamp;
    bool   isRead;
public:
    Message() : messageID(0), listingID(0), senderName(""),
                receiverName(""), content(""), timestamp("N/A"), isRead(false) {}

    Message(int id, string sn, string rn, string ct, int lid, string ts)
        : messageID(id), listingID(lid), senderName(sn),
          receiverName(rn), content(ct), timestamp(ts), isRead(false) {}

    Message(const Message& m)
        : messageID(m.messageID), listingID(m.listingID),
          senderName(m.senderName), receiverName(m.receiverName),
          content(m.content), timestamp(m.timestamp), isRead(m.isRead) {}

    int    getMessageID()  const { return messageID; }
    string getSender()     const { return senderName; }
    string getReceiver()   const { return receiverName; }
    string getContent()    const { return content; }
    int    getListingID()  const { return listingID; }
    bool   getIsRead()     const { return isRead; }
    string getTimestamp()  const { return timestamp; } // no setter

    void markRead()            { isRead = true; }
    void setContent(string c)  { content = c; }

    bool isForUser(const string& u)  const { return receiverName == u; }
    bool isFromUser(const string& u) const { return senderName == u; }

    void display() const {
        cout << "  [MSG #" << messageID << "] "
             << (isRead ? "" : "[NEW] ")
             << "From: " << senderName
             << " | Listing #" << listingID << "\n";
        cout << "  Content: " << content << "\n";
        cout << "  Sent   : " << timestamp << "\n";
    }
};

// ═══════════════════════════════════
// CLASS 5: User (Abstract Base)
// OWNS Address (COMPOSITION)
// Default: guest user
// Parameterized: registered user
// Copy: clone profiles
//
// STATIC: totalUsers
//   Reason: one counter for ALL users
//   (platform-wide, not per-object)
// ═══════════════════════════════════
class User {
protected:
    int     userID;
    string  username, email, phoneNumber;
    Address address;   // COMPOSITION
    bool    isVerified;
    static int totalUsers;
public:
    User() : userID(0), username("Guest"), email(""),
             phoneNumber(""), address(), isVerified(false)
    { totalUsers++; }

    User(int id, string un, string em, string ph, Address ad)
        : userID(id), username(un), email(em),
          phoneNumber(ph), address(ad), isVerified(false)
    { totalUsers++; }

    User(const User& u)
        : userID(u.userID), username(u.username), email(u.email),
          phoneNumber(u.phoneNumber), address(u.address),
          isVerified(u.isVerified)
    { totalUsers++; }

    virtual ~User() {}

    int    getUserID()   const { return userID; }
    string getUsername() const { return username; }
    string getEmail()    const { return email; }
    string getPhone()    const { return phoneNumber; }
    bool   getVerified() const { return isVerified; }
    const Address& getAddress() const { return address; }

    void setEmail(string e)    { email = e; }
    void setPhone(string p)    { phoneNumber = p; }
    void setAddress(Address a) { address = a; }
    void verify()              { isVerified = true; }

    // STATIC FUNCTION: class-wide user count
    static int getTotalUsers() { return totalUsers; }

    virtual string getRole() const = 0;

    // CONSTANT FUNCTION: display user info (read-only)
    void displayBasicInfo() const {
        cout << "  User ID  : " << userID    << "\n";
        cout << "  Username : " << username  << "\n";
        cout << "  Email    : " << email     << "\n";
        cout << "  Phone    : " << phoneNumber << "\n";
        cout << "  Role     : " << getRole() << "\n";
        cout << "  Verified : " << (isVerified ? "Yes" : "No") << "\n";
        address.print();
    }

    bool isSameCityAs(const User& o) const {
        return address.isSameCity(o.address);
    }
};
int User::totalUsers = 0;

// ═══════════════════════════════════
// CLASS 7: Buyer (IS-A User)
// AGGREGATES Vehicle* favorites
// COMPOSES Message[] inbox
// Parameterized: buyer always has budget/preference
// ═══════════════════════════════════
class Buyer : public User {
private:
    Vehicle* favorites[MAX_FAVORITES]; // AGGREGATION
    int      favCount;
    Message  inbox[MAX_MESSAGES];      // COMPOSITION
    int      msgCount;
    double   budget;
    string   preferredBrand;
public:
    Buyer(int id, string un, string em, string ph,
          Address ad, double bud, string pref)
        : User(id, un, em, ph, ad), favCount(0), msgCount(0),
          budget(bud), preferredBrand(pref)
    { for (int i=0;i<MAX_FAVORITES;i++) favorites[i]=nullptr; }

    double getBudget()         const { return budget; }
    string getPreferredBrand() const { return preferredBrand; }
    int    getFavCount()       const { return favCount; }
    int    getMsgCount()       const { return msgCount; }
    void   setBudget(double b)       { budget = b; }
    void   setPreferredBrand(string p){ preferredBrand = p; }

    string getRole() const override { return "Buyer"; }

    bool saveFavorite(Vehicle* v) {
        if (favCount >= MAX_FAVORITES) {
            cout << "  Favorites full.\n"; return false;
        }
        for (int i=0;i<favCount;i++)
            if (favorites[i]->getID()==v->getID()) {
                cout << "  Already saved.\n"; return false;
            }
        favorites[favCount++] = v;
        cout << "  Saved: " << v->getSummary() << "\n";
        return true;
    }

    bool removeFavorite(int lid) {
        for (int i=0;i<favCount;i++) {
            if (favorites[i]->getID()==lid) {
                for (int j=i;j<favCount-1;j++) favorites[j]=favorites[j+1];
                favorites[--favCount]=nullptr;
                cout << "  Removed favorite #" << lid << "\n";
                return true;
            }
        }
        cout << "  Not found in favorites.\n"; return false;
    }

    void showFavorites() const {
        cout << "\n  Favorites of " << username << ":\n";
        if (favCount==0) { cout << "  None.\n"; return; }
        for (int i=0;i<favCount;i++)
            cout << "  "<<(i+1)<<". "<<favorites[i]->getSummary()<<"\n";
    }

    bool receiveMessage(const Message& m) {
        if (msgCount >= MAX_MESSAGES) { cout << "  Inbox full.\n"; return false; }
        inbox[msgCount++] = m;
        return true;
    }

    void showInbox() const {
        cout << "\n  Inbox of " << username << ":\n";
        if (msgCount==0) { cout << "  Empty.\n"; return; }
        for (int i=0;i<msgCount;i++) inbox[i].display();
    }

    // CONSTANT FUNCTION: affordability check
    bool canAfford(const Vehicle& v) const { return v.getPrice() <= budget; }
};

// ═══════════════════════════════════
// CLASS 6: Seller (IS-A User)
// AGGREGATES Vehicle* listings (shared with Marketplace)
// Parameterized: sellers always have a business name
// ═══════════════════════════════════
class Seller : public User {
private:
    Vehicle* myListings[MAX_SELLER_ADS]; // AGGREGATION
    int      listingCount;
    double   totalRevenue;
    string   businessName;
    bool     isPremium;
public:
    Seller(int id, string un, string em, string ph,
           Address ad, string biz, bool prem=false)
        : User(id, un, em, ph, ad), listingCount(0), totalRevenue(0),
          businessName(biz), isPremium(prem)
    { for (int i=0;i<MAX_SELLER_ADS;i++) myListings[i]=nullptr; }

    string getBusinessName() const { return businessName; }
    bool   getIsPremium()    const { return isPremium; }
    int    getListingCount() const { return listingCount; }
    double getTotalRevenue() const { return totalRevenue; }
    void   setBusinessName(string b) { businessName = b; }
    void   setIsPremium(bool p)      { isPremium = p; }

    string getRole() const override { return "Seller"; }

    bool addListing(Vehicle* v) {
        if (listingCount >= MAX_SELLER_ADS) {
            cout << "  Listing limit reached.\n"; return false;
        }
        myListings[listingCount++] = v;
        cout << "  " << username << " claimed: " << v->getSummary() << "\n";
        return true;
    }

    bool removeListing(int lid) {
        for (int i=0;i<listingCount;i++) {
            if (myListings[i]->getID()==lid) {
                myListings[i]->deactivate();
                for (int j=i;j<listingCount-1;j++) myListings[j]=myListings[j+1];
                myListings[--listingCount]=nullptr;
                cout << "  Listing #"<<lid<<" removed.\n";
                return true;
            }
        }
        cout << "  Listing not found.\n"; return false;
    }

    void showMyListings() const {
        cout << "\n  Listings by " << username << " (" << businessName << "):\n";
        if (listingCount==0) { cout << "  None.\n"; return; }
        for (int i=0;i<listingCount;i++)
            cout << "  "<<(i+1)<<". "<<myListings[i]->getSummary()
                 <<(myListings[i]->getApproved()?" [OK]":" [Pending]")<<"\n";
    }

    void recordSale(double amt) {
        totalRevenue += amt;
        cout << "  Sale PKR "<<(long long)amt
             <<" recorded. Total: PKR "<<(long long)totalRevenue<<"\n";
    }

    void upgradeToPremium() {
        isPremium = true;
        cout << "  " << username << " is now Premium!\n";
    }
};

// ═══════════════════════════════════
// CLASS 8: Admin (IS-A User)
// Default: system admin auto-created
// Parameterized: custom admin account
// ═══════════════════════════════════
class Admin : public User {
private:
    string adminLevel;
    int    listingsApproved, listingsRejected, usersVerified;
    bool   canDeleteUsers;
public:
    // Default: system creates one admin automatically
    Admin() : User(1,"sysadmin","admin@pw.pk","0300-0000000",
                   Address("Karachi","Sindh","Pakistan","I.I. Chundrigar",75600)),
              adminLevel("SuperAdmin"), listingsApproved(0),
              listingsRejected(0), usersVerified(0), canDeleteUsers(true)
    { isVerified=true; }

    Admin(int id, string un, string em, string ph,
          Address ad, string lv, bool del)
        : User(id, un, em, ph, ad), adminLevel(lv),
          listingsApproved(0), listingsRejected(0),
          usersVerified(0), canDeleteUsers(del)
    { isVerified=true; }

    string getAdminLevel()       const { return adminLevel; }
    int    getListingsApproved() const { return listingsApproved; }
    int    getListingsRejected() const { return listingsRejected; }

    string getRole() const override { return "Admin (" + adminLevel + ")"; }

    void approveListing(Vehicle* v) {
        if (!v) return;
        if (v->getApproved()) { cout << "  Already approved.\n"; return; }
        v->approve(); listingsApproved++;
        cout << "  APPROVED Listing #"<<v->getID()<<" by "<<username<<"\n";
    }

    void rejectListing(Vehicle* v) {
        if (!v) return;
        v->deactivate(); listingsRejected++;
        cout << "  REJECTED Listing #"<<v->getID()<<" by "<<username<<"\n";
    }

    void verifyUser(User* u) {
        if (!u) return;
        u->verify(); usersVerified++;
        cout << "  User '"<<u->getUsername()<<"' verified.\n";
    }

    // CONSTANT FUNCTION: admin stats summary
    void showStats() const {
        cout << "\n  Admin Dashboard (" << username << "):\n";
        cout << "  Level    : " << adminLevel << "\n";
        cout << "  Approved : " << listingsApproved << "\n";
        cout << "  Rejected : " << listingsRejected << "\n";
        cout << "  Verified : " << usersVerified << " users\n";
    }
};

// ═══════════════════════════════════
// CLASS 10: Marketplace (Central Hub)
// COMPOSITION: owns Car[] and Bike[] (arrays of objects)
// AGGREGATION: stores User* pointers (external)
// Default: fresh empty platform
//
// STATIC: listingFeeRate
//   Reason: platform policy applies globally,
//   independent of any Marketplace instance
//
// CONSTANT: marketplaceName
//   Never changes after construction
// ═══════════════════════════════════
class Marketplace {
private:
    Car  carListings[MAX_LISTINGS];   // ARRAY OF OBJECTS (COMPOSITION)
    Bike bikeListings[MAX_LISTINGS];  // ARRAY OF OBJECTS (COMPOSITION)
    int  carCount, bikeCount;

    User*   registeredUsers[MAX_USERS]; // AGGREGATION
    int     userCount;

    Message msgLog[MAX_MESSAGES * 5];
    int     msgLogCount;

    int nextID, nextMsgID;

    const string marketplaceName; // CONSTANT — no setter, set once
    static double listingFeeRate; // STATIC — platform-wide policy

public:
    Marketplace()
        : carCount(0), bikeCount(0), userCount(0),
          msgLogCount(0), nextID(1001), nextMsgID(1),
          marketplaceName("PakWheels Clone")
    { for (int i=0;i<MAX_USERS;i++) registeredUsers[i]=nullptr; }

    string getName() const { return marketplaceName; }
    static double getListingFeeRate()     { return listingFeeRate; }
    static void   setListingFeeRate(double r) { listingFeeRate = r; }

    // Add car listing (COMPOSITION — car stored in owned array)
    int addCar(string br, string mo, int yr, double pr, int mi,
               string reg, string fuel, string trans, int eng,
               string col, int doors, string tag="Standard") {
        if (carCount>=MAX_LISTINGS) { cout<<"  Car limit.\n"; return -1; }
        int id = nextID++;
        carListings[carCount++] = Car(id,br,mo,yr,pr,mi,reg,
                                      fuel,trans,eng,col,doors,tag);
        cout << "  Car listing created [#"<<id<<"]: "<<br<<" "<<mo<<"\n";
        return id;
    }

    // Add bike listing
    int addBike(string br, string mo, int yr, double pr, int mi,
                string reg, string bt, int eng, bool abs,
                string fuel, string cond) {
        if (bikeCount>=MAX_LISTINGS) { cout<<"  Bike limit.\n"; return -1; }
        int id = nextID++;
        bikeListings[bikeCount++] = Bike(id,br,mo,yr,pr,mi,
                                         reg,bt,eng,abs,fuel,cond);
        cout << "  Bike listing created [#"<<id<<"]: "<<br<<" "<<mo<<"\n";
        return id;
    }

    Car*  findCarByID(int id) {
        for(int i=0;i<carCount;i++) if(carListings[i].getID()==id) return &carListings[i];
        return nullptr;
    }
    Bike* findBikeByID(int id) {
        for(int i=0;i<bikeCount;i++) if(bikeListings[i].getID()==id) return &bikeListings[i];
        return nullptr;
    }
    Vehicle* findVehicleByID(int id) {
        Vehicle* v = findCarByID(id);
        return v ? v : findBikeByID(id);
    }

    bool updateCarPrice(int id, double np) {
        Car* c = findCarByID(id);
        if (!c) { cout<<"  Car #"<<id<<" not found.\n"; return false; }
        double old = c->getPrice();
        c->setPrice(np);
        cout << "  Price updated #"<<id<<": "
             <<(long long)old<<" -> "<<(long long)np<<"\n";
        return true;
    }

    bool deleteListing(int id) {
        Vehicle* v = findVehicleByID(id);
        if (!v) { cout<<"  Not found.\n"; return false; }
        v->deactivate();
        cout << "  Listing #"<<id<<" deactivated.\n";
        return true;
    }

    void searchVehicles(string fb="", string fm="",
                        double lo=0, double hi=1e10,
                        int ylo=1980, int yhi=2030, int mxMi=999999) const {
        cout << "\n  ── SEARCH RESULTS ──\n";
        bool found = false;
        for (int i=0;i<carCount;i++) {
            if (carListings[i].getActive() && carListings[i].getApproved() &&
                carListings[i].matchesFilter(fb,fm,lo,hi,ylo,yhi,mxMi)) {
                cout << "  [CAR]  " << carListings[i].getSummary() << "\n";
                found = true;
            }
        }
        for (int i=0;i<bikeCount;i++) {
            if (bikeListings[i].getActive() && bikeListings[i].getApproved() &&
                bikeListings[i].matchesFilter(fb,fm,lo,hi,ylo,yhi,mxMi)) {
                cout << "  [BIKE] " << bikeListings[i].getSummary() << "\n";
                found = true;
            }
        }
        if (!found) cout << "  No results.\n";
    }

    void showAllApprovedListings() const {
        cout << "\n  ── ALL APPROVED LISTINGS ──\n";
        int cnt = 0;
        for (int i=0;i<carCount;i++)
            if (carListings[i].getApproved() && carListings[i].getActive())
                { cout<<"  [CAR]  "<<carListings[i].getSummary()<<"\n"; cnt++; }
        for (int i=0;i<bikeCount;i++)
            if (bikeListings[i].getApproved() && bikeListings[i].getActive())
                { cout<<"  [BIKE] "<<bikeListings[i].getSummary()<<"\n"; cnt++; }
        if (cnt==0) cout << "  None.\n";
        else cout << "  Total: " << cnt << "\n";
    }

    void registerUser(User* u) {
        if (userCount>=MAX_USERS) { cout<<"  User limit.\n"; return; }
        registeredUsers[userCount++] = u;
        cout << "  Registered: "<<u->getUsername()<<" ["<<u->getRole()<<"]\n";
    }

    bool sendMessage(Buyer* from, Buyer* to, string content, int lid) {
        if (msgLogCount >= MAX_MESSAGES*5) { cout<<"  Log full.\n"; return false; }
        Message m(nextMsgID++, from->getUsername(),
                  to->getUsername(), content, lid, "2024-06-01 14:30");
        msgLog[msgLogCount++] = m;
        to->receiveMessage(m);
        cout << "  Message sent: "<<from->getUsername()
             <<" -> "<<to->getUsername()<<"\n";
        return true;
    }

    void showPlatformStats() const {
        cout << "\n  ── PLATFORM STATS ──\n";
        cout << "  Platform  : " << marketplaceName << "\n";
        cout << "  Cars      : " << carCount  << "\n";
        cout << "  Bikes     : " << bikeCount << "\n";
        cout << "  Users     : " << User::getTotalUsers() << "\n";
        cout << "  Vehicles  : " << Vehicle::getTotalVehiclesCreated() << "\n";
        cout << "  Fee Rate  : " << listingFeeRate*100 << "%\n";
    }
};
double Marketplace::listingFeeRate = 0.02;

// ═══════════════════════════════════
//   MAIN DRIVER
// ═══════════════════════════════════
int main() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║  CAR MARKETPLACE — PakWheels Clone   ║\n";
    cout << "╚══════════════════════════════════════╝\n";

    // [1] Marketplace (COMPOSITION)
    cout << "\n[1] Creating Marketplace...\n";
    Marketplace market;
    cout << "    " << market.getName() << " initialized.\n";

    // [2] Admin — DEFAULT CONSTRUCTOR
    cout << "\n[2] Admin (default constructor):\n";
    Admin admin;
    market.registerUser(&admin);

    // [3] Sellers — PARAMETERIZED CONSTRUCTOR
    cout << "\n[3] Sellers (parameterized constructor):\n";
    Address a1("Lahore",   "Punjab","Pakistan","MM Alam Rd",  54000);
    Address a2("Karachi",  "Sindh", "Pakistan","Faisal Ave",  75350);
    Seller seller1(201,"ahmed_autos","a@autos.pk","0321-1111111",a1,"Ahmed Autos",true);
    Seller seller2(202,"khi_wheels", "k@kw.pk",  "0333-2222222",a2,"KHI Wheels", false);
    market.registerUser(&seller1);
    market.registerUser(&seller2);

    // [4] Buyers — PARAMETERIZED CONSTRUCTOR
    cout << "\n[4] Buyers (parameterized constructor):\n";
    Address a3("Islamabad","ICT",   "Pakistan","F-7 Markaz",  44000);
    Address a4("Lahore",   "Punjab","Pakistan","DHA Phase 5", 54792);
    Buyer buyer1(301,"zain_buyer","z@gmail.com","0312-3333333",a3,3500000,"Toyota");
    Buyer buyer2(302,"sara_shops","s@yahoo.com","0345-4444444",a4,1200000,"Honda");
    market.registerUser(&buyer1);
    market.registerUser(&buyer2);

    // [5] Admin verifies users
    cout << "\n[5] Admin verifying users:\n";
    admin.verifyUser(&seller1);
    admin.verifyUser(&buyer1);

    // [6] Add listings
    cout << "\n[6] Adding vehicle listings:\n";
    int cid1 = market.addCar("Toyota","Corolla",2021,3200000,45000,
                              "LHR-AA-2021","Petrol","Automatic",1800,"White",4,"Family");
    int cid2 = market.addCar("Honda","Civic",  2022,5500000,20000,
                              "KHI-KW-2022","Petrol","Automatic",1500,"Black",4,"Premium");
    int cid3 = market.addCar("Suzuki","Swift", 2020,1900000,60000,
                              "ISB-SW-2020","Petrol","Manual",   1000,"Red",  4,"Economy");
    int cid4 = market.addCar("Hyundai","Tucson",2023,8900000,10000,
                              "RWP-HY-2023","Hybrid","Automatic",2000,"Blue", 4,"SUV");
    int bid1 = market.addBike("Honda","CD-70",  2022,135000,8000,
                              "LHR-BK-001","Standard", 70,false,"Petrol","Used");
    int bid2 = market.addBike("Yamaha","YBR-125",2023,320000,3000,
                              "KHI-BK-002","Standard",125, true,"Petrol","New");

    // [7] Sellers claim listings (AGGREGATION)
    cout << "\n[7] Sellers claiming listings (AGGREGATION):\n";
    Vehicle* v1  = market.findVehicleByID(cid1);
    Vehicle* v2  = market.findVehicleByID(cid2);
    Vehicle* v3  = market.findVehicleByID(cid3);
    Vehicle* bv1 = market.findVehicleByID(bid1);
    if(v1)  seller1.addListing(v1);
    if(v2)  seller2.addListing(v2);
    if(v3)  seller1.addListing(v3);
    if(bv1) seller1.addListing(bv1);

    // [8] Admin approve/reject
    cout << "\n[8] Admin approving & rejecting:\n";
    admin.approveListing(market.findVehicleByID(cid1));
    admin.approveListing(market.findVehicleByID(cid2));
    admin.approveListing(market.findVehicleByID(cid3));
    admin.approveListing(market.findVehicleByID(bid1));
    admin.approveListing(market.findVehicleByID(bid2));
    admin.rejectListing(market.findVehicleByID(cid4));

    // [9] All approved
    market.showAllApprovedListings();

    // [10] Polymorphism — virtual displayInfo()
    cout << "\n[10] POLYMORPHISM via base pointer:\n";
    Vehicle* poly = market.findVehicleByID(cid1);
    if(poly) { cout << "  Type: " << poly->getType() << "\n"; poly->displayInfo(); }
    poly = market.findVehicleByID(bid2);
    if(poly) { cout << "  Type: " << poly->getType() << "\n"; poly->displayInfo(); }

    // [11] Search & Filter
    cout << "[11] SEARCH & FILTER:\n";
    cout << "  >> Toyota:\n";        market.searchVehicles("Toyota");
    cout << "  >> PKR 1M-4M:\n";    market.searchVehicles("","",1000000,4000000);
    cout << "  >> Year>=2022, mileage<=25000:\n";
                                      market.searchVehicles("","",0,1e10,2022,2030,25000);

    // [12] Favorites (AGGREGATION)
    cout << "\n[12] AGGREGATION — Buyer favorites:\n";
    if(v1)  buyer1.saveFavorite(v1);
    if(v2)  buyer1.saveFavorite(v2);
    if(bv1) buyer2.saveFavorite(bv1);
    buyer1.showFavorites();
    buyer2.showFavorites();

    // [13] Remove favorite
    cout << "\n[13] Remove favorite:\n";
    buyer1.removeFavorite(cid2);
    buyer1.showFavorites();

    // [14] Messaging
    cout << "\n[14] MESSAGING:\n";
    market.sendMessage(&buyer1, &buyer2,
                       "Is the CD-70 available? Can we negotiate?", bid1);
    buyer2.showInbox();

    // [15] Update & Delete
    cout << "\n[15] UPDATE & DELETE:\n";
    market.updateCarPrice(cid3, 1750000);
    market.deleteListing(cid2);

    // [16] Seller removes listing
    cout << "\n[16] Seller removes listing:\n";
    seller1.removeListing(cid3);
    seller1.showMyListings();

    // [17] Constant functions
    cout << "\n[17] CONSTANT FUNCTIONS:\n";
    Car* dc = market.findCarByID(cid1);
    if(dc) {
        cout << "  Reg (no setter): " << dc->getRegNo() << "\n";
        cout << "  Tax estimate   : PKR " << (long long)dc->estimateTax() << "\n";
        cout << "  Is Automatic?  : " << (dc->isAutomatic()?"Yes":"No") << "\n";
        cout << "  Buyer1 afford? : " << (buyer1.canAfford(*dc)?"Yes":"No") << "\n";
        cout << "  Seller1 city   : " << seller1.getAddress().getFullAddress() << "\n";
        cout << "  Same city s1s2?: " << (seller1.isSameCityAs(seller2)?"Yes":"No") << "\n";
    }
    Bike* db = market.findBikeByID(bid2);
    if(db) {
        cout << "  City commuter? : " << (db->isCityCommuter()?"Yes":"No") << "\n";
        cout << "  Monthly fuel   : PKR " << db->monthlyFuelCost() << "\n";
        cout << "  Resale est.    : PKR " << (long long)db->estimateResale() << "\n";
    }

    // [18] Static members
    cout << "\n[18] STATIC MEMBERS:\n";
    cout << "  User::getTotalUsers()             = " << User::getTotalUsers() << "\n";
    cout << "  Vehicle::getTotalVehiclesCreated()= " << Vehicle::getTotalVehiclesCreated() << "\n";
    cout << "  Marketplace::getListingFeeRate()  = " << Marketplace::getListingFeeRate()*100 << "%\n";
    Marketplace::setListingFeeRate(0.025);
    cout << "  Updated fee rate                  = " << Marketplace::getListingFeeRate()*100 << "%\n";

    // [19] Copy constructors
    cout << "\n[19] COPY CONSTRUCTORS:\n";
    if(dc) { Car cp(*dc); cout << "  Car copy: " << cp.getSummary() << "\n"; }
    Address acopy(a1);
    cout << "  Address copy: " << acopy.getFullAddress() << "\n";

    // [20] Admin dashboard
    cout << "\n[20] Admin stats:\n";
    admin.showStats();
    admin.displayBasicInfo();

    // [21] Seller actions
    cout << "\n[21] Seller premium & sale:\n";
    seller2.upgradeToPremium();
    seller2.recordSale(5500000);

    // [22] Platform stats
    market.showPlatformStats();

  
    cout << "║       ALL REQUIREMENTS MET           ║\n";
    return 0;
}
