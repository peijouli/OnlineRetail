#include <sstream>
#include <iomanip>
#include "product.h"

using namespace std;

Product::Product(const std::string category, const std::string name, double price, int qty) :
    name_(name),
    price_(price),
    qty_(qty),
    category_(category)
{

}

Product::~Product()
{

}


double Product::getPrice() const
{
    return price_;
}


std::string Product::get_name() 
{
    return name_;
}

std::string Product::getName() const
{
    return name_;
}

void Product::subtractQty(int num)
{
    qty_ -= num;
}

int Product::getQty() const
{
    return qty_;
}

/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Product::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}

void Product::dump(std::ostream& os) const
{
    os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ 
        << "\n" << endl; 
    for (int i = 0; (unsigned int) i < review_vector.size(); i ++){
        review_vector[i]->dump(os);
    }
}


void Product::setAverage(double d){
    average_rating = d; 

    
    
}
    
double Product::getAverage() const{
    return average_rating ;
}


std::vector<Review*> Product::get_vector(){
    return review_vector;
}

