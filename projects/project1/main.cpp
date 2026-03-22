#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
class Product {
private:
    static int noProducts;
    const int id;
    char* name;
    double price;
    bool isAvailable;
    int stock;
public:
    Product();
    Product(char* name, double price,int stock);
    Product(const Product& obj);
    Product& operator=(const Product& obj);
    ~Product();

    int getId() const;
    char* getName() const;
    double getPrice() const;
    bool getIsAvailable() const;
    int getStock() const;

    void restock(int quantity);
    bool purchase();

    bool operator==(const Product& other) const;
    bool operator<(const Product& other) const;

    friend ostream& operator<<(ostream& out, const Product& obj);
    friend istream& operator>>(istream& in, Product& obj);
};
int Product::noProducts=0;
Product::Product() : id(++noProducts) {
    this->name =new char[4];
    strcpy(this->name,"N/A");
    this->price=0.0;
    this->isAvailable=false;
    this->stock=0;
}
Product::Product(char* name, double price,int stock) : id(++noProducts) {
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->price=price;
    this->stock=stock;
    this->isAvailable=(stock>0);
}
Product::Product(const Product& obj) : id(++noProducts) {
    this->name=new char[strlen(obj.name)+1];
    strcpy(this->name,obj.name);
    this->price=obj.price;
    this->stock=obj.stock;
    this->isAvailable=obj.isAvailable;
}
Product& Product::operator=(const Product& obj) {
    if (this==&obj) return *this;
    delete[] this->name;
    this->name=new char[strlen(obj.name)+1];
    strcpy(this->name,obj.name);
    this->price=obj.price;
    this->stock=obj.stock;
    this->isAvailable=obj.isAvailable;
    return *this;
}
Product::~Product() {
    delete[] this->name;
}

int Product::getId() const {
    return id;
}
char* Product::getName() const {
    return name;
}
double Product::getPrice() const {
    return price;
}
bool Product::getIsAvailable() const {
    return isAvailable;
}
int Product::getStock() const {
    return stock;
}

void Product::restock(int quantity) {
    this->stock += quantity;
    this->isAvailable=true;
}
bool Product::purchase() {
    if (this->stock == 0) return false;
    this->stock--;
    if (this->stock == 0) this->isAvailable=false;
    return true;
}

bool Product::operator==(const Product& other) const {
    return strcmp(this->name,other.name)==0;
}
bool Product::operator<(const Product& other) const {
    return this->price < other.price;
}
ostream& operator<<(ostream& out, const Product& obj) {
    out << "[Product #" << obj.id << "] "
        << obj.name <<"|"
        << obj.price << "lei |"
        <<"stock: "<<obj.stock<<"|"
        <<(obj.isAvailable ? "available" : "not available");
    return out;
}
istream& operator>>(istream& in, Product& obj) {
    char buff[256];

    cout<< " Name: ";
    in.getline(buff,256);
    delete[] obj.name;
    obj.name=new char[strlen(buff)+1];
    strcpy(obj.name,buff);

    cout<<" Price: ";
    in >> obj.price;

    cout<<"Stock: ";
    in >> obj.stock;
    in.ignore();

    obj.isAvailable=(obj.stock>0);
    return in;
}

class Cart {
private:
    static int noCarts;
    const int id;
    Product** products;
    int noProducts;
    double TotalPrice;
    bool isCheckedOut;

public:
    Cart();
    Cart(const Cart& obj);
    Cart& operator=(const Cart& obj);
    ~Cart();

    int getId() const;
    double getTotalPrice() const;
    bool getIsCheckedOut() const;
    int getNoProducts() const;

    bool addProduct(Product* product);
    void removeProduct(const char* name);
    double calculateTotal();
    bool checkout();

    Cart& operator+=(Product* product);
    bool operator!() const;

    friend ostream& operator<<(ostream& out, const Cart& obj);
};
int Cart::noCarts=0;
Cart::Cart() : id(++noCarts) {
    this->products=nullptr;
    this->noProducts=0;
    this->TotalPrice=0.0;
    this->isCheckedOut=false;
}

