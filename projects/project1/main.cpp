#include <iostream>
#include <string.h>
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
int User::getnoUsers() const {
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
int main() {
   return 0;
}