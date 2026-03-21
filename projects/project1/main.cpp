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
int main() {
    return 0;
}