Cart::Cart(const Cart &obj) : id(++noCarts) {
    this->noProducts=obj.noProducts;
    this->TotalPrice=obj.TotalPrice;
    this->isCheckedOut=obj.isCheckedOut;
    this->products=new Product*[obj.noProducts];
    for(int i=0;i<obj.noProducts;i++)
        this->products[i]= new Product(*obj.products[i]);
}
Cart& Cart::operator=(const Cart &obj) {
    if (this==&obj) return *this;
    for (int i=0;i<this->noProducts;i++)
        delete this->products[i];
    delete[] this->products;
    this->noProducts=obj.noProducts;
    this->TotalPrice=obj.TotalPrice;
    this->isCheckedOut=obj.isCheckedOut;
    this->products=new Product*[obj.noProducts];
    for (int i=0;i< obj.noProducts;i++)
        this->products[i]= new Product(*obj.products[i]);
    return *this;
}
Cart::~Cart() {
    for (int i=0;i<this->noProducts;i++)
        delete this->products[i];
    delete[] this->products;
}

double Cart::getTotalPrice() const {
    return TotalPrice;
}
bool Cart::getIsCheckedOut() const {
    return isCheckedOut;
}
int Cart::getNoProducts() const {
    return noProducts;
}

bool Cart::addProduct(Product* product) {
    if (this->isCheckedOut == true)
        return false;
    Product** temp= new Product*[this->noProducts + 1];
    for (int i=0 ; i<this->noProducts ; i++)
        temp[i]=this->products[i];
    temp[this->noProducts]=new Product(*product);
    delete[] this->products;
    this->products=temp;
    this->noProducts++;
    this->TotalPrice += product-> getPrice();
    return true;
}
void Cart::removeProduct(const char* name) {
    int position =-1;
    for (int i=0;i<this->noProducts;i++) {
        Product* current= this->products[i];
        if (strcmp(current->getName(),name)==0)
        {
            position=i;
            break;
        }
    }
    if (position==-1)
        return;
    Product* toDelete = this->products[position];
    this->TotalPrice-=toDelete->getPrice();
    delete toDelete;

    Product** temp= new Product*[this->noProducts -1];

    int k=0;
    for (int i=0;i< this->noProducts;i++) {
        if ( i!= position ) {
            temp[k]=this->products[i];
            k++;
        }
    }
    delete[] this->products;
    this->products=temp;
    this->noProducts--;
}
double Cart::calculateTotal() {
    this->TotalPrice=0.0;
    for (int i=0;i<this->noProducts;i++) {
        Product* current=this->products[i];
        this->TotalPrice += current->getPrice();
    }
    return this->TotalPrice;
}
bool Cart::checkout() {
    if (this->isCheckedOut == true || this->noProducts == 0)
        return false;
    for (int i=0;i<this->noProducts;i++) {
        Product* current=this->products[i];
        current->purchase();
    }
    this->isCheckedOut=true;
    return true;
}
Cart& Cart::operator+=(Product* product) {
    addProduct(product);
    return *this;
}
bool Cart::operator!() const {
    return this->noProducts==0;
}
ostream& operator<<(ostream& out,const Cart& obj) {
    out << "[Cart #"<<obj.id<<"]"
        <<"produse: "<<obj.noProducts<<" | "
        <<"total: "<<obj.TotalPrice<<"lei | "
        << (obj.isCheckedOut ? "checked-out" : "not checked-out");
    for (int i=0; i< obj.noProducts; i++) {
        out<< "\n   "<<*obj.products[i];
    }
    return out;
}

class Review {
private:
    static int noReviews;
    const int id;
    char* ProductName;
    float rating;
    char comment[256];
    bool isVerified;
    int helpfulVotes;
public:
    Review();
    Review(char* ProductName,float rating, char* comment, bool isVerified);
    Review(const Review& obj);
    Review& operator=(const Review& obj);
    ~Review();

    int getId() const;
    float getRating() const;
    bool getIsVerified() const;
    int getHelpfulVotes() const;
    const char* getProductName() const;

    void setRating(float rating);
    void setComment(const char* comment);

    void MarkHelpful();
    bool isPositive() const;
    static int getTotalReviews();

    bool operator==(const Review& other) const;
    bool operator<(const Review& other) const;

