#include <iostream>
#include <string>
using namespace std;

class Car {
private:
    string brand;
    string model;
    int year;
    double price;
    int mileage;
    bool available;

public:

    // Default Constructor
    Car() {
        brand="Unknown";
        model="Unknown";
        year=0;
        price=0;
        mileage=0;
        available=true;
    }

    // Parameterized Constructor
    Car(string b,string m,int y,double p,int mil) {
        brand=b;
        model=m;
        year=y;
        price=p;
        mileage=mil;
        available=true;
    }

    // Copy Constructor
    Car(const Car &c) {
        brand=c.brand;
        model=c.model;
        year=c.year;
        price=c.price;
        mileage=c.mileage;
        available=c.available;
    }

    void show() const {
        cout<<"Brand: "<<brand<<endl;
        cout<<"Model: "<<model<<endl;
        cout<<"Year: "<<year<<endl;
        cout<<"Price: "<<price<<endl;
        cout<<"Mileage: "<<mileage<<endl;
        cout<<"Status: "<<(available?"Available":"Sold")<<endl;
        cout<<"-------------------"<<endl;
    }

    void markSold() {
        available=false;
    }

    int getYear(){ return year; }
    double getPrice(){ return price; }
    string getBrand(){ return brand; }
};


class Listing {
private:
    int listingID;
    Car car;
    bool approved;
    const string platformName="PakWheels Clone";

public:

    Listing() {
        listingID=0;
        approved=false;
    }

    Listing(int id,Car c) {
        listingID=id;
        car=c;
        approved=false;
    }

    void approve() {
        approved=true;
    }

    void showListing() const {
        if(approved) {
            cout<<"Listing ID: "<<listingID<<endl;
            car.show();
        }
        else
        cout<<"Listing Pending Approval"<<endl;
    }

    int getID(){ return listingID; }

    Car& getCar(){ return car; }
};


class Message {
private:
    string sender;
    string receiver;
    string text;
    bool read;

public:

    Message() {
        sender="";
        receiver="";
        text="";
        read=false;
    }

    Message(string s,string r,string t) {
        sender=s;
        receiver=r;
        text=t;
        read=false;
    }

    void display() {
        cout<<"From: "<<sender<<endl;
        cout<<"To: "<<receiver<<endl;
        cout<<"Message: "<<text<<endl;
    }

    void markRead() {
        read=true;
    }
};


class Favorite {
private:
    Car favCars[10];
    int count;

public:

    Favorite() {
        count=0;
    }

    void addFavorite(Car c) {
        favCars[count++]=c;
    }

    void showFavorites() {
        cout<<"Favorite Cars"<<endl;
        for(int i=0;i<count;i++)
        favCars[i].show();
    }

    void removeFavorite() {
        if(count>0)
        count--;
    }
};


class SearchFilter {
public:

    void filterByPrice(Listing listings[],int size,double maxPrice) {
        for(int i=0;i<size;i++) {
            if(listings[i].getCar().getPrice()<=maxPrice)
            listings[i].showListing();
        }
    }

    void filterByYear(Listing listings[],int size,int minYear) {
        for(int i=0;i<size;i++) {
            if(listings[i].getCar().getYear()>=minYear)
            listings[i].showListing();
        }
    }

    void filterByBrand(Listing listings[],int size,string brand) {
        for(int i=0;i<size;i++) {
            if(listings[i].getCar().getBrand()==brand)
            listings[i].showListing();
        }
    }
};


class User {
protected:
    string name;
    string email;
    int id;

public:

    User() {
        name="Unknown";
        email="None";
        id=0;
    }

    User(string n,string e,int i) {
        name=n;
        email=e;
        id=i;
    }

    void showUser() {
        cout<<"Name: "<<name<<endl;
        cout<<"Email: "<<email<<endl;
    }
};


class Seller : public User {
public:

    Seller(string n,string e,int i):User(n,e,i){}

    void addListing(Listing listings[],int &count,Listing l) {
        listings[count++]=l;
    }

    void updateListing(Listing &l) {
        cout<<"Listing Updated"<<endl;
    }

    void deleteListing(Listing listings[],int &count,int index) {

        for(int i=index;i<count-1;i++)
        listings[i]=listings[i+1];

        count--;
    }

    void replyMessage(string msg) {
        cout<<"Seller reply: "<<msg<<endl;
    }
};


class Buyer : public User {
private:
    Favorite fav;

public:

    Buyer(string n,string e,int i):User(n,e,i){}

    void saveFavorite(Car c) {
        fav.addFavorite(c);
    }

    void viewFavorites() {
        fav.showFavorites();
    }

    void sendMessage(string seller,string msg) {
        Message m(name,seller,msg);
        m.display();
    }

};


class Admin : public User {
public:

    Admin():User("Admin","admin@market.com",1){}

    void approveListing(Listing &l) {
        l.approve();
        cout<<"Listing Approved"<<endl;
    }

    void removeListing(Listing listings[],int &count,int index) {

        for(int i=index;i<count-1;i++)
        listings[i]=listings[i+1];

        count--;

        cout<<"Listing Removed"<<endl;
    }

    void viewAllListings(Listing listings[],int count) {
        for(int i=0;i<count;i++)
        listings[i].showListing();
    }
};


class Marketplace {
private:
    Listing listings[100];
    int count;

    static int totalListings;

public:

    Marketplace() {
        count=0;
    }

    void addListing(Listing l) {
        listings[count++]=l;
        totalListings++;
    }

    Listing* getListings() {
        return listings;
    }

    int getCount() {
        return count;
    }

    static void showTotalListings() {
        cout<<"Total Listings: "<<totalListings<<endl;
    }
};

int Marketplace::totalListings=0;


int main() {

    Marketplace market;

    Seller seller("Ali","ali@email.com",101);
    Buyer buyer("Ahmed","ahmed@email.com",201);
    Admin admin;

    Car c1("Toyota","Corolla",2020,4500000,30000);
    Car c2("Honda","Civic",2019,4200000,40000);
    Car c3("Suzuki","Alto",2022,2500000,10000);

    Listing l1(1,c1);
    Listing l2(2,c2);
    Listing l3(3,c3);

    market.addListing(l1);
    market.addListing(l2);
    market.addListing(l3);

    admin.approveListing(market.getListings()[0]);
    admin.approveListing(market.getListings()[1]);
    admin.approveListing(market.getListings()[2]);

    cout<<"All Listings"<<endl;
    admin.viewAllListings(market.getListings(),market.getCount());

    SearchFilter filter;

    cout<<"\nCars under 4.3M"<<endl;
    filter.filterByPrice(market.getListings(),market.getCount(),4300000);

    cout<<"\nCars after 2019"<<endl;
    filter.filterByYear(market.getListings(),market.getCount(),2019);

    buyer.saveFavorite(c1);
    buyer.saveFavorite(c3);

    cout<<"\nBuyer Favorites"<<endl;
    buyer.viewFavorites();

    cout<<"\nBuyer sends message"<<endl;
    buyer.sendMessage("Ali","Is this car still available?");

    Marketplace::showTotalListings();

}
