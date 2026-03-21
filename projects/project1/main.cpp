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
};