    friend istream& operator>>(istream& in,Review& obj);
    friend ostream& operator<<(ostream& out,const Review& obj);
};
int Review::noReviews=0;
Review::Review() : id(++noReviews) {
    this->ProductName = new char[4];
    strcpy(this->ProductName,"N/A");
    this->rating=0.0f;
    strcpy(this->comment,"N/A");
    this->isVerified=false;
    this->helpfulVotes=0;
}
Review::Review(char* ProductName,float rating,char* comment,bool isVerified) : id(++noReviews) {
    this->ProductName = new char[strlen(ProductName)+1];
    strcpy(this->ProductName,ProductName);
    this->rating= (rating >= 1.0f && rating<= 5.0f) ? rating : 0.0f;
    strncpy(this->comment,comment,255);
    this->comment[255]='\0';
    this->isVerified=isVerified;
    this->helpfulVotes=0;
}
Review::Review(const Review& obj) : id(++noReviews) {
    this->ProductName = new char[strlen(obj.ProductName)+1];
    strcpy(this->ProductName,obj.ProductName);
    this->rating= obj.rating;
    strcpy(this->comment,obj.comment);
    this->isVerified=obj.isVerified;
    this->helpfulVotes=obj.helpfulVotes;
}
Review& Review::operator=(const Review& obj) {
    if (this==&obj) return *this;
    delete[] this->ProductName;
    this->ProductName = new char[strlen(obj.ProductName)+1];
    strcpy(this->ProductName,obj.ProductName);
    this->rating= obj.rating;
    strcpy(this->comment,obj.comment);
    this->isVerified=obj.isVerified;
    this->helpfulVotes=obj.helpfulVotes;
    return *this;
}
Review::~Review() {
    delete[] this->ProductName;
}
int Review::getId() const {
    return this->id;
}
float Review::getRating() const {
    return this->rating;
}
bool Review::getIsVerified() const {
    return this->isVerified;
}
int Review::getHelpfulVotes() const {
    return this->helpfulVotes;
}
const char* Review::getProductName() const {
    return this->ProductName;
}
void Review::setRating(float rating) {
    if (rating >= 1.0f && rating<= 5.0f)
        this->rating=rating;
}
void Review::setComment(const char* comment) {
    strncpy(this->comment,comment,255);
    this->comment[255]='\0';
}

void Review::MarkHelpful() {
    this->helpfulVotes++;
}
bool Review::isPositive() const {
    return this->rating >=4.0f;
}
int Review::getTotalReviews() {
    return noReviews;
}
bool Review::operator==(const Review& other) const {
    return strcmp(this->ProductName,other.ProductName)==0;
}
bool Review::operator<(const Review& other) const {
    return this->rating < other.rating;
}
istream& operator>>(istream& in,Review& obj) {
    char buff[256];

    cout<<"Product name: ";
    in.getline(buff,256);
    delete[] obj.ProductName;
    obj.ProductName = new char[strlen(buff)+1];
    strcpy(obj.ProductName,buff);

    cout<<"Rating : ";
    in >> obj.rating;
    if (obj.rating<1.0f || obj.rating>5.0f)
        obj.rating=0.0f;
    in.ignore();

    cout<<"Comment : ";
    in.getline(buff,256);
    strncpy(obj.comment,buff,255);
    obj.comment[255]='\0';

    cout<<"Verified Purchase:";
    in >> obj.isVerified;
    in.ignore();
    return in;
}

ostream& operator<<(ostream& out,const Review& obj) {
    out << "[Review#"<<obj.id<<"]"
        <<obj.ProductName<<"| "
        <<obj.rating<<"/5.0 | "
        <<(obj.isVerified ? "Verified" : "Not Verified") <<"|"
        <<obj.helpfulVotes<<"helpful votes"
        <<" \n  Comment: "<<obj.comment;
    return out;
}

class User {
private:
    static int noUsers;
    const int id;
    char* name;
    char email[100];
    char* password;
    double totalSpent;
    long LoyaltyPoints;
    float* spendHistory;
    int HistorySize;
    bool isGold;
    Cart* cart;
public:
    User();
    User(char* name,char* email,char* password);
    User(const User& obj);
    User& operator=(const User& obj);
    ~User();

