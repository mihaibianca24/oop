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
int main() {
    return 0;
}