    int getId() const;
    const char* getName() const;
    const char* getEmail() const;
    double getTotalSpent() const;
    long getLoyaltyPoints() const;
    bool getIsGold() const;
    int getHistorySize() const;
    static int getnoUsers();

    void setName(const char* name);
    void setEmail(const char* email);
    void setPassword(const char* password);

    void AddPurchase(float amount);
    void RedeemPoints(long Points);
    float getAverageSpend() const;

    bool operator==(const User& other) const;
    bool operator<(const User& other) const;

    friend istream& operator>>(istream& in,User& obj);
    friend ostream& operator<<(ostream& out,const User& obj);
};
int User::noUsers=0;
User::User() : id(++noUsers) {
    this->name=new char[4];
    strcpy(this->name,"N/A");
    strcpy(this->email,"N/A");
    this->password=new char[4];
    strcpy(this->password,"N/A");
    this->totalSpent=0.0;
    this->LoyaltyPoints=0;
    this->spendHistory=nullptr;
    this->HistorySize=0;
    this->isGold=false;
    this->cart=nullptr;
}
User::User(char* name,char* email,char* password) : id(++noUsers) {
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    strcpy(this->email,email);
    this->password=new char[strlen(password)+1];
    strcpy(this->password,password);
    this->totalSpent=0.0;
    this->LoyaltyPoints=0;
    this->spendHistory=nullptr;
    this->HistorySize=0;
    this->isGold=false;
    this->cart=nullptr;
}
User::User(const User& obj) : id(++noUsers) {
    this->name=new char[strlen(obj.name)+1];
    strcpy(this->name,obj.name);
    strcpy(this->email,obj.email);
    this->password= new char[strlen(obj.password)+1];
    strcpy(this->password,obj.password);
    this->totalSpent=obj.totalSpent;
    this->LoyaltyPoints=obj.LoyaltyPoints;
    this->HistorySize=obj.HistorySize;
    this->isGold=obj.isGold;
    if (obj.spendHistory!=nullptr) {
        this->spendHistory=new float[obj.HistorySize];
        for (int i=0; i<obj.HistorySize; i++)
            this->spendHistory[i]=obj.spendHistory[i];
    } else {
        this->spendHistory=nullptr;
    }
    if (obj.cart!=nullptr)
        this->cart=new Cart(*obj.cart);
    else this->cart=nullptr;

}
User& User::operator=(const User& obj) {
    if (this == &obj)
        return *this;
    delete[] this->name;
    delete[] this->password;
    delete[] this->spendHistory;
    delete this->cart;
    this->name=new char[strlen(obj.name)+1];
    strcpy(this->name,obj.name);
    strcpy(this->email,obj.email);
    this->password=new char[strlen(obj.password)+1];
    strcpy(this->password,obj.password);
    this->totalSpent=obj.totalSpent;
    this->LoyaltyPoints=obj.LoyaltyPoints;
    this->HistorySize=obj.HistorySize;
    this->isGold=obj.isGold;
    if (obj.spendHistory!=nullptr) {
        this->spendHistory=new float[obj.HistorySize];
        for (int i=0;i< obj.HistorySize; i++)
            this->spendHistory[i]=obj.spendHistory[i];
    } else {
        this->spendHistory=nullptr;
    }
    return *this;
}
User::~User() {
    delete[] this->name;
    delete[] this->password;
    delete[] this->spendHistory;
    delete this->cart;
}

int User::getId() const {
    return this->id;
}
const char* User::getName() const {
    return this->name;
}
const char* User::getEmail() const {
    return this->email;
}
double User::getTotalSpent() const {
    return this->totalSpent;
}
long User::getLoyaltyPoints() const {
    return this->LoyaltyPoints;
}
bool User::getIsGold() const {
    return this->isGold;
}
int User::getHistorySize() const {
    return this->HistorySize;
}
int User::getnoUsers() {
    return noUsers;
}

void User::setName(const char* name) {
    delete[] this->name;
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
}
void User::setEmail(const char* email) {
    strcpy(this->email,email);
}
void User::setPassword(const char* password) {
    delete[] this->password;
    this->password=new char[strlen(password)+1];
    strcpy(this->password,password);
}

void User::AddPurchase(float amount) {
    float* temp= new float[this->HistorySize +1];
    for (int i=0;i<this->HistorySize;i++)
        temp[i]=this->spendHistory[i];
    temp[this->HistorySize]=amount;
    delete[] this->spendHistory;
    this->spendHistory=temp;
    this->HistorySize++;
    this->totalSpent+=amount;
    this->LoyaltyPoints+= (long)amount /10;
    if (this->LoyaltyPoints >= 500)
        this->isGold=true;
}

void User::RedeemPoints(long points) {
    if (points<= this->LoyaltyPoints)
        this->LoyaltyPoints-=points;
}

float User::getAverageSpend() const {
    if ( this-> HistorySize==0)
        return 0.0f;
    float total=0.0f;
    for (int i=0;i<this->HistorySize;i++)
        total+=this->spendHistory[i];
    return total/this->HistorySize;
}

bool User::operator==(const User& other) const {
    return strcmp(this->email,other.email)==0;
}
bool User::operator<(const User& other) const {
    return this->LoyaltyPoints < other.LoyaltyPoints;
}

istream& operator>>(istream& in, User& obj) {
    char buff[256];

    cout<<"Name: ";
    in.getline(buff,256);
    delete[] obj.name;
    obj.name=new char[strlen(buff)+1];
    strcpy(obj.name,buff);

    cout<<"Email: ";
    in.getline(buff,256);
    strcpy(obj.email,buff);

    cout<<"Password: ";
    in.getline(buff,256);
    delete[] obj.password;
    strcpy(obj.password,buff);

    return in;
}

ostream& operator<<(ostream& out, const User& obj) {
     out<<" [User#" <<obj.id<<"]"
        <<obj.name<<" | "
        <<obj.email<<" | "
        <<(obj.isGold ? "Gold member" : "Regular member") <<" | "
        <<"Points: "<<obj.LoyaltyPoints<<" | "
        <<"Total spent: "<<obj.totalSpent<<" lei ";
    if (obj.cart!=nullptr)
        out<< "\n   Cart: "<<*obj.cart;
    else
        out<< "\n   Cart: empty";
    return out;
}

class Menu {
    vector<Product*> products;
    vector<User*> users;
    vector<Review*> reviews;

    void PrintProducts() const;
    void PrintUsers() const;
    void PrintReviews() const;
    int PickProduct() const;
    int PickUser() const;
    int PickReview() const;

public:
    Menu()=default;
    ~Menu();
    void run();
    void productMenu();
    void userMenu();
    void reviewMenu();
    void assignMenu();
};

Menu::~Menu() {
    for (int i=0;i<products.size();i++)
        delete products[i];
    for (int i=0;i<users.size();i++)
        delete users[i];
    for (int i=0;i<reviews.size();i++)
        delete reviews[i];
}

void Menu::PrintProducts() const {
    if (products.empty()) {
        cout<<"No products registered.\n";
        return;
    }
    for (int i=0;i<products.size();i++)
        cout<<" "<<i<<". "<<*products[i]<<"\n";
}

void Menu::PrintUsers() const {
    if (users.empty()) {
        cout<<"No users registered.\n";
        return;
    }
    for (int i=0;i<users.size();i++)
        cout<<" "<<i<<". "<<*users[i]<<"\n";
}

void Menu::PrintReviews() const {
    if (reviews.empty()) {
        cout<<"No reviews registered.\n";
        return;
    }
    for (int i=0;i<reviews.size();i++)
        cout<<" "<<i<<". "<<*reviews[i]<<"\n";
}

int Menu::PickProduct() const {
    if (products.empty()) {
        cout<<"No products available.\n";
        return -1;
    }
    PrintProducts();
    cout<<"Pick product index(-1 to cancel):";
    int idx;
    cin>>idx;
    cin.ignore();
    if (idx <0 || idx >= (int)products.size()) return -1;
    return idx;
}

int Menu::PickUser() const {
    if (users.empty()) {
        cout<<"No users available.\n";
        return -1;
    }
    PrintUsers();
    cout<<"Pick user index(-1 to cancel):";
    int idx;
    cin>>idx;
    cin.ignore();
    if (idx <0 || idx >= (int)users.size() ) return -1;
    return idx;
}
int Menu::PickReview() const {
    if (reviews.empty()) {
        cout<<"No reviews available.\n";
        return -1;
    }
    PrintReviews();
    cout<<"Pick review index(-1 to cancel):";
    int idx;
    cin>>idx;
    cin.ignore();
    if (idx <0 || idx >= (int)reviews.size() ) return -1;
    return idx;
}

void Menu::run() {
    while (true) {
        cout<<"Sephora Account Management System";
        cout<<"0 - Exit\n";
        cout<<"1 - Products\n";
        cout<<"2 - Users\n";
        cout<<"3 - Reviews\n";
        cout<<"4 - Assign product to cart\n";
        cout<<"Option: ";

        int option;
        cin>>option;
        cin.ignore();

        switch (option) {
            case 0:
                cout<<"Goodbye\n";
                return;
            case 1:
                productMenu();
                break;
            case 2:
                userMenu();
                break;
            case 3:
                reviewMenu();
                break;
            case 4:
                assignMenu();
                break;
            default:
                cout<<"Invalid option.\n";
        }
    }
}

void Menu::productMenu() {
    while (true) {
        cout<<"Product Menu  \n";
        cout<<"0 - Back\n";
        cout<<"1 - Create Product\n";
        cout<<"2 - List Product\n";
        cout<<"3 - Edit Product\n";
        cout<<"4 - Remove Product\n";
        cout<<"5 - Restock Product\n";
        cout<<"6 - Compare two products\n";
        cout<<"7 - Sort products by price\n";
        cout<<"Option: ";

        int option;
        cin>>option;
        cin.ignore();

        switch (option) {
            case 0:
                return;
            case 1: {
                Product* p= new Product();
                cin>> *p;
                products.push_back(p);
                cout<<"  Product created.\n";
                break;
            }
            case 2:
                PrintProducts();
                break;
            case 3: {
                int idx= PickProduct();
                if (idx ==-1) break;
                cin>> *products[idx];
                cout<< "Product updated.\n";
                break;
            }
            case 4: {
                int idx = PickProduct();
                if (idx == -1) break;
                delete products[idx];
                products.erase(products.begin() + idx);
                cout << "  Product removed.\n";
                break;
            }
            case 5: {
                int idx = PickProduct();
                if (idx == -1) break;
                int quantity;
                cout << "  Quantity to restock: ";
                cin >> quantity;
                cin.ignore();
                products[idx]->restock(quantity);
                cout << "  Product restocked.\n";
                break;
            }
            case 6: {
                cout << "  First product:\n";
                int a = PickProduct();
                if (a == -1) break;
                cout << "  Second product:\n";
                int b = PickProduct();
                if (b == -1) break;
                if (*products[a] == *products[b])
                    cout << "  Same product.\n";
                else
                    cout << "  Different products.\n";
                break;
            }
            case 7:{
                for (int i = 0; i < products.size(); i++)
                    for (int j = 0; j + 1 < products.size() - i; j++)
                        if (*products[j + 1] < *products[j])
                            swap(products[j], products[j + 1]);
                cout << "  Products sorted by price.\n";
                PrintProducts();
                break;
            }
            default:
                cout<<"Invalid option.\n";
        }
    }
}

void Menu::userMenu() {
    while (true) {
        cout<<"User Menu \n";
        cout<<"0 - Back\n";
        cout<<"1 - Create User\n";
        cout<<"2 - List User\n";
        cout<<"3 - Edit User\n";
        cout<<"4 - Remove User\n";
        cout<<"5 - Add purchase\n";
        cout<<"6 - Redeem points\n";
        cout<<"7 - Average spend\n";
        cout<<"8 - Compare two users\n";
        cout<<"9 - Sort users by loyalty points\n";
        cout<<"Option: ";

        int option;
        cin>>option;
        cin.ignore;

        switch(option) {
            case 0:
                return;
            case 1: {
                User* u= new User();
                cin>> *u;
                users.push_back(u);
                cout<<"  User created.\n";
                break;
            }
            case 2:
                PrintUsers();
                break;
            case 3: {
                int idx = PickUser();
                if (idx == -1) break;
                cin >> *users[idx];
                cout << "  User updated.\n";
                break;
            }
            case 4: {
                int idx = PickUser();
                if (idx == -1) break;
                delete users[idx];
                users.erase(users.begin() + idx);
                cout << "  User removed.\n";
                break;
            }
            case 5: {
                int idx = PickUser();
                if (idx == -1) break;
                float amount;
                cout << "  Amount spent: ";
                cin >> amount;
                cin.ignore();
                users[idx]->AddPurchase(amount);
                cout << "  Purchase added.\n";
                break;
            }
            case 6: {
                int idx = PickUser();
                if (idx == -1) break;
                long points;
                cout << "  Points to redeem: ";
                cin >> points;
                cin.ignore();
                users[idx]->RedeemPoints(points);
                cout << "  Points redeemed.\n";
                break;
            }
            case 7: {
                int idx = PickUser();
                if (idx == -1) break;
                cout << "  Average spend: " << users[idx]->getAverageSpend() << " lei\n";
                break;
            }
            case 8: {
                cout << "  First user:\n";
                int a = PickUser();
                if (a == -1) break;
                cout << "  Second user:\n";
                int b = PickUser();
                if (b == -1) break;
                if (*users[a] == *users[b])
                    cout << "  Same user.\n";
                else
                    cout << "  Different users.\n";
                break;
            }
            case 9: {
                for (int i = 0; i < users.size(); i++)
                    for (int j = 0; j + 1 < users.size() - i; j++)
                        if (*users[j + 1] < *users[j])
                            swap(users[j], users[j + 1]);
                cout << "  Users sorted by loyalty points.\n";
                PrintUsers();
                break;
            }
            default:
                cout<<"Invalid option.\n";
        }
    }
}

void Menu::reviewMenu() {
    while (true) {
        cout<<"Review Menu \n";
        cout<<"0 - Back\n";
        cout<<"1 - Create Review\n";
        cout<<"2 - List Reviews\n";
        cout<<"3 - Edit Review\n";
        cout<<"4 - Remove Review\n";
        cout<<"5 - Mark Review as Helpful\n";
        cout<<"6 - Check if review is positive\n";
        cout<<"7 - Compare two reviews\n";
        cout<<"8 - Sort reviews by review points\n";
        cout<<"Option: ";

        int option;
        cin>>option;
        cin.ignore();

        switch (option) {
            case 0:
                return;
            case 1: {
                Review* r = new Review();
                cin >> *r;
                reviews.push_back(r);
                cout << "  Review created.\n";
                break;
            }
            case 2:
                PrintReviews();
                break;
            case 3: {
                int idx = PickReview();
                if (idx == -1) break;
                cin >> *reviews[idx];
                cout << "  Review updated.\n";
                break;
            }
            case 4: {
                int idx = PickReview();
                if (idx == -1) break;
                delete reviews[idx];
                reviews.erase(reviews.begin() + idx);
                cout << "  Review removed.\n";
                break;
            }
            case 5: {
                int idx = PickReview();
                if (idx == -1) break;
                reviews[idx]->MarkHelpful();
                cout << "  Review marked as helpful.\n";
                break;
            }
            case 6: {
                int idx = PickReview();
                if (idx == -1) break;
                if (reviews[idx]->isPositive())
                    cout << "  Review is positive.\n";
                else
                    cout << "  Review is not positive.\n";
                break;
            }
            case 7: {
                cout << "  First review:\n";
                int a = PickReview();
                if (a == -1) break;
                cout << "  Second review:\n";
                int b = PickReview();
                if (b == -1) break;
                if (*reviews[a] == *reviews[b])
                    cout << "  Same product reviewed.\n";
                else
                    cout << "  Different products reviewed.\n";
                break;
            }
            case 8: {
                for (int i = 0; i < reviews.size(); i++)
                    for (int j = 0; j + 1 < reviews.size() - i; j++)
                        if (*reviews[j + 1] < *reviews[j])
                            swap(reviews[j], reviews[j + 1]);
                cout << "  Reviews sorted by rating.\n";
                PrintReviews();
                break;
            }
            default:
                cout << "  Invalid option.\n";
        }
    }
}
int main() {
   return 0;
}